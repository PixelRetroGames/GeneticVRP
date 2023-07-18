# Solving the Vechicle Routing Problem using Genetic Algorithms

## Team
* Cioltan Marian-Alexandru
* Tulba-Lecu Theodor-Gabriel

## Assistant
* Andrei-Catalin Ouatu

## Problem Description
The Vechicle Routing Problem (VRP) is a generalisation of the Travelling Salesman Problem (TSP). <br>
The statement of Capacitated VRP (CVRP) is the following:<br> 
1. Let `S` be a set of `N` 2D Drop-off points
2. Let `K` be the size of the fleet of transporting vechicles
3. Let `D` be the 2D coordinate of the depot
4. Let `C` be the capacity of each vechicle
5. Let `s_i` be the quantity of goods that need to be transported to the `i-th` Drop-off point<br>
6. Each vechicle follows a path that starts and ends from the depot.

What is the minimum sum of the distance traveled by all the `K` vechicles knowing that all Drop-off points receive their goods and no vechicle is overloaded (i.e. The sum of quantities of the visited nodes in the path of the vechicle does not exceed `C`).

## Algorithm Description
Since finding an optimal solution for VRP is NP-hard, our approach is an approximation algorithm implemented using a Genetic Algorithm.<br>
### Algorithm Overview
A Genetic Algorithm (GA) is inspired by the process of natural selection of species in nature.<br>
The pseudocode of the algorithm is as follows:
0. Choose a population size and the number of generations that we want to run
1. Start with a population of individuals with random genes
2. For each generation do the following
3. For each individual in the population, compute it's performance using a fitness function and sort them by their fitness
4. Pair up the best-preforming individuals and create offspring from each pair
5. For each pair of parents, create 2 offsprings by mixing up the genes of the parents (a process called Crossover)
6. Mutate a small percentage of the genes of the offsprings in order to provide evolutionary randomness
7. The worse-preforming individuals are discarded and replaced by the newly born individuals
8. If we've reached the final generation, output the best individual, otherwise go to step 2

### Encoding the problem in the genome
The VRP problem can be encoded in a genome as follows:<br>
Consider an array equal with the count of Drop-off points, the `i-th` entry in the array describes what vehicle and at what time visits the drop-off point.<br>
An entry in the genome (also called a chromosome) is represented as a real number, where the integer part represents the index of the vechicle that visits that drop-off point, and the fractional part represents the `time` at which the vechicle visits the point. For example consider the following genome `[0.1, 1.2, 0.3, 1.0, 1.4]` for 5 drop-off points and 2 vechicles. The decoded solution is:
* Vechicle `0` visits drop-off point `0` and then drop-off point `2`
* Vechicle `1` visits drop-off points `3`, `1` and `4`

### Fitness function
Since not all encoding of genomes are valid solutions (a vechicle can be overloaded), the fitness function is directly proportional with the length of the path, and inversely proportional with the square of the quantity of overload. For example, a vechicle that takes a path of length `100` and is overload by `20` will have a fitness of `100 + 20^2 = 500`.

### Selection
Selection has 3 steps:
1. **Elitism** - Take a small fraction of the best-performing individuals and copy them in the new popoulation 
2. **Breeding** - Take pairs of the next best-performing individuals and create offsprings by combining their genes in the crossover process
3. **Newcomers** - A small percentage of individuals are created at random in order to provide envolutionary randomness to reduce the chance of getting stuck in a local minimum

### Crossover
The crossover process is uniform (i.e each chromosome is chosen at random from one of the 2 parents with equal probabilities).

### Mutation 
The mutation process is split into two distinct proceses:
1. When a new individual is created as the offspring of two individuals from the previous generation, a small percentage of it's chromosomes are replaced by new random ones
2. A global mutation is applied to all the individuals of a generation in order to decrease the chance of getting stuck in a local minimum. This simulates the process of adaptation to new environments found in nature

## Parallelization
The most time-consuming part of the algorithm is calculating the fitness of each individual, this has an asymptotic time complexity of `O(N * PopSize)`. This was also confirmed using the tool *Intel VTune Profiler* for analysing the preformance of the algorithm.<br>
Luckily, this part of the algorithm is easily parallelizable, as the fitness can be calculated independently for each individual. <br><br>

Three parallel implementations are provided in this project:
* Pthread
* OpenMP
* MPI

## Project Structure
The tree project has the following structure:
```
├── plots *visualisation of speed-up and performance*
├── cmd *test functions*
│   ├── linear
│   ├── mpi
│   ├── openmp
│   └── pthread
├── dataset
│   └── unicamp
│       ├── problems *tests + Visualisation python scripts*
│       ├── results *GA solutions*
│       └── solutions *optimal solutions*
├── genetic *abstract implementation of a genetic algorithm*
│   ├── best_individual_keeper.h
│   ├── crossover_calculator.h
│   ├── fitness_calculator.h
│   ├── genetic_algorithm.cpp
│   ├── genetic_algorithm.h
│   ├── individual.cpp
│   ├── individual.h
│   ├── initializer.h
│   ├── mutator.h
│   └── selector.h
├── main.cpp
├── Makefile
└── VRP
    ├── linear
    │   ├── *implementation of the serial CVRPGA*
    ├── mpi
    │   ├── *MPI implementation of CVRPGA*
    ├── openmp
    │   ├── *OpenMP implementation of CVRPGA*
    ├── pthread
    │   ├── *Pthread implementation of CVRPGA*
    └── *other utilitary files*
```

## Dependencies
This project requires the existence of `MPI` `OpenMP` and `Pthread` on the machine.

## Building & Running the Project
### Building
In order to build the project just run the `build` rule of the `Makefile`.

### Running
* **linear** - `./<binary> <test_num{1, 2, 3}> linear 0 <gen_size> <gen_cnt>`
* **Pthread** - `./<binary> <test_num{1, 2, 3}> pthread <thread_cnt> <gen_size> <gen_cnt>`
* **OpenMP** - `./<binary> <test_num{1, 2, 3}> openmp <thread_cnt> <gen_size> <gen_cnt>`
* **MPI** - `mpirun -np <thread_cnt> ./<binary> <test_num{1, 2, 3}> mpi 0 <gen_size> <gen_cnt>`

## Visualisation and Results
### Pthread
![Pthread Test #1](/plots/pthread_test1.png)
![Pthread Speed-up #1](/plots/pthread_speedup1.png)
![Pthread Test #2](/plots/pthread_test2.png)
![Pthread Speed-up #2](/plots/pthread_speedup2.png)
![Pthread Test #3](/plots/pthread_test3.png)
![Pthread Speed-up #3](/plots/pthread_speedup3.png)
### OpenMP
![OpenMP Test #1](/plots/openmp_test1.png)
![OpenMP Speed-up #1](/plots/openmp_speedup1.png)
![OpenMP Test #2](/plots/openmp_test2.png)
![OpenMP Speed-up #2](/plots/openmp_speedup2.png)
![OpenMP Test #3](/plots/openmp_test3.png)
![OpenMP Speed-up #3](/plots/openmp_speedup3.png)
### MPI
![MPI Test #1](/plots/mpi_test1.png)
![MPI Speed-up #1](/plots/mpi_speedup1.png)
![MPI Test #2](/plots/mpi_test2.png)
![MPI Speed-up #2](/plots/mpi_speedup2.png)
![MPI Test #3](/plots/mpi_test3.png)
![MPI Speed-up #3](/plots/mpi_speedup3.png)
