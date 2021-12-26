//
// Created by vlad on 24.12.2021.
//

#ifndef OS2_MODULE_MY_NET_DEVICE_H
#define OS2_MODULE_MY_NET_DEVICE_H

struct _my_net_device {
    char name[128];
    unsigned long		mem_end;
    unsigned long		mem_start;
    unsigned long		base_addr;
    unsigned long		state;
    unsigned int		flags;
    unsigned int		priv_flags;
    int			ifindex;
    unsigned short		gflags;
    unsigned int		mtu;
    unsigned int		min_mtu;
    unsigned int		max_mtu;
    unsigned short		type;
    unsigned char		name_assign_type;
    int			group;
    unsigned char		operstate;
    unsigned char		link_mode;

    unsigned char		if_port;
    unsigned char		dma;
    unsigned char		perm_addr[128];
    unsigned char		addr_assign_type;
    unsigned char		addr_len;
    unsigned char		upper_level;
    unsigned char		lower_level;

    unsigned short		neigh_priv_len;
    unsigned short          dev_id;
    unsigned short          dev_port;
    int irq;
};

#endif //OS2_MODULE_MY_NET_DEVICE_H
