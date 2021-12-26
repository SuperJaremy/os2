//
// Created by vlad on 25.12.2021.
//

#ifndef OS2_MODULE_MY_PCI_DEV_H
#define OS2_MODULE_MY_PCI_DEV_H

struct in_data{
    int domain;
    unsigned int bus;
    unsigned int devfn;
};

struct my_pci_dev{
    unsigned int	devfn;
    unsigned short	vendor;
    unsigned short	device;
    unsigned short	subsystem_vendor;
    unsigned short	subsystem_device;
    unsigned int	irq;
    unsigned int  class;
};

#endif //OS2_MODULE_MY_PCI_DEV_H
