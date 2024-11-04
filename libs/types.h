#ifndef TYPES_H
#define TYPES_H

int citoa(int num, int base, char **ans);
int cutoa(unsigned int num, int base, char **ans);
int catoi(char const *str, int base, int *ans);

int char_to_int(int *ans, char c);
int int_to_char(char *ans, int num);

#endif