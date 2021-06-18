#! /bin/sh
cd ~/..
sudo tar cvfpz pi-`uname -n`.`date +%Y-%m-%d`.tgz --exclude=.cache pi
