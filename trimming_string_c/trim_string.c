// taken from https://github.com/tsoding/sv
#include <string.h>
#include <stdio.h>

typedef struct sv {
    int size;
    const char* data;
} sv;

static sv chopleft(sv target,const char delim){
    int i = 0;
    sv result;
    while(*(target.data+i) == delim){
        i++;
    }
    result.size =  target.size - i;
    result.data = target.data + i;
    return result;
}

static sv chopright(sv target,const char delim){
    int sz = target.size - 1; // for cstring only not needed for simple sv
    sv result;
    while(*(target.data+sz) == delim){
        sz--;
    }
    result.size = sz + 1; // for cstring only
    result.data = target.data;
    return result;
}

static sv trim(sv target,const char delim){
    return chopright(chopleft(target,delim),delim);
}

int main(){
    char* target = "  hello  ";
    sv send = {.size = strlen(target),.data = target};
    sv result = /* chopright(chopleft(send,' '),' ')*/ trim(send,' ');
    printf("%.*s\n",result.size,result.data);
}