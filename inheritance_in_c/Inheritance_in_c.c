#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Animal_t {
    char *name;
    void (*setname) (struct Animal_t*,const char *);
} ;

inline void set_name(struct Animal_t *Base, const char* name){
    Base -> name = (char *)malloc(sizeof(name));
    Base -> name = strdup(name);
}

struct Dog {
    struct Animal_t *Base;
    char *(*getname)(struct Dog *);
};

inline char* get_dog_name(struct Dog* dog){
    return dog->Base->name;
}

int main() {
    struct Animal_t Base;
    struct Dog tommy; 
    tommy.Base = &Base;
    tommy.Base->setname = set_name;
    tommy.getname = get_dog_name;
    tommy.Base->setname(tommy.Base,"tommy");
    printf("%s",tommy.getname(&tommy));
    free(tommy.Base->name);
}