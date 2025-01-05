
#ifndef __GRIDGEN
#define __GRIDGEN

#define G_LOG_FILE "gg_log.txt"
#define G_LOG(str) do {FILE* fp = fopen (G_LOG_FILE, "a"); \
    fputs (str, fp); fclose(fp);}while(0)

#define DEBUG_OVERLAP      0

#define RESTART_THRESHOLD  100  // retry with new numbers after this many tries
#define ABORT_THRESHOLD    300  // abort after this many retries

#define CHECK_OVERLAP     -1 
#define NOTHING            0
#define OPEN               1
#define TUNNEL             2
#define DEBUG_OVERLAP_SYM  3

#define MAX_BUF_WIDTH      100
#define MAX_BUF_HEIGHT     100

typedef struct {int x, y;} Point2d;
typedef struct {int **buf, width, height, numSet;} Grid;


#endif
