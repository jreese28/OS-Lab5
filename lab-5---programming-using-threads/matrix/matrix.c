#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

// Used this as a reference: https://www.geeksforgeeks.org/multiplication-of-matrix-using-threads/

int* matA;
int* matB;
int* matSumResult;
int* matDiffResult;
int* matProductResult;

int MAX;
int turn0 = 0, turn1 = 0, turn2 = 0; 

void fillMatrix(int* matrix) {
    for(int i = 0; i < MAX; i++) {
        for(int j = 0; j < MAX; j++) {
		*(matrix + i*MAX + j) = rand()%10+1;
        }
    }
}

void printMatrix(int* matrix) {
    for(int i = 0; i < MAX; i++) {
        for(int j = 0; j < MAX; j++) {
            printf("%5d", *(matrix + i*MAX + j));
        }
        printf("\n");
    }
    printf("\n");
}


void* computeSum(void* args) {
	int i, j;
	int k = turn0++;
	for (i = k ; i < (k + 1); i++) {
		for (j = 0; j < MAX; j++) { 
			*((int*)matSumResult + i*MAX + j) = *(matA + i*MAX + j) + *(matB + i*MAX + j); 

		}
	}
}


void* computeDiff(void* args) {
	int i, j;
	int k = turn1++;
	for (i = k; i < (k + 1); i++) {
		for (j = 0; j < MAX; j++) { 
			*((int*)matDiffResult + i*MAX + j) = *(matA + i*MAX + j) - *(matB + i*MAX + j); 
		}
	}
}


void* computeProduct(void* args) {
		int i, j, m;
		int k = turn2++;
		for (i = k; i < (k + 1); i++) {
			for (j = 0; j < MAX; j++) { 
				for (m = 0; m < MAX; m++) {
          				*((int*)matProductResult + i*MAX + j) += *(matA + i*MAX + m) * *(matB + m*MAX + j); 
				}
			}
		}
}

int main() {
    srand(time(0));  
    // Sets seed for call to random below.
    // 0. Get the matrix size from the command line
    printf("What NxN matrix size would you like? Please enter an integer N.\n");
    scanf("%d", &MAX);

    // 0.1 Dynamically allocate matrices.
    matA = (int *)malloc(MAX * MAX * sizeof(int));
    matB = (int *)malloc(MAX * MAX * sizeof(int));
    matSumResult = (int *)malloc(MAX * MAX * sizeof(int));
    matDiffResult = (int *)malloc(MAX * MAX * sizeof(int));
    matProductResult = (int *)malloc(MAX * MAX * sizeof(int));

    // 1. Fill the matrices (matA and matB) with random values.
    fillMatrix(matA);
    fillMatrix(matB);

    // 2. Print the initial matrices.
    printf("Matrix A:\n");
    printMatrix(matA);
    printf("Matrix B:\n");
    printMatrix(matB);

    // 3. Create pthread_t objects for our threads.
    pthread_t threads[MAX];

    // 4. Create a thread for each cell of each matrix operation.
	int i;
	for(i = 0; i < MAX; i++) {
		int* arg;
		pthread_create(&threads[i], NULL, computeSum, (void*)(arg));
		pthread_create(&threads[i], NULL, computeDiff, (void*)(arg)); 
		pthread_create(&threads[i], NULL, computeProduct, (void*)(arg));
	}
    
    // 5. Wait for all threads to finish.
	for (i = 0; i < MAX; i++) {
		pthread_join(threads[i], NULL);  
	}

    // 6. Print the results.
    printf("Results:\n");
    printf("Sum:\n");
    printMatrix(matSumResult);
    printf("Difference:\n");
    printMatrix(matDiffResult);
    printf("Product:\n");
    printMatrix(matProductResult);
    free(matA);
    free(matB);
    free(matSumResult);
    free(matDiffResult);
    free(matProductResult);
    return 0;
}