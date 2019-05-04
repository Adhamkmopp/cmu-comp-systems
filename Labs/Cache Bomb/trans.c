/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}


char trans_block_desc[] = "Simple block transpose";
void trans_block(int M, int N, int A[N][M], int B[M][N]){

    int j, k, kk, jj;
    int row_b, col_b, bsize, col_en, row_en;
    
    if (M ==64){
        bsize = 4;
        row_b =col_b = bsize; 
        row_en = col_en = bsize * (M/bsize); /* Amount that fits evenly into blocks */
    } else if (M==32){
        bsize = 8;
        row_b =col_b = bsize; 
        row_en = col_en = bsize * (M/bsize); /* Amount that fits evenly into blocks */
    } else {
        bsize = 8;
        row_b =col_b = bsize; 
        row_en = col_en = bsize * (M/bsize); /* Amount that fits evenly into blocks */

    }
        

    for (kk = 0; kk < col_en; kk += col_b) {
        for (jj = 0; jj < row_en; jj += row_b) {
            for (j = jj; j < jj + row_b; j++) {
                for (k = kk; k < kk + col_b; k++) {
                    
                    B[k][j] = A[j][k];
                        
                }                   
            }           
        } 
    }

    for (j = 0; j < N; j++) {
        for (k = col_en; k < M; k++) {
            B[k][j] = A[j][k];
                        
        }                   
    }

    for (j = row_en; j < N; j++) {
        for (k = 0; k < M; k++) {
            B[k][j] = A[j][k];
                        
        }                   
    }

}

char trans_block_desc_plus[] = "Simple block transpose plus";
void trans_block_plus(int M, int N, int A[N][M], int B[M][N]){
    int j, k, kk, jj, i;
    int row_b, col_b, bsize, col_en, row_en;
    
    if (M ==64){
        bsize = 4;
        row_b =col_b = bsize; 
        row_en = col_en = bsize * (M/bsize); /* Amount that fits evenly into blocks */
    } else if (M==32){
        bsize = 8;
        row_b =col_b = bsize; 
        row_en = col_en = bsize * (M/bsize); /* Amount that fits evenly into blocks */
    } 
        

    for (kk = 0; kk < col_en; kk += col_b) {
        for (jj = 0; jj < row_en; jj += row_b) {
            i=0;
            for (k = kk; k < kk + row_b; k++) {
                for (j = jj + i; j < jj + col_b; j++) {
                    
                    B[j][k] = A[k][j];
                        
                }
                i++;                 
            }           
        } 
    }

    for (kk = 0; kk < col_en; kk += col_b) {
        for (jj = 0; jj < row_en; jj += row_b) {
            i=0;
            for (k = kk; k < kk + row_b; k++) {
                for (j = jj; j < jj + i; j++) {
                    
                    B[j][k] = A[k][j];
                        
                }
                i++;                 
            }           
        } 
    }


    for (j = 0; j < N; j++) {
        for (k = col_en; k < M; k++) {
            B[k][j] = A[j][k];
                        
        }                   
    }

    for (j = row_en; j < N; j++) {
        for (k = 0; k < M; k++) {
            B[k][j] = A[j][k];
                        
        }                   
    }

}

   


/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 
    registerTransFunction(trans_block, trans_block_desc);

    registerTransFunction(trans_block_plus, trans_block_desc_plus); 


}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

