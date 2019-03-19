#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <iostream>
#include <assert.h>


/* The L1 cache on this machine is 8-way associative, 64B per line (16 ints) split over 64 sets. */

void co_converter (int *G, int dim){
    clock_t t;
    t = clock();

	int i,j;
	for (i =0; i < dim; i++)
		for (j = 0; j < dim; j++){
			G[j*dim + i] = G[j*dim + i] || G[i*dim + j];

        }
    t = clock() - t;
    printf ("It took me %ld clicks (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);
}



void co_converter_1 (int *G, int dim){
    clock_t ttt;
    ttt = clock();

	int i,j, t,s;
    int k =0; 
    int block_s = 8;
    int limit = dim - block_s;
    int res_c, res_r;

 

    for (t=0; t < limit; t+=block_s){

    
        for(s=0; s< limit; s+=block_s){
        k=0;
	        for (i = t  ; i < (t + block_s) ; i++){
		        for (j = s ; j < (s + block_s); j++){
                    res_c = G[(j + k)*dim + i] || G[(i) *dim + (j)];
                    G[(i) *dim + (j + k)] = res_c;   
			        G[(j+k)*dim + i] = res_c;                 
                }
            k++;
            }
        }
    }
    for (i = t; i < dim; i++)
        for (k = 0; k < s; k += block_s)
            for (int j = k; j < k + block_s; j++){
                
                res_c = G[(j)*dim + i + k] || G[(i) *dim + (j)];
                G[(j)*dim + i] = res_c;
                G[(i) *dim + (j)] = res_c;
            }
     k=0;
    for (i = 0; i < dim; i++){
        for (j = t; j < dim; j++){
            
            res_c = G[(j)*dim + i + k] || G[(i) *dim + (j)];
                G[(j)*dim + i] = res_c;
                G[(i) *dim + (j)] = res_c;
        }
    k++;
    }
    ttt = clock() - ttt;
    printf ("It took me %ld clicks (%f seconds).\n",ttt,((float)t)/CLOCKS_PER_SEC);
}




int main (){
    int *a = (int *)malloc(10000 * 10000 * sizeof(int)); 
    int *b = (int *)malloc(10000 *10000 * sizeof(int)); 
    int *c = (int *)malloc(10000 * 10000 * sizeof(int)); 

    std::fill_n(a, (10000*10000), 3);
    std::fill_n(b, (10000*10000), 3);
    std::fill_n(c, (10000*10000), 3);
    
    co_converter(a,10000);
    co_converter_1(b,10000);



     for (int i = 0; i < 1000000; i++)
      assert(a[i] == b[i]);

  /*
for (int i = 0; i < 100; i++)
   for (int j = 0; j < 100; j++)
        std::cout << c[j*100 + i]; 


  for (int i = 0; i < 10000; i++)
      std::cout << b[i];
*/
}