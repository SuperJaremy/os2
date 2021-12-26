//
// Created by vlad on 24.12.2021.
//

#include "_my_net_device.h"

void print_my_net_device(struct _my_net_device dev){
    printf("name: %s\n", dev.name);
    printf("mem_end: %lu\n", dev.mem_end);
    printf("mem_start: %lu\n", dev.mem_start);
    printf("base_addr: %lu\n", dev.base_addr);
    printf("state: %lu\n", dev.state);
    printf("flags: 0x%x\n", dev.flags);
    printf("priv_flags: 0x%x\n", dev.priv_flags);
    printf("ifindex: %d\n", dev.ifindex);
    printf("gflags: 0x%x\n", dev.gflags);
    printf("mtu: %u\n", dev.mtu);
    printf("min_mtu: %u\n", dev.min_mtu);
    printf("max_mtu: %u\n", dev.max_mtu);
    printf("type: %u\n", dev.type);
    printf("name_assign_type: %u\n", dev.name_assign_type);
    printf("group: %d\n", dev.group);
    printf("operstate: %u\n", dev.operstate);
    printf("link_mode: %u\n", dev.link_mode);
    printf("if_port: %u\n", dev.if_port);
    printf("dma: %u\n", dev.dma);
    printf("perm_addr: %s\n", dev.perm_addr);
    printf("addr_assign_type: %u\n", dev.addr_assign_type);
    printf("addr_len: %u\n", dev.addr_len);
    printf("upper_level: %u\n", dev.upper_level);
    printf("lower_level: %u\n", dev.lower_level);
    printf("neigh_priv_len: %u\n", dev.neigh_priv_len);
    printf("dev_id: %u\n", dev.dev_id);
    printf("dev_port: %u\n", dev.dev_port);
    printf("irq: %d\n", dev.irq);
}
