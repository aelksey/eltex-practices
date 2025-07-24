#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

uint32_t ip_to_uint32(const char *ip){
    uint32_t val = 0;
    uint8_t octet;
    char *ip_copy = strdup(ip);
    char *token = strtok(ip_copy,".");
    for(int i = 0; i < 4; ++i){
        if(token != NULL){
            octet = (uint8_t)atoi(token);
            val = (val << 8) | octet;
            token = strtok(NULL,".");
        }
    }
    free(ip_copy);
    return val;
}

int is_in_subnet(uint32_t ip, uint32_t gw, uint32_t mask){
    return ip & mask == gw & mask;
}

int main(int argc,char *argv[]){
    uint32_t random_ip;
    uint32_t gateway;
    uint32_t mask;
    int own_subnet_count = 0;
    int oth_subnet_count = 0;

    if(argc > 2){
        char* gw_ip = strdup(argv[1]);
        char* mask_ip = strdup(argv[2]);
        int n = atoi(argv[3]);
        gateway = ip_to_uint32(gw_ip);
        mask = ip_to_uint32(mask_ip);
        
        srand(time(NULL));

        uint32_t min = 0;
        uint32_t max = UINT32_MAX;

        for (int i = 0; i < n; i++){
            random_ip = (rand() % (max - min + 1) + min);
            if(is_in_subnet(random_ip,gateway,mask)){
                own_subnet_count++;
            }
        }

        oth_subnet_count = n - own_subnet_count;

        double own_percent = (own_subnet_count / n) * 100;
        double oth_percent = (oth_subnet_count / n) * 100;

        printf("gw-subnet: %d %.2lf%% oth-subnet: %d %.2lf%%",own_subnet_count,own_percent,oth_subnet_count,oth_percent);
        
        return EXIT_SUCCESS;

    }else{
        fprintf(stderr,"Usage: ipapp <gw> <mask> <n>");
        return EXIT_FAILURE;
    }
}