//
// Created by vlad on 24.12.2021.
//
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "_my_net_device.h"
#include "_my_pci_dev_.h"

#define MAX_BUFFER_LENGTH 256
static const char* path_to_net_device = "/proc/os2_module/os2_mod_net";
static const char* path_to_pci_dev = "/proc/os2_module/os2_mod_pci";

void retrieve_net_device(){
    char name[MAX_BUFFER_LENGTH];
    struct _my_net_device net_dev = {};
    printf("What is the name of the net_device? ");
    scanf("%s", name);
    FILE *in = fopen(path_to_net_device, "r+");
    if(!in) {
        printf("Error in module\n");
        return;
    }
    if(fprintf(in, "%s", name) < 0){
        printf("Error im module\n");
        fclose(in);
        return;
    }
    if(!fread(&net_dev, sizeof (struct _my_net_device), 1, in)){
        printf("Device not found\n");
        fclose(in);
        return;
    }
    fclose(in);
    printf("Something found!!\n");
    print_my_net_device(net_dev);
}

bool extract_domain_bus_slot_from_path(char* path, int* domain, unsigned int* bus, unsigned int* devfn){
    int _domain = 0;
    unsigned int _bus;
    unsigned int _devfn;
    size_t slash_cnt = 0;
    const char* to_extract = "/proc/bus/pci/";
    char _path[MAX_BUFFER_LENGTH];

    if(strstr(path, to_extract))
        strcpy(_path, &path[strlen(to_extract)]);
    else
        strcpy(_path, path);
    for(size_t i = 0; _path[i]!=0; i++){
        if(_path[i]=='/')
            slash_cnt++;
    }
    if(slash_cnt > 2 || slash_cnt == 0)
        return false;
    char endptrch = '\0';
    char *endptr = &endptrch;
    char* token = strtok(_path, "/");
    if(slash_cnt>1) {
        _domain = (int)strtol(token, &endptr, 16);
        if(endptrch != '\0') {
            printf("wrong domain");
            return false;
        }
        token = strtok(NULL, "/");
    }
    _bus = (int)strtol(token, &endptr, 16);
    if(endptrch != '\0') {
        printf("wrong bus");
        return false;
    }
    token = strtok(NULL, ".");
    _devfn = (int)strtol(token, &endptr, 16);
    if(endptrch != '\0') {
        printf("wrong device");
        return false;
    }
    _devfn *= 8;
    token = strtok(NULL, ".");
    _devfn += (int)strtol(token, &endptr, 16);
    if(endptrch != '\0') {
        printf("wrong function");
        return false;
    }
    *domain = _domain;
    *bus = _bus;
    *devfn = _devfn;
    return true;
}

void retrieve_pci_dev(){
    char path[MAX_BUFFER_LENGTH];
    struct my_pci_dev pci_dev = {};
    printf("What is the path to pci_dev (/proc/bus/pci/...) ");
    scanf("%s", path);
    FILE *in = fopen(path_to_pci_dev, "r+");
    if(!in){
        printf("Error in module\n");
        return;
    }
    int domain = 0;
    unsigned int bus = 0;
    unsigned int devfn = 0;
    if(!extract_domain_bus_slot_from_path(path, &domain, &bus, &devfn)){
        printf("Error while reading\n");
        fclose(in);
        return;
    }
    struct in_data input = {.domain = domain, .bus = bus, .devfn = devfn};
    if(!fwrite(&input, sizeof(struct in_data), 1, in)){
        printf("Error in  module\n");
        fclose(in);
        return;
    }
    if(!fread(&pci_dev, sizeof(struct my_pci_dev), 1, in)){
        printf("Device not found\n");
        fclose(in);
        return;
    }
    fclose(in);
    printf("Something found!!\n");
    print_my_pci_dev(pci_dev);
}

int main(){
    char flag;
    printf("What do you want to retrieve: net_device or pci_dev [n/p] ");
    scanf("%c", &flag);
    if(flag == 'n'){
        retrieve_net_device();
    }
    else if(flag == 'p'){
        retrieve_pci_dev();
    }
    else{
        printf("Error while reading. Please, try again");
        return 1;
    }
    return 0;
}