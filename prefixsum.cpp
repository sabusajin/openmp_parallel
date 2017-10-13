#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <chrono>


#ifdef __cplusplus
extern "C" {
#endif

  void generatePrefixSumData (int* arr, size_t n);
  void checkPrefixSumResult (int* arr, size_t n);


#ifdef __cplusplus
}
#endif

void prefixsum(int *arr, long int n, int thrds);


int main (int argc, char* argv[]) {

  //forces openmp to create the threads beforehand
#pragma omp parallel
  {
    int fd = open (argv[0], O_RDONLY);
    if (fd != -1) {
      close (fd);
    }
    else {
      std::cerr<<"something is amiss"<<std::endl;
    }
  }

  if (argc < 3) {
    std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int * arr = new int [atoi(argv[1])];
  long int n = atol(argv[1]);
  int thrds = atoi(argv[2]);

  generatePrefixSumData (arr, atoi(argv[1]));
  //omp_set_num_threads(thrds);
  int *pr = new int [atoi(argv[1])+1];
  //write code here
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
  prefixsum(arr, n, thrds);
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsed_seconds = end-start;

  std::cerr<<elapsed_seconds.count()<<std::endl;
  pr[0] = 0;
  for (int i=1;i<n;i++) {
    pr[i+1]=arr[i];
  }
  checkPrefixSumResult(pr, atoi(argv[1]));

  delete[] arr;

  return 0;
}

void prefixsum(int *arr, long int n, int thrds) {

  int *temp;
  omp_set_num_threads(thrds);
  #pragma omp parallel
  {
    int thnum = omp_get_thread_num();
    #pragma omp single
    {
            temp = new int[thrds+1];
            temp[0] = 0;
    }
    
    int sum = 0;
    #pragma omp for schedule(static)
    for (int i=0; i<n; i++) {
        sum += arr[i];
        arr[i] = sum;
    }
    temp[thnum+1] = sum;

    #pragma omp barrier
    int offset = 0;
    for(int i=0; i<(thnum+1); i++) {
          offset += temp[i];
    }
    #pragma omp for schedule(static)
    for (int i=0; i<n; i++) {
        arr[i] += offset;
    }

  }


}
