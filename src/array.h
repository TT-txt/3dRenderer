#ifndef ARRAY_H
#define ARRAY_H

#define arrayPush(array, value)                                              \
    do {                                                                      \
        (array) = arrayHold((array), 1, sizeof(*(array)));                   \
        (array)[arrayLength(array) - 1] = (value);                           \
    } while (0);

void* arrayHold(void* array, int count, int item_size);
int arrayLength(void* array);
void arrayFree(void* array);

#endif
