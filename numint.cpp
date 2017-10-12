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

float f1(float x, int intensity);
float f2(float x, int intensity);
float f3(float x, int intensity);
float f4(float x, int intensity);

#ifdef __cplusplus
}
#endif

float integrate(char* argv[]) {
  int functionid = atoi(argv[1]);
  float a = atof(argv[2]);
  float b = atof(argv[3]);
  unsigned long n = atol(argv[4]);
  uint32_t intensity = atoi(argv[5]);
  int thrds = atoi(argv[6]);
  std::string sched = argv[7];
  int granularity = atoi(argv[8]);
  float multiplier = (b - a) / (float)n;
  float result = 0.0;

  omp_set_num_threads(thrds);

  if (sched.compare("static") == 0) {
    if (granularity > 0) {
      float x = 0.0;
#pragma omp parallel for schedule(static, granularity) reduction(+ : result)
      for (int i = 0; i < n; i++) {
        x = a + ((float)i + 0.5) * multiplier;
        switch (functionid) {
          case 1:
            result = result + f1(x, intensity) * multiplier;
            break;
          case 2:
            result = result + f2(x, intensity) * multiplier;
            break;
          case 3:
            result = result + f3(x, intensity) * multiplier;
            break;
          case 4:
            result = result + f4(x, intensity) * multiplier;
            break;
          default:
            break;
        }
      }

    } else {
      float x = 0.0;
#pragma omp parallel for schedule(static) reduction(+ : result)
      for (int i = 0; i < n; i++) {
        x = a + ((float)i + 0.5) * multiplier;
        switch (functionid) {
          case 1:
            result = result + f1(x, intensity) * multiplier;
            break;
          case 2:
            result = result + f2(x, intensity) * multiplier;
            break;
          case 3:
            result = result + f3(x, intensity) * multiplier;
            break;
          case 4:
            result = result + f4(x, intensity) * multiplier;
            break;
          default:
            break;
        }
      }
    }
  } else if (sched.compare("dynamic") == 0) {
    if (granularity > 0) {
      float x = 0.0;
#pragma omp parallel for schedule(dynamic, granularity) reduction(+ : result)
      for (int i = 0; i < n; i++) {
        x = a + ((float)i + 0.5) * multiplier;
        switch (functionid) {
          case 1:
            result = result + f1(x, intensity) * multiplier;
            break;
          case 2:
            result = result + f2(x, intensity) * multiplier;
            break;
          case 3:
            result = result + f3(x, intensity) * multiplier;
            break;
          case 4:
            result = result + f4(x, intensity) * multiplier;
            break;
          default:
            break;
        }
      }

    } else {
      float x = 0.0;
#pragma omp parallel for schedule(dynamic) reduction(+ : result)
      for (int i = 0; i < n; i++) {
        x = a + ((float)i + 0.5) * multiplier;
        switch (functionid) {
          case 1:
            result = result + f1(x, intensity) * multiplier;
            break;
          case 2:
            result = result + f2(x, intensity) * multiplier;
            break;
          case 3:
            result = result + f3(x, intensity) * multiplier;
            break;
          case 4:
            result = result + f4(x, intensity) * multiplier;
            break;
          default:
            break;
        }
      }
    }
  } else if (sched.compare("guided") == 0) {
    if (granularity > 0) {
      float x = 0.0;
#pragma omp parallel for schedule(guided, granularity) reduction(+ : result)
      for (int i = 0; i < n; i++) {
        x = a + ((float)i + 0.5) * multiplier;
        switch (functionid) {
          case 1:
            result = result + f1(x, intensity) * multiplier;
            break;
          case 2:
            result = result + f2(x, intensity) * multiplier;
            break;
          case 3:
            result = result + f3(x, intensity) * multiplier;
            break;
          case 4:
            result = result + f4(x, intensity) * multiplier;
            break;
          default:
            break;
        }
      }

    } else {
      float x = 0.0;
#pragma omp parallel for schedule(guided) reduction(+ : result)
      for (int i = 0; i < n; i++) {
        x = a + ((float)i + 0.5) * multiplier;
        switch (functionid) {
          case 1:
            result = result + f1(x, intensity) * multiplier;
            break;
          case 2:
            result = result + f2(x, intensity) * multiplier;
            break;
          case 3:
            result = result + f3(x, intensity) * multiplier;
            break;
          case 4:
            result = result + f4(x, intensity) * multiplier;
            break;
          default:
            break;
        }
      }
    }
  }
  return result;
}

int main(int argc, char* argv[]) {
// forces openmp to create the threads beforehand
#pragma omp parallel
  {
    int fd = open(argv[0], O_RDONLY);
    if (fd != -1) {
      close(fd);
    } else {
      std::cerr << "something is amiss" << std::endl;
    }
  }

  if (argc < 9) {
    std::cerr << "Usage: " << argv[0]
              << " <functionid> <a> <b> <n> <intensity> <nbthreads> "
                 "<scheduling> <granularity>"
              << std::endl;
    return -1;
  }
  std::chrono::time_point<std::chrono::system_clock> start =
      std::chrono::system_clock::now();
  float result = integrate(argv);
  std::chrono::time_point<std::chrono::system_clock> end =
      std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;

  std::cout << result << std::endl;

  std::cerr << elapsed_seconds.count() << std::endl;

  return 0;
}
