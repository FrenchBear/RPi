#!/bin/sh
# Raspberry Pi info
# 2019-07-25 PV


echo File /sys/firmware/devicetree/base/model:
cat /sys/firmware/devicetree/base/model
echo 
echo 

echo Command gpio -v:
gpio -v | tail -n 5
echo 

echo Command uname -a:
uname -a
echo 

echo Command lsb_release -da:
lsb_release -da
echo 

echo Command hostnamectl:
hostnamectl
echo 

echo Can also run command pinout
