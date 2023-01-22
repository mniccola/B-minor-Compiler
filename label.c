#include "label.h"


int label_count = 0;

int label_create() {
    label_count = label_count + 1;
    return label_count;
}

const char * label_name(int label) {
    char * str = malloc(8);
    sprintf(str, ".L%d", label);

    return str;
}