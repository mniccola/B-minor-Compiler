#ifndef SCRATCH_H
#define SCRATCH_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

struct scratch {
    int reg;
    const char * name;
    bool using;
    struct scratch * next;
};



struct scratch *scratch_create(int reg, char *name, bool using, struct scratch *next);
void scratch_init();
int scratch_alloc();
void scratch_free(int r);
const char * scratch_name(int r);
void scratch_display_regs();



#endif