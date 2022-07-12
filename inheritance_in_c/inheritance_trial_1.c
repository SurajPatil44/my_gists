#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

//Parent struct with get and set method
struct Animal_t {
    char *name;
    void (*setname) (struct Animal_t*,const char *);
    char *(*getname)(struct Animal_t *);
    void (*freefun)(struct Animal_t *);
} ;

void simple_set_name(struct Animal_t* animal,const char* name){
    animal->name = (char *)malloc(sizeof(name));
    animal->name = strdup(name);
}

char* simple_get_name(struct Animal_t* animal){
    return animal->name;
}

enum Dog_type {
    labrador, dalmesian,unknown,
};



//Base struct using inheritance
struct Dog {
    //this is parent stuct object
    struct Animal_t *Base;
    //extra functionality
    bool is_name;
    enum Dog_type dgtyp;
};

enum CatBreed {
    Persian,Bengal,unkbreed
};

struct Cat {
    struct Animal_t *Base;
    //extra functionality
    enum CatBreed catb;
};


void free_cat(struct Animal_t *obj){
    struct Cat *catobj = (struct Cat*) obj;
    free(catobj->Base->name);
    catobj->Base = NULL;
}

void Cat_init(struct Cat *catobj){
    struct Animal_t *Parent = (struct Animal_t *) malloc(sizeof(struct Animal_t));
    catobj->Base = Parent;
    catobj->Base->setname = simple_set_name;
    catobj->Base->getname = simple_get_name;
    catobj->Base->freefun = free_cat;
    catobj->catb = (enum CatBreed) unkbreed;
}

//match function signature as parent function
static void set_dog_name(struct Animal_t *Base, const char* name){
    //change it Base object
    struct Dog *dog_obj = (struct Dog *)Base;
    //Now you can use all the functionality
    dog_obj->dgtyp = (enum Dog_type) unknown;
    dog_obj->Base->name = (char *)malloc(sizeof(name));
    dog_obj->Base->name = strdup(name);
    dog_obj->is_name = true;
}


static char* get_dog_name(struct Animal_t* base){
    struct Dog *dog = (struct Dog*) base;
    return dog->is_name ? dog->Base->name : NULL;
}

void freedog(struct Animal_t *obj){
    struct Dog *dogobj = (struct Dog *)obj; 
    if(dogobj->is_name) free(dogobj->Base->name);
    free(dogobj->Base);
    dogobj->Base = NULL;
}

void Dog_init(struct Dog *dogobj){
    struct Animal_t *Parent = (struct Animal_t *) malloc(sizeof(struct Animal_t));
    dogobj->Base = Parent;
    dogobj->Base->setname = set_dog_name;
    dogobj->Base->getname = get_dog_name;
    dogobj->Base->freefun = freedog;
    dogobj->is_name = false;
    dogobj->dgtyp = unknown;
}

int main() {
    struct Dog tommy; 
    struct Cat tom;
    Dog_init(&tommy);
    Cat_init(&tom);
    tommy.Base->setname((struct Animal_t*)&tommy,"tommy");
    tom.Base->setname(tom.Base,"tom");
    printf("%s %s",tommy.Base->getname((struct Animal_t*)&tommy),tom.Base->getname(tom.Base));
    tommy.Base->freefun((struct Animal_t *)&tommy);
    tom.Base->freefun((struct Animal_t *)&tom);
}