// Compile it with: g++ parallel.cpp -fopenmp
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <unistd.h>
#include <time.h>

using namespace std;

int main() {
  time_t start, end;
  time(&start);
  printf("Testing OpenMP\n");
  int i;
  #pragma omp parallel for
  for (i = 0; i < 3; i++) {
    printf("Loop number %d\n", i);
    usleep(3000000);
  }
  time(&end);
  double dif = difftime(end,start);
  printf("Elapsed time: %f\n", dif);
  return 1;
}
