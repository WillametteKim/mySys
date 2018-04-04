sudo rmmod simple
@/dev sudo rm simple

make
sudo insmod simple.ko
dmesg
sudo mknod -m 666 /dev/simple c ? ?