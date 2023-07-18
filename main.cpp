#include "cmd/linear/data1.h"
#include "cmd/linear/data2.h"
#include "cmd/linear/data3.h"

#include "cmd/pthread/pthread_data1.h"
#include "cmd/pthread/pthread_data3.h"
#include "cmd/pthread/pthread_data2.h"

#include "cmd/openmp/openmp_data1.h"
#include "cmd/openmp/openmp_data2.h"
#include "cmd/openmp/openmp_data3.h"

#include "cmd/mpi/mpi_data1.h"
#include "cmd/mpi/mpi_data2.h"
#include "cmd/mpi/mpi_data3.h"

#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <string>

int main(int argc, char** argv) {
    if(argc != 6) {
        printf("Usage: ./<file> <test_num> <mpi|openmp|pthread|linear> <thread_num> <gen_size> <gen_cnt>\n");
        return 0;
    }
    int testNum = std::atoi(argv[1]);
    std::string type = argv[2];
    int threadCnt = std::atoi(argv[3]);
    int genSize = std::atoi(argv[4]);
    int genCnt = std::atoi(argv[5]);

    int start = time(NULL);
    
    if(type == "linear") {
        if(testNum == 1) {
            runData1(genSize, genCnt);
        } else if(testNum == 2) {
            runData2(genSize, genCnt);
        } else if(testNum == 3) {
            runData3(genSize, genCnt);
        } else {
            printf("Invalid test index!\n");
            return 0;
        }
    } else if(type == "mpi") {
        if(testNum == 1) {
            mpiRunData1(genSize, genCnt);
        }
        else if(testNum == 2) {
            mpiRunData2(genSize, genCnt);
        }
        else if(testNum == 3) {
            mpiRunData3(genSize, genCnt);
        } else {
            printf("Invalid test index!\n");
            return 0;
        }
    } else if(type == "openmp") {
        if(testNum == 1) {
            openmpRunData1(genSize, genCnt, threadCnt);
        }
        else if(testNum == 2) {
            openmpRunData2(genSize, genCnt, threadCnt);
        }
        else if(testNum == 3) {
            openmpRunData3(genSize, genCnt, threadCnt);
        } else {
            printf("Invalid test index!\n");
            return 0;
        }
    } else if(type == "pthread") {
        if(testNum == 1) {
            pthreadRunData1(genSize, genCnt, threadCnt);
        }
        else if(testNum == 2) {
            pthreadRunData2(genSize, genCnt, threadCnt);
        }
        else if(testNum == 3) {
            pthreadRunData3(genSize, genCnt, threadCnt);
        } else {
            printf("Invalid test index!\n");
            return 0;
        }
    } else {
        printf("Invalid implementation!\n");
    }

    printf("Duration: %d seconds\n", time(NULL) - start);
}
