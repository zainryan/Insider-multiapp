#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: ./install.sh AGFI_ID"
    exit -1
fi

if [ -d "driver/dma" ]; then
    make -C driver/dma
else
    echo "folder driver/dma doest not exist!"
    exit -1
fi

if [ -d "driver/nvme" ]; then
    make -C driver/nvme
else
    echo "folder driver/nvme doest not exist!"
    exit -1
fi

AGFI_ID=$1

if mount | grep /mnt > /dev/null; then
    sudo umount /dev/nvme_fpga 2>/dev/null \
	|| { echo 'umount failed!'; exit 1;}
fi

if lsmod | grep fpga_dma > /dev/null; then
    sudo rmmod fpga_dma 2>/dev/null \
	|| { echo 'rmmod fpga_dma failed!'; exit 1;}
fi

if lsmod | grep nvme_fpga > /dev/null; then
    sudo rmmod nvme_fpga 2>/dev/null \
	|| { echo 'rmmod nvme_fpga failed!'; exit 1;}
fi

if ls /dev | grep fpga_dma$ > /dev/null; then
    sudo rm /dev/fpga_dma 2>/dev/null \
	|| { echo 'rm /dev/fpga_dma failed!'; exit 1;}
fi

sudo fpga-load-local-image -S 0 -I $AGFI_ID \
    || { echo 'load fpga image failed!'; exit 1;}

sudo insmod driver/dma/fpga_dma.ko \
    || { echo 'insmod fpga_dma failed!'; exit 1;}

sudo insmod driver/nvme/nvme_fpga.ko \
    || { echo 'insmod nvme_fpga failed!'; exit 1;}

sudo mknod /dev/fpga_dma c 88 0 \
    || { echo 'mknod failed!'; exit 1;}

sudo mkfs.xfs /dev/nvme_fpga \
    || { echo 'mkfs failed!'; exit 1;}

sudo mount /dev/nvme_fpga /mnt \
    || { echo 'mount failed!'; exit 1;}

sudo chown centos:centos -R /mnt \
    || { echo 'chown /mnt failed!'; exit 1;}

sudo chown centos:centos /sys/devices/pci0000:00/0000:00:1d.0/resource0 \
    || { echo 'chown FPGA PCIe device failed!'; exit 1;}

sudo chown centos:centos /dev/fpga_dma \
    || { echo 'chown /dev/fpga_dma failed!'; exit 1;}
