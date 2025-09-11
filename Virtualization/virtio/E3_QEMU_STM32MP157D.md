# Ejercicio 3 Adaptado: Explorar Virtio en QEMU con STM32MP157D
Perfecto, si ya tienes el SDK y el kernel compilado para la STM32MP157D, podemos adaptar el ejercicio para trabajar específicamente con esta plataforma. Aquí tienes una versión práctica y directa:

# Ejercicio 3: Explorar Virtio en QEMU con STM32MP157D
# Paso 1: Preparar los archivos necesarios
# Crear un directorio de trabajo
mkdir -p ~/stm32mp1_virtio_test
cd ~/stm32mp1_virtio_test

# Asumiendo que ya tienes el SDK instalado, configura el entorno
source /path/to/your/sdk/environment-setup-cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi
Paso 2: Crear un Device Tree Overlay para Virtio
# Crear un archivo DTS para añadir soporte virtio
cat > stm32mp157d-virtio-overlay.dts << 'EOF'
/dts-v1/;
/plugin/;

/ {
    compatible = "st,stm32mp157d-dk1", "st,stm32mp157";
    
    fragment@0 {
        target = <&soc>;
        __overlay__ {
            virtio_mmio@10100000 {
                compatible = "virtio,mmio";
                reg = <0x10100000 0x1000>;
                interrupts = <GIC_SPI 42 IRQ_TYPE_LEVEL_HIGH>;
                status = "okay";
            };
            
            virtio_mmio@10101000 {
                compatible = "virtio,mmio";
                reg = <0x10101000 0x1000>;
                interrupts = <GIC_SPI 43 IRQ_TYPE_LEVEL_HIGH>;
                status = "okay";
            };
        };
    };
};
EOF

# Compilar el Device Tree Overlay
dtc -@ -I dts -O dtb -o stm32mp157d-virtio-overlay.dtbo stm32mp157d-virtio-overlay.dts
Paso 3: Configurar QEMU para STM32MP157D con soporte Virtio
# Crear un script para lanzar QEMU con la configuración adecuada
cat > run_stm32mp1_virtio.sh << 'EOF'
#!/bin/bash

# Rutas a los archivos necesarios - AJUSTA ESTAS RUTAS según tu entorno
KERNEL="/path/to/your/zImage"
DTB="/path/to/your/stm32mp157d-dk1.dtb"
ROOTFS="/path/to/your/rootfs.ext4"
DTBO="./stm32mp157d-virtio-overlay.dtbo"

# Crear un disco virtual para pruebas con virtio-blk
if [ ! -f virtio-disk.qcow2 ]; then
    qemu-img create -f qcow2 virtio-disk.qcow2 1G
fi

# Ejecutar QEMU con soporte para STM32MP1 y dispositivos virtio
qemu-system-arm \
    -M stm32mp15xx-dk \
    -cpu cortex-a7 \
    -smp 2 \
    -m 512M \
    -kernel ${KERNEL} \
    -dtb ${DTB} \
    -device loader,file=${DTBO},addr=0x4A000000 \
    -append "root=/dev/mmcblk0p2 rw console=ttySTM0,115200 earlycon=stm32-uart,0x4000e000,115200 loglevel=8" \
    -drive file=${ROOTFS},format=raw,if=sd,index=0 \
    -drive file=virtio-disk.qcow2,if=none,format=qcow2,id=hd0 \
    -device virtio-blk-device,drive=hd0 \
    -netdev user,id=eth0 \
    -device virtio-net-device,netdev=eth0 \
    -device virtio-serial-device \
    -chardev stdio,id=virtcon0 \
    -device virtconsole,chardev=virtcon0 \
    -nographic
EOF

chmod +x run_stm32mp1_virtio.sh
# Paso 4: Crear un módulo de kernel simple para probar Virtio
cat > virtio_test.c << 'EOF'
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/virtio.h>
#include <linux/virtio_config.h>
#include <linux/virtio_ids.h>
#include <linux/virtio_blk.h>

static int virtio_devices_found = 0;

static int virtio_test_probe(struct virtio_device *vdev)
{
    virtio_devices_found++;
    
    printk(KERN_INFO "virtio_test: Dispositivo Virtio encontrado!\n");
    printk(KERN_INFO "virtio_test: ID del dispositivo: %d\n", vdev->id.device);
    
    switch(vdev->id.device) {
        case VIRTIO_ID_BLOCK:
            printk(KERN_INFO "virtio_test: Dispositivo de bloque Virtio\n");
            break;
        case VIRTIO_ID_NET:
            printk(KERN_INFO "virtio_test: Dispositivo de red Virtio\n");
            break;
        case VIRTIO_ID_CONSOLE:
            printk(KERN_INFO "virtio_test: Consola Virtio\n");
            break;
        default:
            printk(KERN_INFO "virtio_test: Otro tipo de dispositivo Virtio\n");
    }
    
    return 0;
}

static void virtio_test_remove(struct virtio_device *vdev)
{
    printk(KERN_INFO "virtio_test: Dispositivo Virtio eliminado\n");
}

/* Tabla de dispositivos compatibles - acepta cualquier dispositivo virtio */
static const struct virtio_device_id id_table[] = {
    { VIRTIO_ID_NET, VIRTIO_DEV_ANY_ID },
    { VIRTIO_ID_BLOCK, VIRTIO_DEV_ANY_ID },
    { VIRTIO_ID_CONSOLE, VIRTIO_DEV_ANY_ID },
    { VIRTIO_ID_RNG, VIRTIO_DEV_ANY_ID },
    { VIRTIO_ID_BALLOON, VIRTIO_DEV_ANY_ID },
    { VIRTIO_ID_RPMSG, VIRTIO_DEV_ANY_ID },
    { VIRTIO_ID_SCSI, VIRTIO_DEV_ANY_ID },
    { VIRTIO_ID_9P, VIRTIO_DEV_ANY_ID },
    { VIRTIO_ID_RPROC_SERIAL, VIRTIO_DEV_ANY_ID },
    { VIRTIO_ID_CAIF, VIRTIO_DEV_ANY_ID },
    { VIRTIO_ID_GPU, VIRTIO_DEV_ANY_ID },
    { VIRTIO_ID_INPUT, VIRTIO_DEV_ANY_ID },
    { VIRTIO_ID_VSOCK, VIRTIO_DEV_ANY_ID },
    { VIRTIO_ID_CRYPTO, VIRTIO_DEV_ANY_ID },
    { 0 },
};

static struct virtio_driver virtio_test_driver = {
    .driver.name = KBUILD_MODNAME,
    .driver.owner = THIS_MODULE,
    .id_table = id_table,
    .probe = virtio_test_probe,
    .remove = virtio_test_remove,
};

static int __init virtio_test_init(void)
{
    int ret;
    
    printk(KERN_INFO "virtio_test: Inicializando módulo de prueba Virtio\n");
    
    ret = register_virtio_driver(&virtio_test_driver);
    if (ret < 0) {
        printk(KERN_ERR "virtio_test: Error al registrar el driver Virtio\n");
        return ret;
    }
    
    printk(KERN_INFO "virtio_test: Driver registrado correctamente\n");
    return 0;
}

static void __exit virtio_test_exit(void)
{
    printk(KERN_INFO "virtio_test: Desregistrando driver, se encontraron %d dispositivos\n", 
           virtio_devices_found);
    unregister_virtio_driver(&virtio_test_driver);
}

module_init(virtio_test_init);
module_exit(virtio_test_exit);

MODULE_AUTHOR("Tu Nombre");
MODULE_DESCRIPTION("Módulo de prueba para dispositivos Virtio en STM32MP1");
MODULE_LICENSE("GPL");
EOF

# Crear el Makefile para compilar el módulo
cat > Makefile << 'EOF'
# Si estás usando el SDK de STM32MP1, asegúrate de que el entorno esté configurado
# source /path/to/your/sdk/environment-setup-cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi

obj-m += virtio_test.o

KERNEL_SRC ?= /path/to/your/kernel/source

all:
	make -C $(KERNEL_SRC) M=$(PWD) modules

clean:
	make -C $(KERNEL_SRC) M=$(PWD) clean
EOF
# Paso 5: Compilar el módulo y prepararlo para la prueba
# Ajusta la ruta al código fuente del kernel en el Makefile
sed -i "s|/path/to/your/kernel/source|$(echo $KERNEL_SRC)|g" Makefile

# Compilar el módulo
make
# Paso 6: Preparar un script para probar el módulo en QEMU
cat > test_virtio_module.sh << 'EOF'
#!/bin/bash

# Iniciar QEMU con STM32MP1
./run_stm32mp1_virtio.sh &
QEMU_PID=$!

# Esperar a que QEMU arranque
sleep 30

# Usar SSH para conectar y probar el módulo (ajusta la IP según tu configuración)
scp virtio_test.ko root@192.168.7.2:/tmp/
ssh root@192.168.7.2 << 'ENDSSH'
cd /tmp
insmod virtio_test.ko
dmesg | grep virtio_test
lsmod | grep virtio
ls -l /sys/bus/virtio/
ls -l /sys/bus/virtio/devices/
cat /proc/interrupts | grep virtio
ENDSSH

# Opcional: Detener QEMU cuando hayas terminado
# kill $QEMU_PID
EOF

chmod +x test_virtio_module.sh
# Paso 7: Explorar la implementación de Virtio en el kernel
# Examinar los archivos de implementación de Virtio en el kernel de STM32MP1
cd $KERNEL_SRC
find drivers/virtio -type f -name "*.c" | xargs ls -la
find include/linux -name "virtio*.h" | xargs ls -la

# Ver específicamente la implementación de virtio-mmio
less drivers/virtio/virtio_mmio.c
# Paso 8: Experimento práctico - Implementar un driver virtio-blk simple
cat > virtio_blk_simple.c << 'EOF'
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/virtio.h>
#include <linux/virtio_blk.h>
#include <linux/virtio_config.h>
#include <linux/virtio_ids.h>
#include <linux/blkdev.h>
#include <linux/blk-mq.h>
#include <linux/slab.h>

struct virtblk_simple_req {
    struct virtio_blk_outhdr out_hdr;
    u8 status;
    struct scatterlist sg[];
};

struct virtblk_simple {
    struct virtio_device *vdev;
    struct virtqueue *vq;
    struct gendisk *disk;
    struct request_queue *queue;
    unsigned int sg_elems;
    int index;
    spinlock_t lock;
};

static int virtblk_simple_probe(struct virtio_device *vdev)
{
    struct virtblk_simple *vbdev;
    int err;
    
    printk(KERN_INFO "virtblk_simple: Dispositivo de bloque Virtio encontrado\n");
    
    /* Asignar memoria para nuestra estructura de dispositivo */
    vbdev = kzalloc(sizeof(*vbdev), GFP_KERNEL);
    if (!vbdev)
        return -ENOMEM;
    
    vbdev->vdev = vdev;
    spin_lock_init(&vbdev->lock);
    
    /* Aquí iría la inicialización completa del dispositivo de bloque */
    /* Este es un ejemplo simplificado para fines educativos */
    
    vdev->priv = vbdev;
    
    printk(KERN_INFO "virtblk_simple: Inicialización básica completada\n");
    return 0;
}

static void virtblk_simple_remove(struct virtio_device *vdev)
{
    struct virtblk_simple *vbdev = vdev->priv;
    
    printk(KERN_INFO "virtblk_simple: Eliminando dispositivo\n");
    
    /* Aquí iría la limpieza completa del dispositivo */
    
    kfree(vbdev);
}

static const struct virtio_device_id id_table[] = {
    { VIRTIO_ID_BLOCK, VIRTIO_DEV_ANY_ID },
    { 0 },
};

static struct virtio_driver virtblk_simple_driver = {
    .driver.name = KBUILD_MODNAME,
    .driver.owner = THIS_MODULE,
    .id_table = id_table,
    .probe = virtblk_simple_probe,
    .remove = virtblk_simple_remove,
};

static int __init virtblk_simple_init(void)
{
    return register_virtio_driver(&virtblk_simple_driver);
}

static void __exit virtblk_simple_exit(void)
{
    unregister_virtio_driver(&virtblk_simple_driver);
}

module_init(virtblk_simple_init);
module_exit(virtblk_simple_exit);

MODULE_AUTHOR("Tu Nombre");
MODULE_DESCRIPTION("Driver simple para dispositivos virtio-blk");
MODULE_LICENSE("GPL");
EOF

# Actualizar el Makefile para incluir el nuevo módulo
echo "obj-m += virtio_blk_simple.o" >> Makefile
make
# Paso 9: Explorar la comunicación entre host y guest con Virtio
Para entender mejor cómo funciona la comunicación entre host y guest con Virtio, puedes crear un módulo que muestre los detalles de las virtqueues:

cat > virtio_queue_explorer.c << 'EOF'
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/virtio.h>
#include <linux/virtio_config.h>
#include <linux/virtio_ids