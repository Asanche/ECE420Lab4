/*
    Test the result stored in "data_output" against a serial implementation.

    -----
    Compiling:
    Include "Lab4_IO.c" to compile. Set the macro "LAB4_EXTEND" defined in the "Lab4_IO.c" file to include the extended functions
    $ gcc serialtester.c Lab4_IO.c -o serialtester -lm 

    -----
    Return values:
    0      result is correct
    1      result is wrong
    2      problem size does not match
    253    no "data_output" file
    254    no "data_input" file
*/
#define LAB4_EXTEND

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"
#include "../devkit/Lab4_IO.h"
#include "../devkit/timer.h"


#define EPSILON 0.00001
#define DAMPING_FACTOR 0.85

#define THRESHOLD 0.0001

int main (int argc, char* argv[])
{
    struct node *nodehead;
    int nodecount;
    int *num_in_links, *num_out_links;
    double *r, *r_pre;
    double damp_const;
    int iterationcount = 0;
    int collected_nodecount;
    double *collected_r;
    double cst_addapted_threshold;
    // double error;
    int npes, rank;
    double start, end;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &npes);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Adjust the threshold according to the problem size
    cst_addapted_threshold = THRESHOLD;

    if (get_node_stat(&nodecount, &num_in_links, &num_out_links)) return 254;
    
    collected_nodecount = nodecount / npes;

    // Calculate the result
    if (node_init(&nodehead, num_in_links, num_out_links, nodecount * (rank / npes), ((rank + 1) / npes))) return 254;
    
    r = malloc(nodecount * sizeof(double));
    r_pre = malloc(nodecount * sizeof(double));
    collected_r = malloc(collected_nodecount * sizeof(double));

    for (int i = 0; i < nodecount; ++i) {
        r[i] = 1.0 / nodecount;
    }

    damp_const = (1.0 - DAMPING_FACTOR) / nodecount;

    printf("Hello from process %d out of %d. We have %d nodes\n", rank, npes, collected_nodecount);
    
    GET_TIME(start)

    // CORE CALCULATION
    do {
        ++iterationcount;
        printf("Process %d, iteration %d\n", rank, iterationcount);
        vec_cp(r, r_pre, nodecount);
        for (int i = 0; i < collected_nodecount; ++i) {
            printf("IDK WTF rank: %d, i: %d \n", rank, i);
            collected_r[i] = 0;
            for (int j = 0; j < nodehead[i].num_in_links; ++j) {
                printf("HELP ME rank: %d, j: %d \n", rank, j);
                collected_r[i] += r_pre[nodehead[i].inlinks[j]] / num_out_links[nodehead[i].inlinks[j]];
            }
            printf("collected_r before, rank: %d, %d: %f \n", rank, i, collected_r[i]);

            collected_r[i] *= DAMPING_FACTOR;
            collected_r[i] += damp_const;

            printf("collected_r after, rank: %d, %d: %f \n", rank, i, collected_r[i]);
        }

        printf("Allgather!\n");
        MPI_Allgather(collected_r, collected_nodecount, MPI_DOUBLE, r, collected_nodecount, MPI_DOUBLE, MPI_COMM_WORLD);
    } while (rel_error(r, r_pre, nodecount) >= EPSILON);

    GET_TIME(end);
    printf("End Timing\n");

    printf("Calculation Time: %f\n", (end - start));

    // post processing
    Lab4_saveoutput(r, nodecount, 0);
    MPI_Finalize();
    node_destroy(nodehead, nodecount);
    free(num_in_links); free(num_out_links);
    return 0;
}
