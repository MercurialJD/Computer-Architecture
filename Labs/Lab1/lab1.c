#include <stdio.h>
#include <string.h>

#define PRINTINFO(type) { \
    printf("Size of "#type": %d\n", sizeof(type)); \
}

int main() {
    PRINTINFO(char);
}