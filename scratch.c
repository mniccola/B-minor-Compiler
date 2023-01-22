
#include "scratch.h"

struct scratch *scratch_top = NULL;

struct scratch *scratch_create(int reg, char *name, bool using, struct scratch *next) {

    struct scratch * scratch_table = malloc(sizeof(struct scratch));
    scratch_table->reg = reg;
    scratch_table->name = name;
    scratch_table->using = using;
    scratch_table->next = next;

    return scratch_table;
}

void scratch_init() {
    struct scratch * table = scratch_create(0, "%rbx", 0, NULL);
    struct scratch * last_reg = table;
    struct scratch * curr_reg;
    int counter = 10;
    char * reg_names[] = {"%r10", "%r11", "%r12", "%r13", "%r14", "%r15"};
    char * str = malloc(8);
    while(counter < 16) {
        curr_reg = scratch_create(counter, reg_names[counter - 10], false,  NULL);
        last_reg->next = curr_reg;
        last_reg = curr_reg;
        counter++;
        strcpy(str, "");
    }

    scratch_top = table;
}


int scratch_alloc() {
    struct scratch * curr_reg = scratch_top;
    bool found = false;
    int found_reg = -1;
    while(curr_reg) {
        if(!curr_reg->using) {
            curr_reg->using = true;
            found_reg = curr_reg->reg;
            found = true;
            break;
        }

        curr_reg = curr_reg->next;
    }

    if(!found) {
        printf("codegen error: out of registers\n");
        exit(1);
    }

    return found_reg;
}

void scratch_display_regs() {
    struct scratch * curr = scratch_top;
    while(curr) {
        printf("reg name: %s  reg num: %d  reg used %d\n", curr->name, curr->reg, curr->using);
        curr = curr->next;
    }
}

void scratch_free(int r) {
    struct scratch * curr_reg = scratch_top;

    while(curr_reg) {
        if(curr_reg->reg == r) {
            curr_reg->using = false;
        }

        curr_reg = curr_reg->next;
    }
}

const char * scratch_name(int r) {
    struct scratch * curr_reg = scratch_top;
    // char * str = malloc(8);
    while(curr_reg) {
        if(curr_reg->reg == r) {
            // sprintf(str, "%%%s", curr_reg->name);
            return curr_reg->name;
        }

        curr_reg = curr_reg->next;
    }

    printf("codegen error: r%d is not a scratch register\n", r);
    exit(1);
}