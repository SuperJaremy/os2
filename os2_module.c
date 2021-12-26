#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/proc_fs.h>
#include <linux/uaccess.h>

#include <linux/net_namespace.h>
#include <linux/netdevice.h>
#include <linux/pci.h>

#include<stdbool.h>
#include "my_net_device.h"
#include "my_pci_dev.h"

#define BUFFER_MAX_CAPACITY 128

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("OS2 Module");
MODULE_VERSION("2.71828");


static struct proc_dir_entry *also_os2_mod_root;
static struct proc_dir_entry *also_some_net_device;
static struct proc_dir_entry *also_some_pci_device;

static struct net *maybe_net;
static char my_dev_name[BUFFER_MAX_CAPACITY];
static struct net_device *some_dev;
static struct _my_net_device net_dev ={0};
static struct pci_dev *another_dev;
static bool net_found = false;
static struct in_data in_pci = {0};
static struct my_pci_dev _pci_dev_ = {0};
static bool pci_found = false;

static bool _get_net_device(const char *name) {
    printk(KERN_INFO "Searching for device by name: %s", name);
    maybe_net = &init_net;
    if (!maybe_net) {
        printk(KERN_INFO
        "No net!\n");
        return false;
    }
    if (!dev_valid_name(name)) {
        printk(KERN_INFO
        "Invalid net_device name");
        return false;
    }
    some_dev = dev_get_by_name(maybe_net, name);
    if (!some_dev) {
        printk(KERN_INFO
        "Nothing found ((");
        return false;
    }
    printk(KERN_INFO
    "Actually retrieved something!");
    return true;
}

static ssize_t procfile_read_net_dev(struct file *filePointer, char __user *buffer, size_t buffer_length, loff_t *offset){
    size_t i = 0;
    if(!net_found){
    printk(KERN_INFO "Device wasn't found");
    return -EFAULT;
    }
    if(buffer_length >= sizeof (struct _my_net_device)){
        if(copy_to_user(buffer, &net_dev, sizeof(struct _my_net_device))){
            printk(KERN_INFO "Could not copy");
            return -EFAULT;
        }
        printk(KERN_INFO "Struct net_device copied");
        for(i = 0; net_dev.name[i]!=0; i++)
            net_dev.name[i] = 0;
        for(i = 0; net_dev.perm_addr[i]!=0; i++)
            net_dev.perm_addr[i] = 0;
        net_found = false;
        return sizeof(struct _my_net_device);
    }
    else{
        printk(KERN_INFO "Not enough space");
        return -EFAULT;
    }
}



static void net_dev_to_my_net_dev(void){
    size_t i = 0;
    for(i = 0; some_dev->name[i]!=0; i++)
        net_dev.name[i] = some_dev->name[i];
#define COPY_FIELD(FIELD) net_dev.FIELD = some_dev->FIELD
    COPY_FIELD(mem_end);
    COPY_FIELD(mem_start);
    COPY_FIELD(base_addr);
    COPY_FIELD(state);
    COPY_FIELD(flags);
    COPY_FIELD(priv_flags);
    COPY_FIELD(ifindex);
    COPY_FIELD(gflags);
    COPY_FIELD(mtu);
    COPY_FIELD(min_mtu);
    COPY_FIELD(max_mtu);
    COPY_FIELD(type);
    COPY_FIELD(name_assign_type);
    COPY_FIELD(group);
    COPY_FIELD(operstate);
    COPY_FIELD(link_mode);
    COPY_FIELD(link_mode);
    COPY_FIELD(if_port);
    COPY_FIELD(dma);
    for(i = 0; some_dev->perm_addr[i]!=0; i++)
        net_dev.perm_addr[i] = some_dev->perm_addr[i];
    COPY_FIELD(addr_assign_type);
    COPY_FIELD(addr_len);
    COPY_FIELD(upper_level);
    COPY_FIELD(lower_level);
    COPY_FIELD(neigh_priv_len);
    COPY_FIELD(dev_id);
    COPY_FIELD(dev_port);
    COPY_FIELD(irq);
#undef COPY_FIELD
}

static ssize_t procfile_write_net_dev(struct file *file, const char __user *buff, size_t len, loff_t *off){
    if(len > BUFFER_MAX_CAPACITY){
        printk(KERN_INFO "Too long device name");
        return -EFAULT;
    }
    if(copy_from_user(my_dev_name, buff, len)){
        printk(KERN_INFO "Couldn't copy name from userspace");
        return -EFAULT;
    }
    if(my_dev_name[len-1] == '\n')
        my_dev_name[len - 1] = '\0';
    else if(my_dev_name[len-1]!='\0')
        my_dev_name[len] = '\0';
    if(!_get_net_device(my_dev_name)){
        printk(KERN_INFO "Couldn't retrieve device");
        return -EFAULT;
    }
    printk(KERN_INFO "Retrieved device %s", my_dev_name);
    net_dev_to_my_net_dev();
    dev_put(some_dev);
    some_dev = NULL;
    net_found = true;
    return  len;
}

static const struct proc_ops net_file_ops = {
    .proc_read = procfile_read_net_dev,
    .proc_write = procfile_write_net_dev,
};

void pci_dev_to_my_pci_dev(void){
#define COPY_FIELD(FIELD) _pci_dev_.FIELD = another_dev->FIELD
    COPY_FIELD(devfn);
    COPY_FIELD(vendor);
    COPY_FIELD(device);
    COPY_FIELD(subsystem_vendor);
    COPY_FIELD(subsystem_device);
    COPY_FIELD(irq);
    COPY_FIELD(class);
#undef COPY_FIELD
}

static ssize_t procfile_write_pci_dev(struct file *file, const char __user *buff, size_t len, loff_t *off){
    if(len < sizeof(struct in_data)){
        printk(KERN_INFO "Invalid input data");
        return -EFAULT;
    }
    if(copy_from_user(&in_pci, buff, len)){
        printk(KERN_INFO "Error while copying in_data");
        return -EFAULT;
    }
    another_dev = pci_get_domain_bus_and_slot(in_pci.domain, in_pci.bus, in_pci.devfn);
    if(!another_dev){
        printk(KERN_INFO "Device with domain: %d, bus: %u, devfn:%u wasn't found", in_pci.domain, in_pci.bus,
               in_pci.devfn);
        return -EFAULT;
    }
    pci_dev_to_my_pci_dev();
    pci_found = true;
    pci_dev_put(another_dev);
    return len;
}

static ssize_t procfile_read_pci_dev(struct file *filePointer, char __user *buffer, size_t buffer_length, loff_t *offset){
    if(!pci_found){
        printk(KERN_INFO "Pci_dev wasn't found");
        return -EFAULT;
    }
    if(buffer_length >= sizeof(struct my_pci_dev)){
        if(copy_to_user(buffer, &_pci_dev_, sizeof(struct my_pci_dev))){
            printk(KERN_INFO "Error while copying to pci to user");
            return -EFAULT;
        }
        pci_found = false;
        return sizeof(struct my_pci_dev);
    }
    else{
        printk(KERN_INFO "Not enough space in buffer");
        return -EFAULT;
    }
}

static const struct proc_ops pci_file_ops = {
        .proc_read = procfile_read_pci_dev,
        .proc_write = procfile_write_pci_dev,
};

static int __init os2_mod_init(void) {
    printk(KERN_INFO
    "Hello, World!\n");
    also_os2_mod_root = proc_mkdir("os2_module", NULL);
    if (!also_os2_mod_root) {
        remove_proc_entry("os2_module", NULL);
        printk(KERN_INFO
        "Could not create root folder");
        return -ENOMEM;
    }

    also_some_net_device = proc_create("os2_mod_net", 0666, also_os2_mod_root, &net_file_ops);
    if (!also_some_net_device) {
        remove_proc_entry("os2_mod_net", also_os2_mod_root);
        printk(KERN_INFO
        "Could not create file for net_device");
        return -ENOMEM;
    }

    also_some_pci_device = proc_create("os2_mod_pci", 0666, also_os2_mod_root, &pci_file_ops);
    if(!also_some_net_device){
        remove_proc_entry("os2_mod_pci", also_os2_mod_root);
        printk(KERN_INFO "Couldn't create file for pci_dev");
        return -ENOMEM;
    }
    return 0;
}

static void __exit os2_mod_exit(void) {
    proc_remove(also_os2_mod_root);
    printk(KERN_INFO
    "Goodbye, World!\n");
}

module_init(os2_mod_init);
module_exit(os2_mod_exit);