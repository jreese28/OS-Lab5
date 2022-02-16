#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef void* (*Operators)(void*);

#define MAX 5

int matA[MAX][MAX];
int matB[MAX][MAX];

int matSumResult[MAX][MAX];
int matDiffResult[MAX][MAX];
int matProductResult[MAX][MAX];

void fillMatrix(int matrix[MAX][MAX]){
  for(int i = 0; i < MAX; i++){
    for(int j = 0; j < MAX; j++){
      matrix[i][j] = rand() % 10 + 1;
        }
    }
}

void printMatrix(int matrix[MAX][MAX]){
  for(int i = 0; i < MAX; i++){
    for(int j = 0; j < MAX; j++){
      printf("%5d", matrix[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

void* computeSum(void* args){
  int *i = (int*) args;
  int row = ((*i) / MAX);
  int column = ((*i) % MAX);
  matSumResult[row][column] = matA[row][column] + matB[row][column];
}

void* computeDiff(void* args){
  int *i = (int*) args;
  int row = ((*i) / MAX);
  int column = ((*i) % MAX);
  matDiffResult[row][column] = matA[row][column] - matB[row][column];
}

void* computeProduct(void* args){
  int *i = (int*) args;
  int row = ((*i) / MAX);
  int column = ((*i) % MAX);
  matProductResult[row][column] = matA[row][column] * matB[row][column];
}

int main(){
  srand(time(0));  // Do Not Remove. Just ignore and continue below.

  int size;
  printf("Enter matrix size: \n");
  scanf("%d", &size);

  #undef MAX
  #define MAX size

  //1. Fill the matrices (matA and matB) with random values.
  fillMatrix(matA);
  fillMatrix(matB);

  // 2. Print the initial matrices.
  printf("Matrix A:\n");
  printMatrix(matA);

  printf("Matrix B:\n");
  printMatrix(matB);

  // 3. Create pthread_t objects for our threads.
  pthread_t objects[3][size * size];

  // 4. Create a thread for each cell of each matrix operation.
  Operators operation[3];
  operation[0] = &computeProduct;
  operation[1] = &computeSum;
  operation[2] = &computeDiff;

  for(int i = 0 ; i < 3 ; i++){
    for(int j = 0 ; j < MAX * MAX ; j++){
      int *function_idx = (int*)malloc(sizeof(int));
      int *index = (int*)malloc(sizeof(int));
      *index = j;
      *function_idx = i;

      pthread_create(&objects[*function_idx][*index], NULL, operation[*function_idx], (void*)index);
    }
  }
  // 5. Wait for all threads to finish.
  for(int i = 0 ; i < 3 ; i++){
    for(int j = 0 ; j < MAX *MAX ; j++){
      pthread_join(objects[i][j], NULL);
    }
  }
  // 6. Print the results.
  printf("Results:\n");
  printf("Sum:\n");
  printMatrix(matSumResult);
  printf("Difference:\n");
  printMatrix(matDiffResult);
  printf("Product:\n");
  printMatrix(matProductResult);
  return 0;
}