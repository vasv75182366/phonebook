#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include IMPL

#ifdef OPT
#define OUT_FILE "opt.txt"
#endif

#ifdef HASH
#define OUT_FILE "hash.txt"
#endif

#ifdef ORIG
#define OUT_FILE "orig.txt"
#endif

#define DICT_FILE "./dictionary/words.txt"

static double diff_in_second(struct timespec t1, struct timespec t2)
{
    struct timespec diff;
    if (t2.tv_nsec-t1.tv_nsec < 0) {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec - 1;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
    } else {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
    }
    return (diff.tv_sec + diff.tv_nsec / 1000000000.0);
}

int main(int argc, char *argv[])
{
    FILE *fp;
    int i = 0;
    char line[MAX_LAST_NAME_SIZE];
    struct timespec start, end;
    double cpu_time1, cpu_time2;

    /* check file opening */
    fp = fopen(DICT_FILE, "r");
    if (fp == NULL) {
        printf("cannot open the file\n");
        return -1;
    }
#ifndef HASH
    /* build the entry */
    entry *pHead, *e;
    pHead = (entry *) malloc(sizeof(entry));
    e = pHead;
    e->pNext = NULL;
#else
    entry *pHead[HASH_TABLE_SIZE], *e[HASH_TABLE_SIZE];
    for (i = 0; i < HASH_TABLE_SIZE; i++) {
        pHead[i] = malloc(sizeof(entry));
        pHead[i]->pNext = NULL;
        e[i] = pHead[i];
    }
#endif

    printf("size of entry : %lu bytes\n", sizeof(entry));

#if defined(__GNUC__)
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
    clock_gettime(CLOCK_REALTIME, &start);
    while (fgets(line, sizeof(line), fp)) {
        while (line[i] != '\0')
            i++;
        line[i - 1] = '\0';
        i = 0;
#ifndef HASH
        e = append(line, e);
#else
        e[BKDRHash(line)] = append(line,e[BKDRHash(line)]);
#endif
    }
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time1 = diff_in_second(start, end);

    /* close file as soon as possible */
    fclose(fp);

    /* the givn last name to find */
    char input[MAX_LAST_NAME_SIZE] = "zyxel";
#ifndef HASH
    e = pHead;
    assert(findName(input, e) &&
           "Did you implement findName() in " IMPL "?");
    assert(0 == strcmp(findName(input, e)->lastName, "zyxel"));
#else
    e[BKDRHash(input)] = pHead[BKDRHash(input)];
    assert(findName(input, e[BKDRHash(input)]) &&
           "Did you implement findName() in " IMPL "?");
    assert(0 == strcmp(findName(input, e[BKDRHash(input)])->lastName, "zyxel"));
#endif

#if defined(__GNUC__)
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
    /* compute the execution time */
    clock_gettime(CLOCK_REALTIME, &start);
#ifndef HASH
    findName(input, e);
#else
    findName(input, e[BKDRHash(input)]);
#endif
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time2 = diff_in_second(start, end);

    FILE *output = fopen(OUT_FILE, "a");
    fprintf(output, "append() findName() %lf %lf\n", cpu_time1, cpu_time2);
    fclose(output);

    printf("execution time of append() : %lf sec\n", cpu_time1);
    printf("execution time of findName() : %lf sec\n", cpu_time2);
#ifndef HASH
    entry *tmp;
    while (pHead->pNext) {
        tmp = pHead->pNext;
        free(pHead);
        pHead = tmp;
    }
    free(pHead);
#else
    for (i = 0; i < HASH_TABLE_SIZE; i++) {
        entry *tmp;
        while (pHead[i]->pNext) {
            tmp = pHead[i]->pNext;
            free(pHead[i]);
            pHead[i] = tmp;
        }
        free(pHead[i]);
    }
#endif
    return 0;
}
