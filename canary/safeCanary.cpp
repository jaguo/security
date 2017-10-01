#include <iostream>
#include <cstdio>
#include <cstring>
#include <regex>

// DyninstAPI
#include "BPatch.h"
#include "BPatch_basicBlock.h"
#include "BPatch_flowGraph.h"
#include "BPatch_function.h"
#include "BPatch_object.h"
#include "BPatch_point.h"
#include "BPatch_snippet.h"

//PatchAPI
#include "PatchMgr.h"
#include "PatchModifier.h"
#include "Point.h"
#include "Snippet.h"
#include "Buffer.h"

using namespace std;
using namespace Dyninst;
using namespace Dyninst::InstructionAPI;
using namespace Dyninst::PatchAPI;

vector<Point *> pts;
BPatch *bpatch = new BPatch;
BPatch_image *appImage;
PatchMgrPtr mgr;
SnippetPtr snippet;


// LD_PRELOAD=libredundantguard.so

class MySnippet : public Snippet
{
  public:
    // 4004da:	41 54                	push   %r12
    // 4004dc:	49 89 c4             	mov    %rax,%r12
    // 4004df:	49 c1 fc 20          	sar    $0x20,%r12
    // 4004e3:	44 31 e0             	xor    %r12d,%eax
    // 4004e6:	41 5c                	pop    %r12

    // 4004e8:	49 89 dc             	mov    %rbx,%r12
    // 4004eb:	44 31 e3             	xor    %r12d,%ebx

    // 4004ee:	49 89 cc             	mov    %rcx,%r12
    // 4004f1:	44 31 e1             	xor    %r12d,%ecx

    // 4004f4:	49 89 d4             	mov    %rdx,%r12
    // 4004f7:	44 31 e2             	xor    %r12d,%edx

    // 400500:	49 89 fc             	mov    %rdi,%r12
    // 400503:	44 31 e7             	xor    %r12d,%edi

    // 4004fa:	49 89 f4             	mov    %rsi,%r12
    // 4004fd:	44 31 e6             	xor    %r12d,%esi

    MySnippet(char ch)
    {
        switch (ch)
        {
        case 0x04: //rax
            code[4] = 0xc4;
            code[11] = 0xe0;
            break;
        case 0x1c: //rbx
            code[4] = 0xdc;
            code[11] = 0xe3;
            break;
        case 0x0c: //rcx
            code[4] = 0xcc;
            code[11] = 0xe1;
            break;
        case 0x14: //rdx
            code[4] = 0xd4;
            code[11] = 0xe2;
            break;
        case 0x3c: //rdi
            code[4] = 0xfc;
            code[11] = 0xe7;
            break;
        case 0x34: //rsi
            code[4] = 0xf4;
            code[11] = 0xe6;
            break;
        default:
            // printf("%2X\n", ch);
            break;
        }
    }

    virtual bool generate(Point *pt, Buffer &buf)
    {
        buf.copy((void *)code, sizeof(code)-1);
        return true;
    }

    char code[15] = "\x41\x54"              //push  %r12
                  "\x49\x89\xc4"            //mov   %rax,%r12
                  "\x49\xc1\xfc\x20"        //sar   $0x20,%rax
                  "\x44\x31\xe0"            //xor   %edi, %eax
                  "\x41\x5c";               //pop   %r12
};

bool isCheckCanary(char *rawByte, size_t size)
{
    // find this instruction
    // 64 48 33 04 25 28 00 00 00 	xor    %fs:0x28,%rax
    char date_raw[] = "\x64\x48\x33\x00\x25\x28\x00\x00\x00";
    char date_inst[] ="\x64\x33\x00\x25\xa0\x02\x00\x00";
    bool flag_raw = true;
    bool flag_inst = true;
    for (size_t i = 0; i < size; i ++)
    {
        if (i == 3)
            continue;   //this is a register bit type
        if (rawByte[i] != date_raw[i])
        {
            flag_raw = false;
            break;
        }
    }
    for (size_t i = 0; i < size; i ++)
    {
        if (i == 2)
            continue;   //this is a register bit type
        if (rawByte[i] != date_inst[i])
        {
            flag_inst = false;
            break;
        }
    }

    return (flag_raw || flag_inst);
}

bool overwritePoint(Point *pt, char *date, size_t size) 
{
    //overwrite xor    %fs:0x28,%rax
    //          xor    %fs:0x2a0,%eax

    PatchBlock *block = pt->block();
    ParseAPI::Block *b = block->block();
    Offset off = pt->addr();
    // printf("overwritePoint : %2X\n", off);
    
    ParseAPI::SymtabCodeRegion *r = 
        dynamic_cast<ParseAPI::SymtabCodeRegion*>(b->region());
    
    if (r == NULL)
    {
        cout << "SymtabCodeRegion is null" << endl;
        return false;
    }
    Offset region_off = (Offset)r->getPtrToInstruction(off) - 
                        (Offset)r->symRegion()->getPtrToRawData();
    bool success = false;
    success = r->symRegion()->patchData(region_off, (void *)date, size);
    if (!success)
    {
        cout << "patchData is error" << endl; 
        return false;
    }
    return true;
}

void finishInstrumenting(BPatch_addressSpace *app, const char *newName)
{
    BPatch_binaryEdit *appBin = dynamic_cast<BPatch_binaryEdit *>(app);

    if (!appBin)
    {
        fprintf(stderr, "appBin not defined!\n");
        return;
    }

    // Write binary to file
    if (!appBin->writeFile(newName))
    {
        fprintf(stderr, "writeFile failed\n");
    }
}

void writeCanaryPoint(PatchFunction *func)
{
    const PatchFunction::Blockset &blks = func->blocks();
    char bytes[1024];

    for (auto it = blks.begin(); it != blks.end(); it++)
    {
        PatchBlock *block = *it;
        PatchBlock::Insns insns;
        block->getInsns(insns);
        
        char date[] = "\x90\x64\x33\x00\x25\xa0\x02\x00\x00";
        char date_inst[] = "\x64\x33\x00\x25\xa0\x02\x00\x00";
        // 64 48 33 04 25 28 00 00 00 	xor    %fs:0x28,%rax
        for (auto j = insns.begin(); j != insns.end(); j++)
        {
            // get instruction bytes
            Address addr = (*j).first;
            Instruction::Ptr iptr = (*j).second;
            size_t nbytes = iptr->size();
            assert(nbytes < 1024);
            for (size_t i=0; i<nbytes; i++) {
                bytes[i] = iptr->rawByte(i);
            }

            date[3] = bytes[nbytes - 6];     //raw byte about which register
            date_inst[2] = bytes[nbytes - 6];

            if (isCheckCanary(bytes, nbytes))
            {
                mgr->findPoint(
                    Location::InstructionInstance(func, block, addr),
                    Point::PreInsn,
                    back_inserter(pts));
                
                if (nbytes == 9)
                {
                    if (!overwritePoint(pts.back(), date, 9))
                    {
                        cout << "write error" << endl;
                        exit(1);
                    }
                } else if (nbytes == 8) {
                    if (!overwritePoint(pts.back(), date_inst, 8))
                    {
                        cout << "write error" << endl;
                        exit(1);
                    }
                }

                snippet =  MySnippet::create(new MySnippet(bytes[nbytes - 6]));
                InstancePtr instptr = pts.back()->pushBack(snippet);
                if (instptr == NULL)
                {
                    cerr << "insert snipper error" << endl;
                    exit(1);
                }
            }
        }
    }
}

int main(int argc, char **argv)
{
    BPatch_addressSpace *app = bpatch->openBinary(argv[1]);
    if (app == NULL)
    {
        exit(1);
    }
    mgr = convert(app);
    appImage = app->getImage();
    Patcher patcher(mgr);

    vector<BPatch_module *> *modules = appImage->getModules();
    vector<BPatch_function *> *functions;
    

    for (auto it = modules->begin(); it != modules->end(); ++it)
    {
        BPatch_module * module = *it;
        functions = module->getProcedures();
    }

    for (auto it = functions->begin(); it != functions->end(); ++it)
    {
        PatchFunction *func = convert(*it);
        writeCanaryPoint(func);
    }

    patcher.commit();
    cout << pts.size() << " inst points" << endl;

    string name = string(argv[1]);
    finishInstrumenting(app, name.c_str());
    cout << "Instrumentation Success!" << endl;
    return 0;
}
