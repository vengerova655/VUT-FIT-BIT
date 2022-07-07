#!/bin/bash

#autor: xvenge01 (veronika Vengerova)

#in output file -> outputs from commands
echo "------------------------------------1----------------------------------------"
echo "Creating 4 loops"
for p in {0..3}
do
	echo "Creating disk$p"
	dd if=/dev/zero of="disk$p" bs=1MB count=200 &> output_file.txt
	echo "Creating loop$p"
	losetup "loop$p" "disk$p" &> output_file.txt
done


echo "------------------------------------2----------------------------------------"
echo "Creating RAID1 on the first 2 loop devices"
mdadm --create /dev/md0 --level=mirror --raid-devices=2 /dev/loop0 /dev/loop1 --metadata=0.90 &> output_file.txt
echo "Creating RAID0 on the other 2 loop devices"
mdadm --create /dev/md1 --level=0 --raid-devices=2 /dev/loop2 /dev/loop3 --metadata=0.90 &> output_file.txt


echo "------------------------------------3----------------------------------------"
echo "Making RAID1 accessible to LVM"
pvcreate /dev/md0 &> output_file.txt
echo "Making RAID0 accessible to LVM"
pvcreate /dev/md1 &> output_file.txt
echo "Creating volume group on top of 2 RAID devices"
vgcreate FIT_vg /dev/md0 /dev/md1 &> output_file.txt

echo "------------------------------------4----------------------------------------"
echo "Creating Logical volume FIT_lv1 of size 100MB"
lvcreate FIT_vg -n FIT_lv1 -L100MB &> output_file.txt
echo "Creating Logical volume FIT_lv2 of size 100MB"
lvcreate FIT_vg -n FIT_lv2 -L100MB &> output_file.txt

echo "------------------------------------5----------------------------------------"
echo "Creating ext4 filesystem on FIT_lv1"
mkfs.ext4 /dev/FIT_vg/FIT_lv1 &> output_file.txt

echo "------------------------------------6----------------------------------------"
echo "Creating xfs filesystem on FIT_lv2"
mkfs.xfs /dev/FIT_vg/FIT_lv2 &> output_file.txt

echo "------------------------------------7----------------------------------------"
echo "Creating /mnt/test1"
mkdir /mnt/test1
echo "Creating /mnt/test2"
mkdir /mnt/test2
echo "Mounting FIT_lv1 to /mnt/test1"
mount /dev/FIT_vg/FIT_lv1 /mnt/test1 
echo "Mounting FIT_lv2 to /mnt/test2"
mount /dev/FIT_vg/FIT_lv2 /mnt/test2

echo "------------------------------------8----------------------------------------"
echo "Unmounting filesystem"
umount /mnt/test1
echo "Resizing logical volume FIT_lv1 to full available space in volume group"
lvextend -l +100%FREE /dev/FIT_vg/FIT_lv1 &> output_file.txt
echo "Resizing filesystem"
resize2fs /dev/FIT_vg/FIT_lv1 &> output_file.txt
echo "Mounting ressized filesystem"
mount /dev/FIT_vg/FIT_lv1 /mnt/test1
df -h

echo "------------------------------------9----------------------------------------"
echo "Creating 300MB file with data from /dev/urandom"
dd if=/dev/urandom of=/mnt/test1/big_file bs=1MB count=300 &> output_file.txt

echo "Creating checksum"
sha512sum /mnt/test1/big_file | tee checksumfile.txt

echo "------------------------------------10---------------------------------------"
echo "Creating new disk"
dd if=/dev/zero of=disk4 bs=1MB count=200 &> output_file.txt
echo "Creating 5th loop"
losetup -f disk4 &> output_file.txt
echo "degrading raid array"
mdadm --manage /dev/md0 --fail /dev/loop0 &> output_file.txt
mdadm --manage /dev/md0 --remove /dev/loop0 &> output_file.txt
echo "Adding new disk to raid array"
mdadm --manage /dev/md0 --add /dev/loop4 &> output_file.txt
sleep 5
echo "Changes: "
mdadm --detail /dev/md0
