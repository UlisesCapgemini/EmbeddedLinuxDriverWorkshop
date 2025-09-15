#!/bin/bash

# Rutas a los archivos necesarios - AJUSTA ESTAS RUTAS según tu entorno
KERNEL="/home/victorb/stm32mp-openstlinux-6.6-yocto-scarthgap-mpu-v25.06.11/sources/ostl-linux/linux-stm32mp-6.6.78-stm32mp-r2-r0/build/arch/arm/boot/zImage"
DTB="/home/victorb/stm32mp-openstlinux-6.6-yocto-scarthgap-mpu-v25.06.11/sources/ostl-linux/linux-stm32mp-6.6.78-stm32mp-r2-r0/build/arch/arm/boot/dts/st/stm32mp157d-dk1.dtb"
ROOTFS="/home/victorb/stm32mp1-openstlinux-6.6-yocto-scarthgap-mpu-v25.06.11/images/stm32mp1/st-image-weston-openstlinux-weston-stm32mp1.rootfs.ext4"
DTBO="./stm32mp157d-virtio-overlay.dtbo"

# Crear un disco virtual para pruebas con virtio-blk
if [ ! -f virtio-disk.qcow2 ]; then
    qemu-img create -f qcow2 virtio-disk.qcow2 1G
fi

# Ejecutar QEMU con soporte para STM32MP1 y dispositivos virtio
qemu-system-arm \
    -M virt \
    -cpu cortex-a7 \
    -smp 2 \
    -m 512M \
    -kernel ${KERNEL} \
    -dtb ${DTB} \
    -device loader,file=${DTBO},addr=0x4A000000 \
    -append "root=/dev/vda rw console=ttyAMA0 earlycon=pl011,0x9000000 loglevel=8" \
    -drive file=${ROOTFS},format=raw,if=virtio \
    -drive file=virtio-disk.qcow2,if=none,format=qcow2,id=hd0 \
    -device virtio-blk-device,drive=hd0 \
    -netdev user,id=eth0 \
    -device virtio-net-device,netdev=eth0 \
    -device virtio-serial-device \
    -chardev pty,id=virtcon0 \
    -device virtconsole,chardev=virtcon0 \
    -nographic