#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// Ex1
int main(int argc, char **argv) {
  FILE *inF = fopen(argv[1], "r");
  FILE *outF = fopen(argv[2], "w");
  char line[128];
  while(fgets(line, sizeof line, inF) != NULL) {
    if(line[0] == '#') continue;
    unsigned int nl = atoi(strtok(line, " \t"));
    unsigned int nr = atoi(strtok(NULL, " \t"));
    if (nl < nr) fprintf(outF, "%d %d\n", nl, nr);
    else if(nr < nl) fprintf(outF, "%d %d\n", nr, nl);
  }
  fclose(inF);
  fclose(outF);
  return 0;
}
