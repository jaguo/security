# encoding: utf-8
#!/usr/bin/python

import os
import hashlib


instexec = []

Directory = "/home/guo/cpu2006/benchspec/CPU2006/"
configPath = "/home/guo/cpu2006/config/instruction.cfg"


def instruction():
    i = 0
    for dirs in os.listdir(Directory):
        if os.path.isdir(Directory + dirs):
            temp = Directory
            temp += dirs
            temp += "/exe/"
            if os.path.isdir(temp):
                for filepath in os.listdir(temp):
                    temp += filepath
                    instexec.append(temp)
                    i = i + 1
            # print instexec[i]


def md5sum(filename):
    filepath = ''
    flag = False
    for i in instexec:
        if i.find(filename) != -1:
            filepath = i
            flag = True
    if flag:
        fd = open(filepath)
        fcont = fd.read()
        fd.close()
        fmd5 = hashlib.md5(fcont)
        return fmd5.hexdigest()
    else:
        return ''

def readConfig():
    config = open(configPath, 'r')
    lines = config.readlines()
    config.close()
    md5_line = 0
    filename = ''
    for i in range(len(lines) - 1):
        if lines[i].find('__MD5__') != -1:
            md5_line = i
            break

    print 'md5 line:' + str(md5_line)
    for i in range(md5_line, len(lines) - 1):
        if lines[i].find(':') != -1 and not lines[i].startswith('#'):
            filename = lines[i].split('=')[0]
        if lines[i].startswith('exemd5'):
            lines[i] = 'exemd5=' + md5sum(filename) + '\n'
            print lines[i],
    config = open(configPath, 'w+')
    config.writelines(lines)
    config.close()

instruction()
# expand = {};
for exe in instexec:
    print exe
    raw = os.path.getsize(exe)
    os.system("/home/guo/Desktop/canary/safeCanary " + exe)
    inst = os.path.getsize(exe)
    # print "raw : " + str(raw)
    # print "inst: " + str(inst)
    print str(raw) + ' ' + str(inst) + ' ' + str(inst / float(raw))

readConfig()
