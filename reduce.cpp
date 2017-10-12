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

  void generateReduceData (int* arr, size_t n);

#ifdef __cplusplus
}
#endif
void reduce (int *arr, int argc, char* argv[]);


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

  if (argc < 5) {
    std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads> <scheduling> <granularity>"<<std::endl;
    return -1;
  }

  int * arr = new int [atoi(argv[1])];

  generateReduceData (arr, atoi(argv[1]));

  //write code here
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
  reduce (arr, argc, argv);
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cerr<<elapsed_seconds.count()<<std::endl;
  delete[] arr;
  return 0;
}
void reduce (int *arr, int argc, char* argv[]) {
  int n = atoi(argv[1]);
  int thrds = atoi(argv[2]);
  std::string sched = argv[3];
  int granularity = atoi(argv[4]);
  int sum = 0;

  omp_set_num_threads(thrds);

  if (sched.compare("static")==0) {
    if (granularity == -1) {
      #pragma omp parallel for schedule(static) reduction(+:sum)
      for (int i = 0; i<n; i++){
        sum+=arr[i];
      }
    }
    else {
      #pragma omp parallel for schedule(static, granularity) reduction(+:sum)
      for (int i = 0; i<n; i++)
        sum+=arr[i];
    }
  }
  else if (sched.compare("dynamic")==0) {
    if (granularity>0){
      #pragma omp parallel for schedule(dynamic, granularity) reduction(+:sum)
      for (int i = 0; i<n; i++)
        sum+=arr[i];
      }
      else {
        #pragma omp parallel for schedule(dynamic) reduction(+:sum)
        for (int i = 0; i<n; i++)
          sum+=arr[i];

      }
  }
  else if (sched.compare("guided")==0) {
    if (granularity>0){
      #pragma omp parallel for schedule(guided, granularity) reduction(+:sum)
      for (int i = 0; i<n; i++)
        sum+=arr[i];
      }
      else {
        #pragma omp parallel for schedule(guided) reduction(+:sum)
        for (int i = 0; i<n; i++)
          sum+=arr[i];

      }

  }

  std::cout<<sum<<std::endl;


}
