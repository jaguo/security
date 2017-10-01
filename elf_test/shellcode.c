#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

// " eb 19 31 c0 31 db 31 c9 31 d2 b0 04 b2 0e 59 b3 01 cd 80 31 c0 b0 01 31 db cd 80 e8 e2 ff ff ff 48 65 6c 6c 6f 2c 20 57 6f 72 6c 64 21 0a";
//  eb 10 48 31 c0 5f 48 31 f6 48 31 d2 48 83 c0 3b 0f 05 e8 eb ff ff ff 2f 62 69 6e 2f 2f 73 68
// 

// unsigned char shellcode[] = \
// "\xeb\x19\x31\xc0\x31\xdb\x31\xc9\x31\xd2\xb0\x04\xb2\x0e\x59\xb3\x01\xcd\x80\x31\xc0\xb0\x01"\
// "\x31\xdb\xcd\x80\xe8\xe2\xff\xff\xff\x48\x65\x6c\x6c\x6f\x2c\x20\x57\x6f\x72\x6c\x64\x21\x0a";
// int main()
// {
//     int (*ret)() = (int(*)())shellcode;
//     ret();
// }

int main()
{
    system("/bin//sh");
    return 0;
}