#!/bin/sh

source /home/guo/cpu2006/shrc
runspec -c /home/guo/cpu2006/config/gcc43.cfg --action=build -T base int
runspec -c /home/guo/cpu2006/config/gcc43.cfg --reportable -T base int
python /home/guo/Desktop/canary/instruction.py
rm -rf /home/guo/cpu2006/benchspec/C*/*/run/
runspec -c /home/guo/cpu2006/config/instruction.cfg --reportable -T int