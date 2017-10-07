#include <stdlib.h>
#include <string.h>
#include "phonebook_opt.h"

/* TODO: FILL YOUR OWN IMPLEMENTATION HERE! */
entry *findName(char lastName[], entry *pHead)
{
    /* TODO: implement */
    while (pHead != NULL) {
        if (strcasecmp(lastName, pHead->lastName) == 0)
            return pHead;
        pHead = pHead->pNext;
    }
    return NULL;
}

entry *append(char lastName[], entry *e)
{
    /* TODO: implement */
    e->pNext = (entry *) malloc(sizeof(entry));
    e = e->pNext;
    strcpy(e->lastName, lastName);
    e->pNext = NULL;

    return e;

}
// BKDR Hash Function
unsigned int BKDRHash(char *str)
{
    unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
    unsigned int hash = 0;
    while (*str) {
        hash = hash * seed + (*str++);
    }
    return (hash & 0x7FFFFFFF) % HASH_TABLE_SIZE;
}
// ELF Hash Function
unsigned int ELFHash(char *str)
{
    unsigned int h = 0, g;

    while(*str) {
        h = (h << 4) + (*str++);
        g = h & 0xf0000000L;
        if(g) {
            h ^= g >> 24;
        }

        h &= ~g;
    }

    return (h & 0x7FFFFFFF) % HASH_TABLE_SIZE;
}
