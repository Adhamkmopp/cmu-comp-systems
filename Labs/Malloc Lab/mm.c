/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "ateam",
    /* First member's full name */
    "Harry Bovik",
    /* First member's email address */
    "bovik@cs.cmu.edu",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};


/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8
#define WSIZE 4
#define DSIZE 8
//#define DEBUG
/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)
#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define MAX(x,y)    ((x)>(y)?(x):(y))

#define PACK(size,alloc)    ((size) | (alloc))

#define GET(p)  (*(unsigned int *)(p))
#define GET_ADDRESS(p)  (unsigned long int *) (*(unsigned long int *)(p))

#define PUT(p,val)  (*(unsigned int *)(p) = (val))
#define PUT_ADDRESS(p,val)  (*(unsigned long int *)(p) = (unsigned long int) (val))

#define GET(p)  (*(unsigned int *)(p))
#define GET_ADDRESS(p)  (unsigned long int *) (*(unsigned long int *)(p))


#define GET_SIZE(p)     (GET(p) & ~0x7)
#define GET_ALLOC(p)    (GET(p) & 0x1)

#define HDRP(bp)    ((char *)(bp)-WSIZE)
#define FTRP(bp)    ((char *)(bp)+GET_SIZE(HDRP(bp))-DSIZE)

#define NEXT(bp)    ((char *)(bp))
#define PRED(bp)    ((char *)(bp)+ DSIZE)

#define NEXT_BLKP(bp)   ((char *)(bp)+GET_SIZE(((char *)(bp)-WSIZE)))
#define PREV_BLKP(bp)   ((char *)(bp)-GET_SIZE(((char *)(bp)-DSIZE)))

static unsigned long int* free_listp = NULL;
static char* heap_listp = NULL;
static void *find_fit(size_t size);
static void place(void *bp,size_t asize);

static void *coalesce(void *bp);
void insert_to_Emptylist(char *p);
void fix_list(char *p);
static int i = 0 ;
int mm_check(char *function);

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    if((heap_listp = (char *)mem_sbrk(6*WSIZE))==(void *)-1) 
        return -1;
    
    PUT(heap_listp, 0);
    PUT(heap_listp + 1*WSIZE, 0);
    PUT(heap_listp + 2*WSIZE, 0);
    PUT(heap_listp + 3*WSIZE, PACK(DSIZE,1));
    PUT(heap_listp + 4*WSIZE, PACK(DSIZE,1));

    PUT(heap_listp + 5*WSIZE, PACK(0,1));

    heap_listp +=4*WSIZE;

    #ifdef DEBUG
    printf("MMInit 0: %lx %d %lx %d\n ", heap_listp, GET_SIZE(HDRP(heap_listp)), NEXT_BLKP(heap_listp), 
    GET_SIZE(HDRP(NEXT_BLKP(heap_listp))));
    #endif
    return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    unsigned long int *p = 0;
    unsigned long int *epi = 0;
    unsigned long int *curr = 0;
    i = i+1;
    size_t plusepi;
    size_t newsize =  ALIGN(size + 2*DSIZE);

    p =  find_fit(newsize);

    if ( p != NULL){
        place(p,newsize);
        #ifdef DEBUG
        printf("Not so proper: %d, %d, %lx\n",i,  GET_SIZE(HDRP(p)), p);
        mm_check(__FUNCTION__);
        #endif
        return p;
   }

    p = (unsigned long int *) mem_sbrk(newsize);
    

    if (p == (void *)-1)
	    return NULL;

    PUT(HDRP(p),PACK(newsize,1));
    PUT(FTRP(p),PACK(newsize,1));
    PUT(HDRP(NEXT_BLKP(p)), PACK(0,1));
    curr = free_listp;
   #ifdef DEBUG
    printf("\nProper method: %d, %lx, %d",i,  p, GET_SIZE(HDRP(p)));

    mm_check(__FUNCTION__);
    #endif

    return p;
    
}

static void *find_fit(size_t size)
{
    
    unsigned long int *curr = 0;
    curr = free_listp;
    while(curr != NULL)
    {
        #ifdef DEBUG
        printf("\nFound fit: %d, %lx, %d\n",i, curr, GET_SIZE(HDRP(curr)));
        #endif
        if(GET_SIZE(HDRP(curr))>=size) 
            return curr;

        curr = GET_ADDRESS(PRED(curr));
    }
    return NULL;

}


static void place(void *bp,size_t asize)
{
    size_t csize = GET_SIZE(HDRP(bp));
    #ifdef DEBUG
    printf("Placement: %d The whole size is %d and I am %lx\n My pred and next are %lx and %lx \n", i , csize, bp,
    GET_ADDRESS(PRED(bp)), GET_ADDRESS(NEXT(bp)));
    #endif
    size_t fsize = csize - asize;

    fix_linklist(bp);

    if(fsize >= (3*DSIZE))
    {
        PUT(HDRP(bp),PACK(asize,1));
        PUT(FTRP(bp),PACK(asize,1));
    
        bp = NEXT_BLKP(bp);
        PUT(HDRP(bp),PACK(fsize,0));
        PUT(FTRP(bp),PACK(fsize,0));

        PUT_ADDRESS(NEXT(bp),0);
        PUT_ADDRESS(PRED(bp),0);

        coalesce(bp);
    }
    else
    {
        PUT(HDRP(bp),PACK(csize,1));
        PUT(FTRP(bp),PACK(csize,1));
    }
}


/* Frees block and sets the next and predecessor pointers to zero 
 * as well as the allocation bits. Works correctly for the time being.
 */

void mm_free(void *ptr)
{
        i = i+1;
        if(GET_ALLOC(HDRP(ptr)) == 0){

        } else{

       
        PUT(HDRP(ptr), PACK(GET_SIZE(HDRP(ptr)),0));
        PUT(FTRP(ptr), PACK(GET_SIZE(FTRP(ptr)),0));     
        PUT_ADDRESS(NEXT(ptr), 0 );
        PUT_ADDRESS(PRED(ptr), 0 ); 

        coalesce(ptr);
        #ifdef DEBUG
        mm_check(__FUNCTION__); 
        #endif 
         }        
}


static void *coalesce(void *bp)
{
    
    size_t  prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t  next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));

    size_t size = GET_SIZE(HDRP(bp));
    #ifdef DEBUG
    printf("\nCoaled X: %d I am %lx and my PRELIMENARY is (%d,%d). My next and pred is (%lx, %d, %d) and (%lx %d, %d)\n", 
            i, bp, size,  GET_ALLOC(HDRP(bp)), NEXT_BLKP(bp), GET_SIZE(HDRP(NEXT_BLKP(bp))), GET_ALLOC(HDRP(NEXT_BLKP(bp)))
            , PREV_BLKP(bp), GET_SIZE(FTRP(PREV_BLKP(bp))), GET_ALLOC(FTRP(PREV_BLKP(bp))));
            #endif

    if(prev_alloc && next_alloc){

    }
    else if(prev_alloc && !next_alloc)
    {   
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        fix_linklist(NEXT_BLKP(bp));
        PUT(HDRP(bp), PACK(size,0));
        PUT(FTRP(bp), PACK(size,0));
        #ifdef DEBUG
        printf("\nCoaled 1: %d my new size is %d and I (%lx) was added to the block %lx\n", i , size, bp, PREV_BLKP(bp)); 
        #endif     
    }
    else if(!prev_alloc && next_alloc)
    {
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        fix_linklist(PREV_BLKP(bp));
        PUT(FTRP(bp),PACK(size,0));
        PUT(HDRP(PREV_BLKP(bp)),PACK(size,0));
        bp = PREV_BLKP(bp);
        #ifdef DEBUG
        printf("\nCoaled 2: %d my new size is %d and I (%lx) was added to the block %lx\n", i , size, bp, NEXT_BLKP(bp));
    #endif
    }
    else
    {
        size +=GET_SIZE(FTRP(NEXT_BLKP(bp)))+ GET_SIZE(HDRP(PREV_BLKP(bp)));
        fix_linklist(PREV_BLKP(bp));
        fix_linklist(NEXT_BLKP(bp));
        PUT(FTRP(NEXT_BLKP(bp)),PACK(size,0));
        PUT(HDRP(PREV_BLKP(bp)),PACK(size,0));
        bp = PREV_BLKP(bp);
        #ifdef DEBUG
         printf("\nCoaled 3: %d I am %lx and my new size is %d. My next and pred is (%lx, %d, %d) and (%xl %d, %d)\n", 
            i , bp, size, NEXT_BLKP(bp), GET_SIZE(NEXT_BLKP(bp)), GET_ALLOC(NEXT_BLKP(bp))
            , PREV_BLKP(bp), GET_SIZE(PREV_BLKP(bp)), GET_ALLOC(PREV_BLKP(bp)));
            #endif

    }

    insert_to_Emptylist(bp);
    return bp;
}


inline void insert_to_Emptylist(char *p)
{
    char *next =free_listp;
    if(next != NULL){

        PUT_ADDRESS(PRED(p),next);
        PUT_ADDRESS(NEXT(next),p);
        PUT_ADDRESS(NEXT(p),NULL);    
    }

    free_listp = p;
    
}


inline void fix_linklist(char *p)
{
    char *pred = GET_ADDRESS(PRED(p));
    char *next = GET_ADDRESS(NEXT(p));
    #ifdef DEBUG
    printf("\nFix: %d I am %lx and my pred and next are %lx, %lx\n", i, p, pred, next);
    #endif
    if(pred == NULL && next == NULL){
        free_listp = NULL;
    }

    else if(pred == NULL && next != NULL){    
        PUT_ADDRESS(PRED(next),NULL);
        //free_listp = next;
        #ifdef DEBUG
        printf("\nFix 1: %d I am %lx and my pred and next are %lx, %lx\n", i, p, pred, next);
        #endif
    }
    else if(pred != NULL && next != NULL){ 
        
        PUT_ADDRESS(PRED(next),pred);
        PUT_ADDRESS(NEXT(pred),next);
        #ifdef DEBUG
        printf("\nFix 2: %d I am %lx and my pred and next are %lx, %lx\n", i, p, pred, next);
        #endif
    } else if(pred != NULL && next==NULL){
        PUT_ADDRESS(NEXT(pred),NULL);
        free_listp = pred;
    }

    PUT_ADDRESS(NEXT(p),0);
    PUT_ADDRESS(PRED(p),0);
}



void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;
    
    newptr = mm_malloc(size);
    if (newptr == NULL)
      return NULL;
    copySize = *(size_t *)((char *)oldptr - ALIGN(sizeof(size_t)));
    if (size < copySize)
      copySize = size;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;
}

int mm_check(char *function)
{
    char *tmpP = heap_listp;
    printf("\nMMcheck %d: ", i);
    while(GET_SIZE(HDRP(tmpP)) != 0)
    {
     printf("%lx (%d,%d) ",  tmpP, GET_SIZE(HDRP(tmpP)), GET_ALLOC(HDRP(tmpP)) );
     tmpP = NEXT_BLKP(tmpP);
    }

    tmpP= free_listp;
    printf("\nMMcheck Freecheck %d %lx: ", i, free_listp);
  

    
    while(tmpP != NULL)
    {
     printf("%lx (%d,%d) ",  tmpP, GET_SIZE(HDRP(tmpP)), GET_ALLOC(HDRP(tmpP)) );
     tmpP = GET_ADDRESS(PRED(tmpP));
    }

}



