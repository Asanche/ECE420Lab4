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

#define EPSILON 0.00001
#define DAMPING_FACTOR 0.85

#define THRESHOLD 0.0001

int main (int argc, char* argv[])
{
    struct node *nodehead;
    int nodecount;
    int *num_in_links, *num_out_links;
    double *r, *r_pre, *local_r;
    int i, j;
    double damp_const;
    int iterationcount = 0;
    //int collected_nodecount;
    //double *collected_r;
    //double cst_addapted_threshold;
    //double error;
    int npes, rank, localnodecount, processNodeStart, processNodeEnd;
    //FILE *fp;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &npes);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("Hello from process %d out of %d\n", rank, npes);



    // Adjust the threshold according to the problem size
    //cst_addapted_threshold = THRESHOLD;
    
    // Calculate the result

    if (get_node_stat(&nodecount, &num_in_links, &num_out_links)) return 254;
    if (node_init(&nodehead, num_in_links, num_out_links, 0, nodecount)) return 254;

    localnodecount = nodecount / npes;
    processNodeStart = rank * localnodecount;
    //processNodeEnd = processNodeStart + localnodecount;

    r = malloc(nodecount * sizeof(double));
    r_pre = malloc(nodecount * sizeof(double));
    local_r = malloc(localnodecount * sizeof(double));

    if (rank == 0){
        for (i = 0; i < nodecount; ++i){
            r[i] = 1.0 / nodecount;
        }
    }

    damp_const = (1.0 - DAMPING_FACTOR) / nodecount;

    // CORE CALCULATION
    do {
        ++iterationcount;

        if (rank == 0){
            vec_cp(r, r_pre, nodecount);
        }

        MPI_Bcast(r_pre, nodecount, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        //Splits the array to each process
        MPI_Scatter(r, localnodecount, MPI_DOUBLE, local_r, localnodecount, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        

        for (i = 0; i < localnodecount; i++) {
            //printf("Thread: %i  i: %i    local_r[i]:%i\n", rank, i, local_r[i]);
            /*if ( (rank == 0) && (i%10==0)){
                printf("i:%i    local_r[i]:%i", i, local_r[i]);
            }*/
            local_r[i] = 0;
            for (j = 0; j < nodehead[i+processNodeStart].num_in_links; ++j)
                local_r[i] += r_pre[nodehead[i+processNodeStart].inlinks[j]] / num_out_links[nodehead[i+processNodeStart].inlinks[j]];
            local_r[i] *= DAMPING_FACTOR;
            local_r[i] += damp_const;
        }

        MPI_Allgather(local_r, localnodecount, MPI_DOUBLE, r, localnodecount, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        

    } while (false);

    //rel_error(r, r_pre, nodecount) >= EPSILON
    // post processing
    Lab4_saveoutput(r, nodecount, 0);
    MPI_Finalize();
    node_destroy(nodehead, nodecount);
    free(num_in_links); free(num_out_links);
    return 0;
}
