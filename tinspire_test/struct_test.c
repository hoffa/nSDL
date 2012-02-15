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

int main(void) {
    nl_relocdata((unsigned int *)bootstrap, 1);
    nl_relocdata((unsigned int *)&bootstrap[0]->name, 1);
    nl_relocdata((unsigned int *)&bootstrap[0]->desc, 1);
    nl_relocdata((unsigned int *)&bootstrap[0]->fp, 1);
    printf("bootstrap[0]->name: %s\n", bootstrap[0]->name);
    printf("bootstrap[0]->desc: %s\n", bootstrap[0]->desc);
    printf("bootstrap[0]->fp(): %d\n", bootstrap[0]->fp());
    return 0;
}