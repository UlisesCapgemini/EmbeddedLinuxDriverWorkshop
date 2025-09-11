# Instalar QEMU si aún no lo tienes
# sudo apt install -y qemu-system-arm qemu-utils

# Crear un disco virtual para pruebas
# qemu-img create -f qcow2 virtio-test.qcow2 8G

# Ejecutar QEMU con soporte para virtio
qemu-system-arm -M virt -cpu cortex-a15 -m 1G \
    -kernel /path/to/zImage \
    -dtb /path/to/vexpress-v2p-ca15-tc1.dtb \
    -drive file=virtio-test.qcow2,if=virtio \
    -net nic,model=virtio -net user \
    -nographic