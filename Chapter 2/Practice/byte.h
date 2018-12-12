#include <stdio.h>

typedef unsigned char *byte_pointer;
void show_bytes(byte_pointer start, size_t len);

void show_int(int x);
void show_float(float x);
void show_pointer(void *x);
void show_long(long x);
void show_short(short x);
void show_double(double x);
void test_show_bytes(int val);