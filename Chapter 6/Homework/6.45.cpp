#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <iostream>
#include <assert.h>


/* The L1 cache on this machine is 8-way associative, 64B per line (16 ints) split over 64 sets. */

void transpose (int *dst, int *src, int dim){
    clock_t t;
    t = clock();

	int i,j;
	for (i =0; i <dim; i++)
		for (j = 0; j < dim; j++)
			dst[j*dim + i] = src[i*dim + j];
              t = clock() - t;
  printf ("It took me %ld clicks (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);
}



void transpose_4x1 (int *dst, int *src, int dim){
    clock_t t;
    t = clock();
	int i,j;
	int length = dim -3; 
	for (i =0; i <dim; i++){
		for (j = 0; j < length; j+=4){
			dst[j*dim + i] = src[i*dim + j];
			dst[(j + 1)*dim + i] = src[i*dim + (j + 1)];
			dst[(j + 2)*dim + i] = src[i*dim + (j + 2)];
			dst[(j + 3)*dim + i] = src[i*dim + (j + 3)];
		}
		for (; j < dim; j++)
			dst[j*dim + i] = src[i*dim + j];
	}
      t = clock() - t;
  printf ("It took me %ld clicks (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);
}

void transpose_c (int *dst, int *src, int dim){
    clock_t t;
    t = clock();

	int i,j,k,h;
    int block_s = 16;
    int block_n = dim/block_s;
    int limit = dim - block_s;

    for(h=0; h <= limit ;h+=block_s)
        for (k=0; k <= limit ; k+=block_s)
	        for (i = h ; i < h + block_s; i++)
		        for (j= k; j < k + block_s; j++)
			        dst[j*dim + i] = src[i*dim + j];

     for (i = h; i < dim; i++)
        for (k = 0; k < h; k += block_s)
            for (int j = k; j < k + block_s; j++)
                dst[j * dim + i] = src[i * dim + j];

  for (i = 0; i < dim; i++)
    for (j = h; j < dim; j++)
      dst[j * dim + i] = src[i * dim + j];
        t = clock() - t;
  printf ("It took me %ld clicks (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);
}






void transpose_a (int *dst, int *src, int dim){
/* memory aligned transpose */
    clock_t t;
    t = clock();

    
	int i,j,k =0;
    int cnt = 0;
	int length = dim -3; 
    char* dst_b = (char *)dst;
    char* src_b = (char *)src;

    while((size_t)dst % sizeof(int)!=0){
        dst_b[i] = src_b[i];
        i++;
        cnt++;
    }

    src_b +=cnt;
    dst_b +=cnt;

    dst = (int *) dst_b;
    src = (int *) src_b;
    dim--;

	for (i =0; i <dim; i++){
		for (j = 0; j < length; j+=4){
			dst[j*dim + i] = src[i*dim + j];
			dst[(j + 1)*dim + i] = src[i*dim + (j + 1)];
			dst[(j + 2)*dim + i] = src[i*dim + (j + 2)];
			dst[(j + 3)*dim + i] = src[i*dim + (j + 3)];
		}
		for (; j < dim; j++)
			dst[j*dim + i] = src[i*dim + j];
        
    
        dst_b = (char *) (dst + j*dim + i);
        src_b = (char *) (src + i*dim + j);
        int cnt_rem = 4 - cnt;

        for (k = 0; k <=cnt_rem;k++ )
            dst_b[k] = src_b[k];
        
	}

  t = clock() - t;
  printf ("It took me %ld clicks (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);
}


void transpose_4x1c (int *dst, int *src, int dim){
    clock_t t;
    t = clock();

	int i,j,k,h;
    int block_s = 16;
    int block_n = dim/block_s;
    int limit = dim - block_s;

    for(h=0; h <= limit ;h+=block_s)
        for (k=0; k <= limit ; k+=block_s  )
	        for (i = h ; i < h + block_s; i++)
		        for (j= k; j < k + block_s -3 ; j+=4){
			        dst[j*dim + i] = src[i*dim + j];
                    dst[(j + 1) *dim + i] = src[i*dim + j + 1];
                    dst[(j + 2)*dim + i] = src[i*dim + j + 2];
                    dst[(j + 3)*dim + i] = src[i*dim + j + 3];
                }
                
        

     for (i = h; i < dim; i++)
        for (k = 0; k < h; k += block_s)
            for (int j = k; j < k + block_s; j++)
                dst[j * dim + i] = src[i * dim + j];

  for (i = 0; i < dim; i++)
    for (j = h; j < dim; j++)
      dst[j * dim + i] = src[i * dim + j];
        t = clock() - t;
  printf ("It took me %ld clicks (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);
}


int main (){
    int *a = (int *)malloc(512 * 512 * sizeof(int)); 
    int *b = (int *)malloc(512 * 512 * sizeof(int)); 

    std::fill_n(a, (512*512), -3);
    std::fill_n(b, (512*512), 3);


    



    transpose(a,b, 512);
    transpose_4x1(a,b, 512);
    transpose_c(a,b, 512);
    transpose_a(a,b, 512);
    transpose_4x1c(a,b, 512);

    
  /*for (int i = 0; i < 1024; i++)
    for (int j = 0; j < 1024; j++)
      assert(a[i * 1023 + j] == b[j * 1023 + i]);
*/
    free((void*) a);
    free((void*) b); 

}