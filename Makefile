# language: Makefile / file: Makefile / runtime: GNU Make / target: Linux kernel external module
obj-m += cassr_hook.o

KDIR ?= $(CURDIR)/kernel-src
ARCH ?= arm64
LLVM ?= 1

.PHONY: all clean

all:
	$(MAKE) -C $(KDIR) M=$(CURDIR) ARCH=$(ARCH) LLVM=$(LLVM) modules

clean:
	$(MAKE) -C $(KDIR) M=$(CURDIR) ARCH=$(ARCH) clean
