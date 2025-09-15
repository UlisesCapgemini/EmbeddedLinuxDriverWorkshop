# Instalar QEMU si aún no lo tienes
# sudo apt install -y qemu-system-arm qemu-utils

# Crear un disco virtual para pruebas
# qemu-img create -f qcow2 virtio-test.qcow2 8G
# Desglose del comando:
# qemu-img: herramienta para crear, convertir y modificar imágenes de disco para QEMU.
# create: indica que se va a crear una nueva imagen.
# -f qcow2: especifica el formato de la imagen, en este caso QCOW2, que es eficiente en espacio y permite snapshots.
# virtio-test.qcow2: nombre del archivo de imagen que se va a crear.
# 8G: tamaño del disco virtual, en este caso 8 gigabytes.
# ¿Para qué sirve?
# Este archivo puede ser usado como disco duro virtual en una máquina virtual que corra bajo QEMU/KVM.
# El nombre virtio-test.qcow2 es una imagen de disco virtual en formato QCOW2 (QEMU Copy-On-Write version 2), 
# utilizada por QEMU para emular discos duros en máquinas virtuales.


# Ejecutar QEMU con soporte para virtio
qemu-system-arm -M virt -cpu cortex-a15 -m 1G \
    -kernel /path/to/zImage \
    -dtb /path/to/vexpress-v2p-ca15-tc1.dtb \
    -drive file=virtio-test.qcow2,if=virtio \
    -net nic,model=virtio -net user \
    -nographic