System: ubuntu16.04  
Dyninst: 9.3.3

使用dyninst进行静态二进制插桩实验，主要实现canary保护，需要使用动态库

```sh
tree
.
├── benchmark.sh                            //change benchmark config's MD5
├── CMakeCache.txt
├── CMakeFiles 
├── cmake_install.cmake
├── CMakeLists.txt
├── demo.cpp
├── dynamic_encrypt_redundant_stackguard
│   ├── dynencryptredundantguard.cpp
│   ├── dynencryptredundantguard.h
│   ├── dynencryptredundantguard.so
│   ├── libredundantguard.cpp
│   ├── libredundantguard.h
│   ├── libredundantguard.so                //dynamic lib
│   ├── Makefile
│   ├── offsets.h
│   └── README
├── infect                                  //using C static instrumentation
│   ├── demo2
│   ├── demo2.c
│   ├── infect
│   ├── infect.1
│   ├── infect.c
│   ├── isdynamic.py
│   ├── Makefile
│   ├── out.txt
│   ├── perlbench_base.ia64-gcc42
│   └── README
├── instruction.py                      //python instruction script and change benchmark config's MD5
├── libinstCanary.cpp
├── Makefile
├── safeCanary
└── safeCanary.cpp                      //using dyninst static instrumentation
```
