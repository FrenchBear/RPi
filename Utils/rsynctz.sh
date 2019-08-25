#!/bin/sh

# rsynctz.sh
# Synchronize pi home to //teraz/backup_rpi/<system_name>
# 2019-08-25	PV		Adaptation to teraz

cd ~
sudo mount //teraz/backup_rpi /mnt -o user=tera -o pass='Astérix!' -o vers=1.0 && {
	root=/mnt/`uname -n`
	[ -d $root ] || mkdir $root
	sudo rsync -rtv --delete --exclude='.cache/*' --exclude='.local/share/Trash/*' --exclude='GitHub/*' --exclude='GitVSTS/*' --copy-links --safe-links . $root/
	sudo umount /mnt
}
