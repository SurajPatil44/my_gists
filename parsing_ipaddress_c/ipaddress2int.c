// Type your code here, or load an example.
#include<stdlib.h>
#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include<errno.h>

//Union all members are laid out at same position in memory
//u8[4] can be interpretes as 4 bytes integer
typedef union {
    uint8_t addr[4];
    int32_t intaddr;
}ipaddr;

/*
ipaddr str2ip(char *ipstr){
    char ch = ipstr[0];
    int i = 0,prev_dot=0,cntr=0;
    ipaddr ip = {0,0,0,0};
    //unsigned char ips[4] = {0};
    while(1){
        ch = ipstr[i];
        if (ch == '.'){
            int size = (i - prev_dot) + 1;
            char *res = (char *)malloc(size);
            res[size] = '\0';
            for(int j=prev_dot;j<i;j++){
                res[j - prev_dot] = ipstr[j]; 
            }
            ip.addr[3 - cntr] = (unsigned char) atoi(res);
            free(res);
            cntr++;
            prev_dot = i + 1;
        }
        else if(ch == '\0') {
            int size = (i - prev_dot) + 1;   
            char* res = (char *)malloc(size);
            res[size] = '\0';
            for(int j = prev_dot;j<i;j++){
                res[j - prev_dot] = ipstr[j];
            }
            ip.addr[3 - cntr] = (unsigned char)atoi(res);
            free(res);
            break;
        }
        i++;
    }
    //ip.addr = ips;
    //for(int a;a<=3;a++) printf("%d\t",(int)ip.addr[a]);
    return ip;
}
*/



ipaddr str2ip(const char *ipstr) {
    char *ret;
    uint32_t size;
    uint32_t offset = 0,cntr = 0,old_offset;
    ipaddr ip = {0,0,0,0};
    char dot = '.';
    while(1) {
        ret = (char *)memchr(ipstr+offset,dot,strlen(ipstr+offset)); //keep changing the offset for memchr
        if(ret == NULL || cntr > 3) goto postproc;
        old_offset = offset;
        offset = ret - ipstr + 1;
        size = offset - old_offset;
        char *res = (char *) malloc(size);
        strncpy(res,ipstr+old_offset,size-1); //don't have /0 at end of string need to pass length
        ip.addr[3-cntr] = (uint8_t) atoi(res); //big endian case 
        cntr++;
        free(res);
    }
    postproc :
        if(offset == 0 || cntr != 3) {
            errno = EINVAL;
            ip.intaddr = -1;
        } else {
            //printf("cntr %d\n",cntr);
            ip.addr[3-cntr] = (uint8_t) atoi(ipstr+offset);
        }

    return ip; 
}


int main(){
    char *ip = "127.0.0.25";
    ipaddr res = str2ip(ip);
    int errnum;
    //printf("%d",sizeof(unsigned int));
    if(res.intaddr < 0){
        errnum = errno;
        printf("%s\n",strerror(errnum));
    } else {
        printf("%d",res.intaddr);
    }
}