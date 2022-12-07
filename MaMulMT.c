#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#define MAX 500

int row, col;
double matrixA[MAX][MAX], matrixB[MAX][MAX], matrixC[MAX][MAX];

void *mul(void *arg) {
    int index = *((int *)arg);
    for (int j = 0; j < col; j++) {
        for (int k = 0; k < row; k++) {
            matrixC[index][j] += matrixA[index][k] * matrixB[k][j];
        }
    }
    free(arg);
    return NULL;
}

int main() {
    float val;
    FILE *inFile, *outFile;
    char inFileName[20], outFileName[20];
    struct timeval tp_s, tp_e;
    struct timezone tzp_s, tzp_e;

    gettimeofday(&tp_s, &tzp_s);

    for (int c = 1; c <= 1; c++) {

        // name inFileName and outFileName
        sprintf(inFileName, "in/in%d.txt", c);
        sprintf(outFileName, "output/out%d.txt", c);

        // check wether the inFile is exist and open it
        if ((inFile = fopen(inFileName, "r")) == NULL) {
            perror("inFile doesn't exist: ");
            printf("%s\n", inFileName);
        }

        // read row and col
        fscanf(inFile, "%d", &row);
        fscanf(inFile, "%d", &col);
        pthread_t th[row];

        printf("%d\n", c);

        // initiate matrixs
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                fscanf(inFile, "%f", &val);
                matrixA[i][j] = matrixB[i][j] = val;
                matrixC[i][j] = 0;
            }
        }

        // create 500 threads and calculate matrixC
        for (int i = 0; i < row; i++) {
            int *index = malloc(sizeof(int));
            *index = i;
            if (pthread_create(&th[i], NULL, &mul, index) != 0) {
                perror("Faild to create thread");
            }
        }

        // join threads
        for (int i = 0; i < row; i++) {
            if (pthread_join(th[i], NULL) != 0) {
                perror("Faild to join thread");
            } 
        }

        // check the output file and open it
        if ((outFile = fopen(outFileName, "w")) == NULL) {
            perror("outFile doesn't exist: ");
            printf("%s\n", outFileName);
        }

        // write matrixC val
        fprintf(outFile, "%d\n%d\n", row, col);
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                fprintf(outFile, "%f\n", matrixC[i][j]);
            }
        }

        fclose(inFile);
        fclose(outFile);
    }

    gettimeofday(&tp_e, &tzp_e);

    if ((outFile = fopen("myTime.txt", "w")) == NULL) {
        perror("outFile dosen't exits: ");
        printf("myTime.txt\n");
    }

    fprintf(outFile, "Total execution time = %ld\n", tp_e.tv_sec - tp_s.tv_sec);
    fclose(outFile);

    return 0;
}