build:
	mpic++ -fopenmp -lpthread -g -O0 -o main *.cpp */*.cpp */*/*.cpp
