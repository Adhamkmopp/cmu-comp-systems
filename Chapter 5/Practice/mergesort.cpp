
void merge (long src1[], long src2[], long dest[], long n ){
    long i1 = 0;
    long i2 = 0;
    long id = 0;
    long tmp1;
    long tmp2;
    while (i1 < n && i2 < n){

        dest[id++] = src1[i1] < src2[i2] ? tmp1 : tmp2;
    }

    while (i1 < n)
        dest[id++] = src1[i1++];
    while (i2 < n)
        dest[id++] = src2[i2++];
}