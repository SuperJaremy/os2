//
// Created by vlad on 25.12.2021.
//

#ifndef OS2_MODULE__MY_PCI_DEV__H
#define OS2_MODULE__MY_PCI_DEV__H
#include "my_pci_dev.h"
#include <stdio.h>

static inline void print_my_pci_dev(struct my_pci_dev dev){
    printf("devfn: %x\n", dev.devfn);
    printf("vendor: %x\n", dev.vendor);
    printf("device: %x\n", dev.device);
    printf("subsystem_vendor: %x\n", dev.subsystem_vendor);
    printf("subsystem_device: %x\n", dev.subsystem_device);
    printf("irq: %u\n", dev.irq);
    printf("class: %x\n", dev.class);
}

#endif //OS2_MODULE__MY_PCI_DEV__H
