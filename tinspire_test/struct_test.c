#include <os.h>

int f(void) {
    return 42;
}

typedef struct type1_t {
    const char *name;
    const char *desc;
    int (*fp)(void);
} type1_t;

type1_t DUMMY_bootstrap = {"foobar", "foobar bootstrap", f};

extern type1_t DUMMY_bootstrap;

static type1_t *bootstrap[] = {
    &DUMMY_bootstrap,
    NULL
};

typedef struct lol_t {
    int wut;
    int wat;
} lol_t;

typedef struct derp_t {
    lol_t *a[256];
    int herp;
} derp_t;

void init_derp(derp_t *derp) {
    int i;
    derp->herp = 42;
    for(i = 0; i < 256; ++i) {
        lol_t *foo = malloc(sizeof *foo);
        foo->wat = 300 - i;
        foo->wut = 24;
        derp->a[i] = foo;
    }
}

int main(void) {
    derp_t derp;
    init_derp(&derp);
    printf("[0]: %d\n", derp.a[1]->wat);
    printf("herp: %d\n", derp.herp);
#if 0
    nl_relocdata((unsigned int *)bootstrap, 1);
    nl_relocdata((unsigned int *)&bootstrap[0]->name, 1);
    nl_relocdata((unsigned int *)&bootstrap[0]->desc, 1);
    nl_relocdata((unsigned int *)&bootstrap[0]->fp, 1);
    nl_relocdata((unsigned int *)c, 2);
    printf("bootstrap[0]->name: %s\n", bootstrap[0]->name);
    printf("bootstrap[0]->desc: %s\n", bootstrap[0]->desc);
    printf("bootstrap[0]->fp(): %d\n", bootstrap[0]->fp());
#endif
    return 0;
}