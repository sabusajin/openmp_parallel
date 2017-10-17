#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <algorithm>
#include <chrono>

#ifdef __cplusplus
extern "C" {
#endif

  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (int* arr, size_t n);


#ifdef __cplusplus
}
#endif

unsigned long int n;
void merge_sort(int *a, std::string sched, int granularity);
void merge(int *arr, unsigned long left, unsigned long middle, unsigned long right);

/*Main function*/

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
    std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads> <policy> <granularity>"<<std::endl;
    return -1;
  }

  int * arr = new int [atoi(argv[1])];

  generateMergeSortData (arr, atoi(argv[1]));

  //write code here
  n = atol (argv[1]);
  int thrds = atoi(argv[2]);
  std::string sched = argv[3];
  int granularity = atoi(argv[4]);
  omp_set_num_threads (thrds);

  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
  merge_sort (arr, sched, granularity);
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cerr<<elapsed_seconds.count()<<std::endl;

  checkMergeSortResult (arr, atoi(argv[1]));

  delete[] arr;

  return 0;
}

/*Merge function- same as sequential merge*/

void merge(int *arr, unsigned long left, unsigned long middle, unsigned long right)
{
	unsigned long n1 = middle - left + 1, n2 = right - middle;
	int L[n1], R[n2];
	for (unsigned long i = 0; i < n1; i++)
	{
		L[i] = arr[left + i];
	}
	for (unsigned long j = 0; j < n2; j++)
	{
		R[j] = arr[middle + 1+ j];
	}


	unsigned long i = 0, j = 0, k = left;

	// Merging 2 arrays of length n1 & n2
	while (i < n1 && j < n2)
	{
		if (L[i] <= R[j])
		{
			arr[k++] = L[i++];
		}
		else
		{
			arr[k++] = R[j++];
		}
	}

	if(j == n2) // in case R finishes first, leftover elements of L will get inserted as it is
	{
		while (i < n1)
		{
			arr[k++] = L[i++];
		}
	}
	else if(i == n1) // in case L finishes first, leftover elements of R will get inserted as it is
	{
		while (j < n2)
		{
			arr[k++] = R[j++];
		}
	}
	return;
}

/*Parallel implementation of merge function*/

void merge_sort(int *a, std::string sched, int granularity)
{

  /*Checks for scheduling policy*/
  if (sched.compare("static")) {
    if(granularity == -1){
	   for (unsigned long i=1; i<=n-1; i = 2*i)
	    {
		      #pragma omp parallel for schedule(static)
		      for (unsigned long j=0; j<n-1; j += 2*i)
		      {
             unsigned long right = j+(2*i)-1 < n-1? j+(2*i)-1:n-1;
			       unsigned long middle = j+i-1< n-1?j+i-1:n-1;

			       merge(a, j, middle, right);
		        }

        }
      }
      else {
        for (unsigned long i=1; i<=n-1; i = 2*i)
   	    {
   		      #pragma omp parallel for schedule(static, granularity)
   		      for (unsigned long j=0; j<n-1; j += 2*i)
   		      {
                unsigned long right = j+(2*i)-1 < n-1? j+(2*i)-1:n-1;
   			       unsigned long middle = j+i-1< n-1?j+i-1:n-1;

   			       merge(a, j, middle, right);
   		        }

           }

      }
    }
    else if (sched.compare("dynamic")) {
      if(granularity == -1){
  	   for (unsigned long i=1; i<=n-1; i = 2*i)
  	    {
  		      #pragma omp parallel for schedule(dynamic)
  		      for (unsigned long j=0; j<n-1; j += 2*i)
  		      {
               unsigned long right = j+(2*i)-1 < n-1? j+(2*i)-1:n-1;
  			       unsigned long middle = j+i-1< n-1?j+i-1:n-1;

  			       merge(a, j, middle, right);
  		        }

          }
        }
        else {
          for (unsigned long i=1; i<=n-1; i = 2*i)
     	    {
     		      #pragma omp parallel for schedule(dynamic, granularity)
     		      for (unsigned long j=0; j<n-1; j += 2*i)
     		      {
                  unsigned long right = j+(2*i)-1 < n-1? j+(2*i)-1:n-1;
     			        unsigned long middle = j+i-1< n-1?j+i-1:n-1;

     			        merge(a, j, middle, right);
     		        }

             }

        }
      }
      else if (sched.compare("guided")) {
        if(granularity == -1){
    	   for (int i=1; i<=n-1; i = 2*i)
    	    {
    		      #pragma omp parallel for schedule(guided)
    		      for (int j=0; j<n-1; j += 2*i)
    		      {
                 int right = j+(2*i)-1 < n-1? j+(2*i)-1:n-1;
    			       int middle = j+i-1< n-1?j+i-1:n-1;

    			       merge(a, j, middle, right);
    		        }

            }
          }
          else {
            for (int i=1; i<=n-1; i = 2*i)
       	    {
       		      #pragma omp parallel for schedule(guided, granularity)
       		      for (int j=0; j<n-1; j += 2*i)
       		      {
                    int right = j+(2*i)-1 < n-1? j+(2*i)-1:n-1;
       			       int middle = j+i-1< n-1?j+i-1:n-1;

       			       merge(a, j, middle, right);
       		        }

               }

          }
        }
        else {
          std::cout<<"Please use either static, guided, or dynamic!!!"<<std::endl;
          exit(-1);
        }
  }
