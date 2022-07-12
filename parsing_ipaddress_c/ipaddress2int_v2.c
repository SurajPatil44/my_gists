// Type your code here, or load an example.

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>

// string_view_t is CoW data structure where we copy only when writing
// reading does not require copy
typedef struct string_view_s {
    char* buffer;
    size_t sz;
} string_view_t;

// again useful for no copy 
typedef struct string_view_pair_s {
    string_view_t first;
    string_view_t second;
} sv_pair;

static string_view_t from_cstring(char* cstring){
    string_view_t sv = {
        .buffer = cstring,
        .sz = strlen(cstring)
    };
    return sv;
}

// atoi for string
// no error checking should be used with error checking
static int _sv_atoi(string_view_t sv){
    size_t sz = sv.sz;
    int num = 0,multi = 1;
    while(sz--){
        num += multi * (int) (sv.buffer[sz] - '0');
        multi *= 10;
    }
    return num;
}

static bool is_num(char c){
    return (c >= '0') && (c <= '9');
}

// trims front and back if char is not a digit
// no copy 
static void trim(string_view_t* sv){
    int i = 0;
    // check from front
    while(!is_num(sv->buffer[i])){
        if(i >= sv->sz) break;
        i++;
    }
    // check from back
    while(sv->sz > i && !isnum(sv->buffer[sv->sz - 1])){
        sv->sz--;
    }
    sv->buffer += i;
    sv->sz = sv->sz - i;
}

// wrapper for c string 
// returns 0 for invalid strings
static int catoi(char* cstring){
    string_view_t sv = from_cstring(cstring);
    trim(&sv);
    return _sv_atoi(sv);
}

static inline sv_pair partition(string_view_t sv,char part){
    string_view_t first = sv;
    string_view_t second;
    sv_pair res;
    int i = 0;
    while(i < sv.sz && sv.buffer[i] != part){
        i++;
    }
    first.sz = i;
    if(i < sv.sz) {
        second.buffer = sv.buffer + i + 1;
        second.sz = sv.sz - i - 1;
    } else {
        second.buffer = NULL;
        second.sz = 0;
    }
    res.first = first;
    res.second = second;
    return res;
}

typedef union ip_address_s {
    uint8_t ip[4];
    uint32_t ip_int;
} ip_address_t;

static int check_val(int num){
    return num < 256 && num > 0;
}

// string_to_ip makes no heap allocation ans should be faster that above version

static uint32_t string_to_ip(char* string){
    string_view_t sv = from_cstring(string);
    ip_address_t ipadd;
    int index = 3,num = 0;
    sv_pair p = partition(sv,'.');
    num = _sv_atoi(p.first);
    if(check_val(num)) ipadd.ip[index] = (uint8_t) num;
    else goto error;
    index -= 1;
    while(p.second.buffer && index > -1){
        p = partition(p.second,'.');
        num = _sv_atoi(p.first);
        if(check_val(num)) ipadd.ip[index] = (uint8_t) num;
        else goto error;
        index -= 1;
    }
    if(index == -1) {
        return ipadd.ip_int;
    }
    error:
        errno = EINVAL;
        return -1;
}

int main(){
    int ip = string_to_ip("192.168.259.51");
    if(ip < 0) {
        printf("error : %s",strerror(errno));
        errno = 0;
    } else {
        printf("ip is = %u \n",ip);
    }
}