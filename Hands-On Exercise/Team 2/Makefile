# Kernel module build
obj-m += snd-soc-mh-i2s-mic.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
# for target stm32mp1 change $(HOME) by your directory if needed
KERNEL_SRC_PATH=$(HOME)/stm32mp-openstlinux-6.6-yocto-scarthgap-mpu-v25.06.11/sources/ostl-linux/linux-stm32mp-6.6.78-stm32mp-r2-r0/build
KERNEL_SRC_PATH_DESKTOP=/lib/modules/$(shell uname -r)/build
# Optional: path to kernel build output directory (for out-of-tree builds)
OUTPUT_BUILD_DIR ?=
# Destination directory for module installation
DESTDIR ?= $(KERNEL_SRC_PATH)/install_artifact
# Current working directory
PWD := $(shell pwd)

# User-space app build
APP_NAME := capgeminiSound
SRC := App/capgeminiSound.c
BUILD_DIR := build
LDFLAGS := -lasound

# Build kernel module for quemu or native linux
modules_desktop:
	$(MAKE) -C $(KERNEL_SRC_PATH_DESKTOP) $(if $(OUTPUT_BUILD_DIR),O=$(OUTPUT_BUILD_DIR)) M="$(PWD)" modules
# Build kernel module make sure to run this command before running make to use crosscompile ie $ source ~/Developer-Package/SDK/environment-setup-cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi
modules_st:
	$(MAKE) -C $(KERNEL_SRC_PATH) $(if $(OUTPUT_BUILD_DIR),O=$(OUTPUT_BUILD_DIR)) M="$(PWD)" modules

clean:
	rm -rf $(BUILD_DIR)
	$(MAKE) -C $(KERNEL_SRC_PATH) $(if $(OUTPUT_BUILD_DIR),O=$(OUTPUT_BUILD_DIR)) M="$(PWD)" clean
	make -C /lib/modules/$(shell uname -r)/build M="$(PWD)" clean
# Build user-space app (make sure not running on same console instance as the SDK)
app_desktop:
	mkdir -p $(BUILD_DIR)
	gcc -Wall $(SRC) -o $(BUILD_DIR)/$(APP_NAME) $(LDFLAGS)
# Build user-space app for STM32MP1 (requires SDK environment sourced)
app_st:
	mkdir -p $(BUILD_DIR)
	$(CC) -Wall $(SRC) -o $(BUILD_DIR)/$(APP_NAME)_st $(LDFLAGS)


install:
# sudo insmod hello.ko
	$(MAKE) -C $(KERNEL_SRC_PATH) $(if $(OUTPUT_BUILD_DIR),O=$(OUTPUT_BUILD_DIR)) M="$(PWD)" INSTALL_MOD_PATH=$(DESTDIR) modules_install

uninstall:
	sudo rmmod hello