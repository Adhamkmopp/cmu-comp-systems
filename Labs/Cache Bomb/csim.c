#include "cachelab.h"
#include "csapp.h"
#include <getopt.h>

typedef struct{
  long int tag;
  int priority;
  int valid;
  struct Line *next;
} Line;

int main(int argc, char **argv)
{
 /*
  * Argument input with getop
  */
  int opt;
  FILE *trace;
  int s, e, b;
  Line **sets;
  char tracefileName[256];
  char buff;
  long int hitmiss;
  long int address;
  long int tag;
  long int set_index;
  long int mask = 0xffffffffffffffff;
  long int set_num;
  int hit; 
  int hits = 0; 
  int misses = 0; 
  int evictions = 0;
  Line *lru, *curr_line;

    

  while ((opt = getopt(argc, argv, "s:E:b:t:")) != -1) 
    switch(opt)
    {
      case 's':
        s = atoi(optarg);
        break;
      case 'E':
        e = atoi(optarg);
        break;
      case 'b':
        b = atoi(optarg);
        break;
      case 't':
        strcpy(tracefileName, optarg);
        break;
      case '?':
        if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr,
          "Unknown option character `\\x%x'.\n",optopt);
      default:
        printf("Wrong parameter arguments");
        exit(0);
      }

  set_num = 1 << s;
  mask = ~((unsigned long int)mask << s); 
  trace = fopen(tracefileName, "r");
   
  sets = (Line **) malloc(set_num * sizeof(Line *));
   
  
  for (int i=0; i < set_num; i++)
    sets[i] = (Line *) malloc(e* sizeof(Line));

  

  for (int i=0; i < set_num; i++)
    for (int j =0; j < e; j++){
      sets[i][j].priority = j;
      sets[i][j].tag = 0;
      sets[i][j].valid = 0;

    }
      
/* setting up a made up priority sequence for the initial case of cold miss  */

  while(fscanf(trace," %c %lx,%ld", &buff, &address, &hitmiss) > 0){
   
    tag = address >> (b + s);
    set_index = (address >> b) & mask;
    lru = &sets[set_index][0];
    int j;
    hit = 0;
     //printf("%lx, %lx, %lx\n",tag, address, hitmiss );
    for (j = 0; j < e; j++){
        
      curr_line = &sets[set_index][j];

    if(curr_line -> tag == tag){
        curr_line->priority = 0;

        if(buff == 'M'){
          hits = hits +2;
        } else {
          hits++;
        }
        
        hit = 1;
        break;
      }
      curr_line -> priority++;

    }

    j++;
    for (; j < e; j++){ 
      curr_line = &sets[set_index][j];
      curr_line -> priority++;
     
    }
  

    if(hit == 0){
      lru = & sets[set_index][0];
      for (j=0 ; j < e; j++){ 
        curr_line = &sets[set_index][j];

        if(lru -> priority < curr_line -> priority){
          lru = curr_line;
        }
      }

      lru -> tag = tag;
      lru -> priority = 0;
      if(lru->valid == 0 && (int)buff ==77){
        misses++;
        hits++;
        lru -> valid = 1;
      } else if (lru->valid == 0 && (int)buff != 77){
        misses++;
        lru -> valid = 1;
      } else if ((int)buff == 77) {
        hits++;
        evictions++;
        misses++;
      } else {
        evictions++;
        misses++;
      }
      
    }
     
}

    
    printf("s = %d, e = %d, b = %d, t = %s\n", s, e, b, tracefileName);
    printSummary(hits, misses, evictions);
    fclose(trace);
    return 0;
}

     