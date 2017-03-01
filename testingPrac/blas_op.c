#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include "level1.h"
#include "utils.h"
#include <string.h>
#include <limits.h>

#include <mpi.h>

/*
 * Vector dot product, integers
 *
 * result = result + v1_i * v2_i
 *
 * Input: size of the vectors (in number of elements)
 * Output: dot product
 *
 */
int int_dot_product(unsigned int size){

  int world_size, world_rank;
  MPI_Comm_size(MPI_COMM_WORLD,&world_size);
  MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);


  int i;

  /*
   * Compute size of block each rank will work on
   * We do this in a slightly odd method to try an ensure we have the same number of data plane layers (n)
   * in the MPI case as in the serial case.
   */
  int local_size = 0;
  if (world_rank != 0){
    local_size = size / world_size;
  }
  else if (world_rank == 0){
    local_size = (size / world_size) + (size % world_size);
  }
  else{
    printf("Some error occured in size calculation\n");
  }


  /* create two vectors */
  int *v1 = (int *)malloc(local_size * sizeof(int));
  int *v2 = (int *)malloc(local_size * sizeof(int));

  /* result variable */
  unsigned int result = 0;

  if(v1 == NULL || v2 == NULL){
    printf("Out Of Memory: could not allocate space for the two arrays.\n");
    return 0;
  }

  srand((int)time(NULL));

  struct timespec start, end;
  unsigned int global_result = 0;

  /* fill vectors with random integer values */
  for(i=0; i<local_size; i++){
    v1[i] = (int)rand() / (int)(RAND_MAX/10);
    v2[i] = (int)rand() / (int)(RAND_MAX/10);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  clock_gettime(CLOCK_MONOTONIC_RAW, &start);



  /* perform dot product */

  for(i=0; i<local_size; i++){
    result = result + v1[i] * v2[i];
  }

  MPI_Reduce(&result, &global_result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  clock_gettime(CLOCK_MONOTONIC_RAW, &end);

  /* print result so compiler does not throw it away */
  if (world_rank == 0){
    printf("Dot product result: %d\n", result);
    elapsed_time_hr(start, end, "Integer dot product.");
  }
  free(v1);
  free(v2);

  return 0;
}

/*
 * Vector dot product, floats
 *
 * result = result + v1_i * v2_i
 *
 * Input: size of the vectors (in number of elements)
 * Output: dot product
 *
 */
int float_dot_product(unsigned int size){

  int world_size, world_rank;
  MPI_Comm_size(MPI_COMM_WORLD,&world_size);
  MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);

  int i;

  /*
   * Compute size of block each rank will work on
   * We do this in a slightly odd method to try an ensure we have the same number of data plane layers (n)
   * in the MPI case as in the serial case.
   */
  int local_size = 0;
  if (world_rank != 0){
    local_size = size / world_size;
  }
  else if (world_rank == 0){
    local_size = (size / world_size) + (size % world_size);
  }
  else{
    printf("Some error occured in size calculation\n");
  }


  /* create two vectors */
  float *v1 = (float *)malloc(local_size * sizeof(float));
  float *v2 = (float *)malloc(local_size * sizeof(float));

  /* result variable */
  float result = 0;

  if(v1 == NULL || v2 == NULL){
    printf("Out Of Memory: could not allocate space for the two arrays.\n");
    return 0;
  }

  srand((int)time(NULL));

  struct timespec start, end;
  float global_result = 0;

  /* fill vectors with random integer values */
  for(i=0; i<local_size; i++){
    v1[i] = (float)rand() / (float)(RAND_MAX/10);
    v2[i] = (float)rand() / (float)(RAND_MAX/10);
  }

  MPI_Barrier(MPI_COMM_WORLD);
  clock_gettime(CLOCK_MONOTONIC_RAW, &start);



  /* perform dot product */

  for(i=0; i<local_size; i++){
    result = result + v1[i] * v2[i];
  }

  MPI_Reduce(&result, &global_result, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

  clock_gettime(CLOCK_MONOTONIC_RAW, &end);

  /* print result so compiler does not throw it away */
  if (world_rank == 0){
    printf("Dot product result: %f\n", result);
    elapsed_time_hr(start, end, "Float dot product.");
  }
  free(v1);
  free(v2);

  return 0;

}


/*
 * Vector dot product, doubles
 *
 * result = result + v1_i * v2_i
 *
 * Input: size of the vectors (in number of elements)
 * Output: dot product
 *
 */
int double_dot_product(unsigned int size){

  int world_size, world_rank;
  MPI_Comm_size(MPI_COMM_WORLD,&world_size);
  MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);

  int i;

  /*
   * Compute size of block each rank will work on
   * We do this in a slightly odd method to try an ensure we have the same number of data plane layers (n)
   * in the MPI case as in the serial case.
   */
  int local_size = 0;
  if (world_rank != 0){
    local_size = size / world_size;
  }
  else if (world_rank == 0){
    local_size = (size / world_size) + (size % world_size);
  }
  else{
    printf("Some error occured in size calculation\n");
  }


  /* create two vectors */
  double *v1 = (double *)malloc(local_size * sizeof(double));
  double *v2 = (double *)malloc(local_size * sizeof(double));

  /* result variable */
  double result = 0;

  if(v1 == NULL || v2 == NULL){
    printf("Out Of Memory: could not allocate space for the two arrays.\n");
    return 0;
  }

  srand((int)time(NULL));

  struct timespec start, end;
  double global_result = 0;

  /* fill vectors with random integer values */
  for(i=0; i<local_size; i++){
    v1[i] = (double)rand() / (double)(RAND_MAX/10);
    v2[i] = (double)rand() / (double)(RAND_MAX/10);
  }

  MPI_Barrier(MPI_COMM_WORLD);
  clock_gettime(CLOCK_MONOTONIC_RAW, &start);



  /* perform dot product */

  for(i=0; i<local_size; i++){
    result = result + v1[i] * v2[i];
  }

  MPI_Reduce(&result, &global_result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  clock_gettime(CLOCK_MONOTONIC_RAW, &end);

  /* print result so compiler does not throw it away */
  if (world_rank == 0){
    printf("Dot product result: %f\n", result);
    elapsed_time_hr(start, end, "Double dot product.");
  }
  free(v1);
  free(v2);

  return 0;

}


/* Vector scalar product, integers    */
/* v_i = a * v1_i                     */
int int_scalar_product(unsigned int size){

  int world_size, world_rank;
  MPI_Comm_size(MPI_COMM_WORLD,&world_size);
  MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);

  int i;

  /*
   * Compute size of block each rank will work on
   * We do this in a slightly odd method to try an ensure we have the same number of data plane layers (n)
   * in the MPI case as in the serial case.
   */
  int local_size = 0;
  int local_size_other = 0;
  int rcounts[world_size];
  int displs[world_size];

  if (world_rank != 0){
    local_size = size / world_size;
  }
  else if (world_rank == 0){
    local_size = (size / world_size) + (size % world_size);
    local_size_other = size / world_size; // Needed for gatherv
    rcounts[0] = local_size;
    displs[0] = 0;
    for (i=1;i<world_size;i++){
      rcounts[i] = local_size_other;
      displs[i] = i*local_size_other;
    }
  }
  else{
    printf("Some error occured in size calculation\n");
  }

  /* create vector and scalar */
  int *v = (int *)malloc(local_size * sizeof(int));
  unsigned int a = 0;
  int* rbuf = NULL;

  /* We only need this space allocated on PE 0 */
  if (world_rank == 0){
    rbuf = (int *)malloc(size * sizeof(int));
    if (rbuf == NULL){
      printf("Out Of Memory: could not allocate space for the array.\n");
      return 0;
    }
  }

  if(v == NULL){
    printf("Out Of Memory: could not allocate space for the array.\n");
    return 0;
  }

  srand((int)time(NULL));

  struct timespec start, end;

  /* fill vector with random ints */
  for(i=0; i<local_size; i++){
    v[i] = (int)rand() / (int)(RAND_MAX/10);
  }

  /* assign random int value */
  a = (int)rand() / (int)(RAND_MAX/10);

  if (world_size > 1){
    MPI_Bcast(&a, 1, MPI_INT, 0, MPI_COMM_WORLD); // Make everyone use the SAME a */
  }

  MPI_Barrier(MPI_COMM_WORLD);

  if (world_rank == 0){
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
  }


  /* perform scalar product */
  for(i=0; i<local_size; i++){
    v[i] = a * v[i];
  }

  /* Collect elements on PE 0 */
  if (world_size > 1){
    MPI_Gatherv(v, local_size, MPI_INT, rbuf, rcounts, displs, MPI_INT, 0, MPI_COMM_WORLD);
  }

  if (world_rank == 0){
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
  }

  /* print result so compiler does not throw it away */
  if (a==999999){
    printf("Scalar product result: %d\n", v[0]);
  }

  if (world_rank == 0){
    elapsed_time_hr(start, end, "Int scalar product.");
    free(rbuf);
  }
  free(v);


  return 0;

}

/* Vector scalar product, floats    */
/* v_i = a * v1_i                     */
int float_scalar_product(unsigned int size){

  int world_size, world_rank;
  MPI_Comm_size(MPI_COMM_WORLD,&world_size);
  MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);

  int i;

  /*
   * Compute size of block each rank will work on
   * We do this in a slightly odd method to try an ensure we have the same number of data plane layers (n)
   * in the MPI case as in the serial case.
   */
  int local_size = 0;
  int local_size_other = 0;
  int rcounts[world_size];
  int displs[world_size];

  if (world_rank != 0){
    local_size = size / world_size;
  }
  else if (world_rank == 0){
    local_size = (size / world_size) + (size % world_size);
    local_size_other = size / world_size; // Needed for gatherv
    rcounts[0] = local_size;
    displs[0] = 0;
    for (i=1;i<world_size;i++){
      rcounts[i] = local_size_other;
      displs[i] = i*local_size_other;
    }
  }
  else{
    printf("Some error occured in size calculation\n");
  }

  /* create vector and scalar */
  float *v = (float *)malloc(local_size * sizeof(float));
  unsigned int a = 0;
  float* rbuf = NULL;

  /* We only need this space allocated on PE 0 */
  if (world_rank == 0){
    rbuf = (float *)malloc(size * sizeof(float));
    if (rbuf == NULL){
      printf("Out Of Memory: could not allocate space for the array.\n");
      return 0;
    }
  }

  if(v == NULL){
    printf("Out Of Memory: could not allocate space for the array.\n");
    return 0;
  }

  srand((int)time(NULL));

  struct timespec start, end;

  /* fill vector with random floats */
  for(i=0; i<local_size; i++){
    v[i] = (float)rand() / (float)(RAND_MAX/10);
  }

  /* assign random float value */
  a = (float)rand() / (float)(RAND_MAX/10);

  if (world_size > 1){
    MPI_Bcast(&a, 1, MPI_FLOAT, 0, MPI_COMM_WORLD); // Make everyone use the SAME a */
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if (world_rank == 0){
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
  }


  /* perform scalar product */
  for(i=0; i<local_size; i++){
    v[i] = a * v[i];
  }

  /* Collect elements on PE 0 */
  if (world_size > 1){
    MPI_Gatherv(v, local_size, MPI_FLOAT, rbuf, rcounts, displs, MPI_FLOAT, 0, MPI_COMM_WORLD);
  }

  if (world_rank == 0){
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
  }

  /* print result so compiler does not throw it away */
  if (a==999999){
    printf("Scalar product result: %f\n", v[0]);
  }

  if (world_rank == 0){
    elapsed_time_hr(start, end, "Float scalar product.");
    free(rbuf);
  }
  free(v);

  return 0;

}

/* Vector scalar product, doubles    */
/* v_i = a * v1_i                     */
int double_scalar_product(unsigned int size){


  int world_size, world_rank;
  MPI_Comm_size(MPI_COMM_WORLD,&world_size);
  MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);

  int i;

  /*
   * Compute size of block each rank will work on
   * We do this in a slightly odd method to try an ensure we have the same number of data plane layers (n)
   * in the MPI case as in the serial case.
   */
  int local_size = 0;
  int local_size_other = 0;
  int rcounts[world_size];
  int displs[world_size];

  if (world_rank != 0){
    local_size = size / world_size;
  }
  else if (world_rank == 0){
    local_size = (size / world_size) + (size % world_size);
    local_size_other = size / world_size; // Needed for gatherv
    rcounts[0] = local_size;
    displs[0] = 0;
    for (i=1;i<world_size;i++){
      rcounts[i] = local_size_other;
      displs[i] = i*local_size_other;
    }
  }
  else{
    printf("Some error occured in size calculation\n");
  }

  /* create vector and scalar */
  double *v = (double *)malloc(local_size * sizeof(double));
  unsigned int a = 0;
  double* rbuf = NULL;

  /* We only need this space allocated on PE 0 */
  if (world_rank == 0){
    rbuf = (double *)malloc(size * sizeof(double));
    if (rbuf == NULL){
      printf("Out Of Memory: could not allocate space for the array.\n");
      return 0;
    }
  }

  if(v == NULL){
    printf("Out Of Memory: could not allocate space for the array.\n");
    return 0;
  }

  srand((int)time(NULL));

  struct timespec start, end;

  /* fill vector with random doubles */
  for(i=0; i<local_size; i++){
    v[i] = (double)rand() / (double)(RAND_MAX/10);
  }

  /* assign random double value */
  a = (double)rand() / (double)(RAND_MAX/10);
  if (world_size > 1){
    MPI_Bcast(&a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD); // Make everyone use the SAME a */
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if (world_rank == 0){
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
  }


  /* perform scalar product */
  for(i=0; i<local_size; i++){
    v[i] = a * v[i];
  }

  /* Collect elements on PE 0 */
  if (world_size > 1){
    MPI_Gatherv(v, local_size, MPI_DOUBLE, rbuf, rcounts, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  }

  if (world_rank == 0){
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
  }

  /* print result so compiler does not throw it away */
  if (a==999999){
    printf("Scalar product result: %lf\n", v[0]);
  }

  if (world_rank == 0){
    elapsed_time_hr(start, end, "Double scalar product.");
    free(rbuf);
  }
  free(v);


  return 0;

}

/* compute the Euclidean norm of a float vector      */
/* !!!! naive implementation -- find algorithm that  */
/* !!!! will avoid over/underflow for large vectors  */
int float_norm(unsigned int size){

  int world_size, world_rank;
  MPI_Comm_size(MPI_COMM_WORLD,&world_size);
  MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);

  int i;

  /*
   * Compute size of block each rank will work on
   * We do this in a slightly odd method to try an ensure we have the same number of data plane layers (n)
   * in the MPI case as in the serial case.
   */
  int local_size = 0;
  if (world_rank != 0){
    local_size = size / world_size;
  }
  else if (world_rank == 0){
    local_size = (size / world_size) + (size % world_size);
  }
  else{
    printf("Some error occured in size calculation\n");
  }


  float *v = (float *)malloc(local_size * sizeof(float));
  float sum = 0.0, norm = 0.0;

  if(v == NULL){
    printf("Out Of Memory: could not allocate space for the array.\n");
    return 0;
  }

  srand((int)time(NULL));

  struct timespec start, end;

  /* fill vector with random floats */
  for(i=0; i<local_size; i++){
    v[i] = (float)rand() / (float)(RAND_MAX/10.0);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if (world_rank == 0){
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
  }

  for (i=0; i<local_size; i++){
    sum = sum + (v[i]*v[i]);
  }

  /* REDUCE */
  float global_sum = 0.0;
  MPI_Reduce(&sum, &global_sum, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
  if (world_rank == 0){
    norm = sqrt(sum);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    elapsed_time_hr(start, end, "Float vector norm.");
  }
  /* print result so compiler does not throw it away */

  if (v[0] == 99999){
    printf("Norm = %f\n", norm);
  }

  free(v);
  return 0;
}

/*
 *
 * Compute vector-scalar product
 * AXPY, integers
 *
 * y = a * x + y
 *
 * Naive implementation
 *
 */
int int_axpy(unsigned int size){


  int world_size, world_rank;
  MPI_Comm_size(MPI_COMM_WORLD,&world_size);
  MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);

  int i;

  /*
   * Compute size of block each rank will work on
   * We do this in a slightly odd method to try an ensure we have the same number of data plane layers (n)
   * in the MPI case as in the serial case.
   */
  int local_size = 0;
  int local_size_other = 0;
  int rcounts[world_size];
  int displs[world_size];

  if (world_rank != 0){
    local_size = size / world_size;
  }
  else if (world_rank == 0){
    local_size = (size / world_size) + (size % world_size);
    local_size_other = size / world_size; // Needed for gatherv
    rcounts[0] = local_size;
    displs[0] = 0;
    for (i=1;i<world_size;i++){
      rcounts[i] = local_size_other;
      displs[i] = i*local_size_other;
    }
  }
  else{
    printf("Some error occured in size calculation\n");
  }


  int a;
  int *x = (int *)malloc(local_size * sizeof(int));
  int *y = (int *)malloc(local_size * sizeof(int));

  int* rbuf = NULL;

  /* We only need this space allocated on PE 0 */
  if (world_rank == 0){
    rbuf = (int *)malloc(size * sizeof(int));
    if (rbuf == NULL){
      printf("Out Of Memory: could not allocate space for the array.\n");
      return 0;
    }
  }

  if(x == NULL || y == NULL){
    printf("Out Of Memory: could not allocate space for the two arrays.\n");
    return 0;
  }

  srand((int)time(NULL));

  a = (int)rand() / (int)(RAND_MAX/10);

  /* fill x and y vectors with random ints */

  for(i=0; i<local_size; i++){
    x[i] = (int)rand() / (int)(RAND_MAX/10);
    y[i] = (int)rand() / (int)(RAND_MAX/10);
  }

  struct timespec start, end;
  if (world_size > 1){
    MPI_Bcast(&a, 1, MPI_INT, 0, MPI_COMM_WORLD); // Make everyone use the SAME a */
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if (world_rank == 0){
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
  }

  for(i=0; i<local_size; i++){
    y[i] = a * x[i] + y[i];
  }
  if (world_size > 1){
    MPI_Gatherv(y, local_size, MPI_INT, rbuf, rcounts, displs, MPI_INT, 0, MPI_COMM_WORLD);
  }

  if (world_rank == 0){
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
  }

  /* print result so compiler does not throw it away */
  if (a==999999){
    printf("Scalar product result: %d\n", rbuf[0]);
  }

  if (world_rank == 0){
    elapsed_time_hr(start, end, "Int AXPY.");
    free(rbuf);
  }


  free(x);
  free(y);
  return 0;
}

/*
 *
 * Compute vector-scalar product
 * AXPY, floats
 *
 * y = a * x + y
 *
 * Naive implementation
 *
 */
int float_axpy(unsigned int size){

  int world_size, world_rank;
  MPI_Comm_size(MPI_COMM_WORLD,&world_size);
  MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);

  int i;

  /*
   * Compute size of block each rank will work on
   * We do this in a slightly odd method to try an ensure we have the same number of data plane layers (n)
   * in the MPI case as in the serial case.
   */
  int local_size = 0;
  int local_size_other = 0;
  int rcounts[world_size];
  int displs[world_size];

  if (world_rank != 0){
    local_size = size / world_size;
  }
  else if (world_rank == 0){
    local_size = (size / world_size) + (size % world_size);
    local_size_other = size / world_size; // Needed for gatherv
    rcounts[0] = local_size;
    displs[0] = 0;
    for (i=1;i<world_size;i++){
      rcounts[i] = local_size_other;
      displs[i] = i*local_size_other;
    }
  }
  else{
    printf("Some error occured in size calculation\n");
  }


  float a;
  float *x = (float *)malloc(local_size * sizeof(float));
  float *y = (float *)malloc(local_size * sizeof(float));

  float* rbuf = NULL;

  /* We only need this space allocated on PE 0 */
  if (world_rank == 0){
    rbuf = (float *)malloc(size * sizeof(float));
    if (rbuf == NULL){
      printf("Out Of Memory: could not allocate space for the array.\n");
      return 0;
    }
  }

  if(x == NULL || y == NULL){
    printf("Out Of Memory: could not allocate space for the two arrays.\n");
    return 0;
  }

  srand((int)time(NULL));

  a = (float)rand() / (float)(RAND_MAX/10);

  /* fill x and y vectors with random ints */

  for(i=0; i<local_size; i++){
    x[i] = (float)rand() / (float)(RAND_MAX/10);
    y[i] = (float)rand() / (float)(RAND_MAX/10);
  }

  struct timespec start, end;
  if (world_size > 1){
    MPI_Bcast(&a, 1, MPI_FLOAT, 0, MPI_COMM_WORLD); // Make everyone use the SAME a */
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if (world_rank == 0){
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
  }

  for(i=0; i<local_size; i++){
    y[i] = a * x[i] + y[i];
  }
  if (world_size > 1){
    MPI_Gatherv(y, local_size, MPI_FLOAT, rbuf, rcounts, displs, MPI_FLOAT, 0, MPI_COMM_WORLD);
  }
  if (world_rank == 0){
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
  }

  /* print result so compiler does not throw it away */
  if (a==999999){
    printf("Scalar product result: %f\n", rbuf[0]);
  }

  if (world_rank == 0){
    elapsed_time_hr(start, end, "Float AXPY.");
    free(rbuf);
  }


  free(x);
  free(y);
  return 0;
}

/*
 *
 * Compute vector-scalar product
 * AXPY, doubles
 *
 * y = a * x + y
 *
 * Naive implementation
 *
 */
int double_axpy(unsigned int size){
  int world_size, world_rank;
  MPI_Comm_size(MPI_COMM_WORLD,&world_size);
  MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);

  int i;

  /*
   * Compute size of block each rank will work on
   * We do this in a slightly odd method to try an ensure we have the same number of data plane layers (n)
   * in the MPI case as in the serial case.
   */
  int local_size = 0;
  int local_size_other = 0;
  int rcounts[world_size];
  int displs[world_size];

  if (world_rank != 0){
    local_size = size / world_size;
  }
  else if (world_rank == 0){
    local_size = (size / world_size) + (size % world_size);
    local_size_other = size / world_size; // Needed for gatherv
    rcounts[0] = local_size;
    displs[0] = 0;
    for (i=1;i<world_size;i++){
      rcounts[i] = local_size_other;
      displs[i] = i*local_size_other;
    }
  }
  else{
    printf("Some error occured in size calculation\n");
  }


  double a;
  double *x = (double *)malloc(local_size * sizeof(double));
  double *y = (double *)malloc(local_size * sizeof(double));

  double* rbuf = NULL;

  /* We only need this space allocated on PE 0 */
  if (world_rank == 0){
    rbuf = (double *)malloc(size * sizeof(double));
    if (rbuf == NULL){
      printf("Out Of Memory: could not allocate space for the array.\n");
      return 0;
    }
  }

  if(x == NULL || y == NULL){
    printf("Out Of Memory: could not allocate space for the two arrays.\n");
    return 0;
  }

  srand((int)time(NULL));

  a = (double)rand() / (double)(RAND_MAX/10);

  /* fill x and y vectors with random doubles */

  for(i=0; i<local_size; i++){
    x[i] = (double)rand() / (double)(RAND_MAX/10);
    y[i] = (double)rand() / (double)(RAND_MAX/10);
  }

  struct timespec start, end;
  if (world_size > 1){
    MPI_Bcast(&a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD); // Make everyone use the SAME a */
  }
  if (world_rank == 0){
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
  }

  for(i=0; i<local_size; i++){
    y[i] = a * x[i] + y[i];
  }
  if (world_size > 1){
    MPI_Gatherv(y, local_size, MPI_DOUBLE, rbuf, rcounts, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if (world_rank == 0){
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
  }

  /* print result so compiler does not throw it away */
  if (a==999999){
    printf("Scalar product result: %lf\n", rbuf[0]);
  }

  if (world_rank == 0){
    elapsed_time_hr(start, end, "Double AXPY.");
    free(rbuf);
  }


  free(x);
  free(y);
  return 0;
}

/*
 * Dense Matrix-Vector product, integers
 *
 * y = A * x
 * where A is a square matrix
 *
 * Input:  number of elements in vectors and of rows/cols
 *         in matrix specified as number of ints
 *
 */
int int_dmatvec_product(unsigned int size){

  int world_size, world_rank;
  MPI_Comm_size(MPI_COMM_WORLD,&world_size);
  MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);

  /*
   * Compute size of block each rank will work on
   * We do this in a slightly odd method to try an ensure we have the same number of data plane layers (n)
   * in the MPI case as in the serial case.
   */
  int local_size = 0;
  int local_size_other = 0;
  int rcounts[world_size];
  int displs[world_size];
  int i,j;

  if (world_rank != 0){
    local_size = size / world_size;
  }
  else if (world_rank == 0){
    local_size = (size / world_size) + (size % world_size);
    local_size_other = size / world_size; // Needed for gatherv
    rcounts[0] = local_size;
    displs[0] = 0;
    for (i=1;i<world_size;i++){
      rcounts[i] = local_size_other;
      displs[i] = i*local_size_other;
    }
  }
  else{
    printf("Some error occured in size calculation\n");
  }



  int r1 = 0;
  int r2 = 0;

  /* create two vectors */
  int *x = (int *)malloc(size * sizeof(int));
  int *y = (int *)calloc(local_size, sizeof(int));

  /* create matrix */
  int **A;
  A = (int **)malloc(local_size * sizeof(int *));
  for(i=0; i<local_size; i++){
    A[i] = (int *)malloc(size * sizeof(int));
  }

  if(x == NULL || y == NULL || A == NULL){
    printf("Out Of Memory: could not allocate space for the vectors and matrix.\n");
    return 0;
  }

  int* rbuf = NULL;

  /* We only need this space allocated on PE 0 */
  if (world_rank == 0){
    rbuf = (int *)malloc(size * sizeof(int));
    if (rbuf == NULL){
      printf("Out Of Memory: could not allocate space for the array.\n");
      return 0;
    }
  }

  srand((int)time(NULL));

  struct timespec start, end;

  if (world_rank == 0){
    r1 = (int)rand() / (int)(RAND_MAX/10);
    r2 = (int)rand() / (int)(RAND_MAX/10);
  }

  /* Synchronise the "random" values from 0 to all PEs */
  if (world_size > 1){
    MPI_Bcast(&r1, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&r2, 1, MPI_INT, 0, MPI_COMM_WORLD);
  }

  /* fill matrix A with "random" integer values */
  for(i=0; i<local_size; i++){
    for(j=0; j<size; j++){
      A[i][j] = r2;
    }
  }
  /* fill vector x with "random" integer values */
  for(i=0;i<size;i++){
    x[i] = r1;
  }

  clock_gettime(CLOCK_MONOTONIC_RAW, &start);

  /* perform matrix-vector product */
  for(i=0; i<local_size; i++){
    for(j=0; j<size; j++){
      y[i] = y[i] + A[i][j] * x[j];
    }
  }
  if (world_size > 1){
    MPI_Gatherv(y, local_size, MPI_INT, rbuf, rcounts, displs, MPI_INT, 0, MPI_COMM_WORLD);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if (world_rank == 0){
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    elapsed_time_hr(start, end, "Int Dense Matrix-Vector product.");
    free(rbuf);
  }

  /* print result so compiler does not throw it away */
  if (r1==99999){
    printf("Result vector y[0] = %d\n", y[0]);
  }

  free(x);
  free(y);
  for(i =0; i<local_size; i++) free(A[i]);
  free(A);

  return 0;

}

/*
 * Dense Matrix-Vector product, floats
 *
 * y = A * x
 * where A is a square matrix
 *
 * Input:  number of elements in vectors and of rows/cols
 *         in matrix specified as number of floats
 *
 */
int float_dmatvec_product(unsigned int size){

  int world_size, world_rank;
  MPI_Comm_size(MPI_COMM_WORLD,&world_size);
  MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);

  /*
   * Compute size of block each rank will work on
   * We do this in a slightly odd method to try an ensure we have the same number of data plane layers (n)
   * in the MPI case as in the serial case.
   */
  int local_size = 0;
  int local_size_other = 0;
  int rcounts[world_size];
  int displs[world_size];
  int i,j;

  if (world_rank != 0){
    local_size = size / world_size;
  }
  else if (world_rank == 0){
    local_size = (size / world_size) + (size % world_size);
    local_size_other = size / world_size; // Needed for gatherv
    rcounts[0] = local_size;
    displs[0] = 0;
    for (i=1;i<world_size;i++){
      rcounts[i] = local_size_other;
      displs[i] = i*local_size_other;
    }
  }
  else{
    printf("Some error occured in size calculation\n");
  }



  float r1 = 0;
  float r2 = 0;

  /* create two vectors */
  float *x = (float *)malloc(size * sizeof(float));
  float *y = (float *)calloc(local_size, sizeof(float));

  /* create matrix */
  float **A;
  A = (float **)malloc(local_size * sizeof(float *));
  for(i=0; i<local_size; i++){
    A[i] = (float *)malloc(size * sizeof(float));
  }

  if(x == NULL || y == NULL || A == NULL){
    printf("Out Of Memory: could not allocate space for the vectors and matrix.\n");
    return 0;
  }

  float* rbuf = NULL;

  /* We only need this space allocated on PE 0 */
  if (world_rank == 0){
    rbuf = (float *)malloc(size * sizeof(float));
    if (rbuf == NULL){
      printf("Out Of Memory: could not allocate space for the array.\n");
      return 0;
    }
  }

  srand((int)time(NULL));

  struct timespec start, end;

  if (world_rank == 0){
    r1 = (float)rand() / (float)(RAND_MAX/10);
    r2 = (float)rand() / (float)(RAND_MAX/10);
  }

  /* Synchronise the "random" values from 0 to all PEs */
  if (world_size > 1){
    MPI_Bcast(&r1, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&r2, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
  }
  /* fill matrix A with "random" integer values */
  for(i=0; i<local_size; i++){
    for(j=0; j<size; j++){
      A[i][j] = r2;
    }
  }
  /* fill vector x with "random" integer values */
  for(i=0;i<size;i++){
    x[i] = r1;
  }
  MPI_Barrier(MPI_COMM_WORLD);
  clock_gettime(CLOCK_MONOTONIC_RAW, &start);

  /* perform matrix-vector product */
  for(i=0; i<local_size; i++){
    for(j=0; j<size; j++){
      y[i] = y[i] + A[i][j] * x[j];
    }
  }
  if (world_size > 1){
    MPI_Gatherv(y, local_size, MPI_FLOAT, rbuf, rcounts, displs, MPI_FLOAT, 0, MPI_COMM_WORLD);
  }
  if (world_rank == 0){
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    elapsed_time_hr(start, end, "Float Dense Matrix-Vector product.");
    free(rbuf);
  }

  /* print result so compiler does not throw it away */
  if (r1==99999){
    printf("Result vector y[0] = %f\n", y[0]);
  }

  free(x);
  free(y);
  for(i =0; i<local_size; i++) free(A[i]);
  free(A);
  return 0;

}

/*
 * Dense Matrix-Vector product, doubles
 *
 * y = A * x
 * where A is a square matrix
 *
 * Input:  number of elements in vectors and of rows/cols
 *         in matrix specified as number of floats
 *
 */
int double_dmatvec_product(unsigned int size){

  int world_size, world_rank;
  MPI_Comm_size(MPI_COMM_WORLD,&world_size);
  MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);

  /*
   * Compute size of block each rank will work on
   * We do this in a slightly odd method to try an ensure we have the same number of data plane layers (n)
   * in the MPI case as in the serial case.
   */
  int local_size = 0;
  int local_size_other = 0;
  int rcounts[world_size];
  int displs[world_size];
  int i,j;

  if (world_rank != 0){
    local_size = size / world_size;
  }
  else if (world_rank == 0){
    local_size = (size / world_size) + (size % world_size);
    local_size_other = size / world_size; // Needed for gatherv
    rcounts[0] = local_size;
    displs[0] = 0;
    for (i=1;i<world_size;i++){
      rcounts[i] = local_size_other;
      displs[i] = i*local_size_other;
    }
  }
  else{
    printf("Some error occured in size calculation\n");
  }



  double r1 = 0;
  double r2 = 0;

  /* create two vectors */
  double *x = (double *)malloc(size * sizeof(double));
  double *y = (double *)calloc(local_size, sizeof(double));

  /* create matrix */
  double **A;
  A = (double **)malloc(local_size * sizeof(double *));
  for(i=0; i<local_size; i++){
    A[i] = (double *)malloc(size * sizeof(double));
    if (A[i] == NULL){
      printf("Out Of Memory: could not allocate space for the vectors and matrix.\n");
      return 0;
    }
  }

  if(x == NULL || y == NULL || A == NULL){
    printf("Out Of Memory: could not allocate space for the vectors and matrix.\n");
    return 0;
  }

  double* rbuf = NULL;

  /* We only need this space allocated on PE 0 */
  if (world_rank == 0){
    rbuf = (double *)malloc(size * sizeof(double));
    if (rbuf == NULL){
      printf("Out Of Memory: could not allocate space for the array.\n");
      return 0;
    }
  }

  srand((int)time(NULL));

  struct timespec start, end;

  if (world_rank == 0){
    r1 = (double)rand() / (double)(RAND_MAX/10);
    r2 = (double)rand() / (double)(RAND_MAX/10);
  }

  /* Synchronise the "random" values from 0 to all PEs */
  if (world_size > 1){
    MPI_Bcast(&r1, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&r2, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  }
  /* fill matrix A with "random" integer values */
  for(i=0; i<local_size; i++){
    for(j=0; j<size; j++){
      A[i][j] = r2;
    }
  }
  /* fill vector x with "random" integer values */
  for(i=0;i<size;i++){
    x[i] = r1;
  }
  MPI_Barrier(MPI_COMM_WORLD);
  clock_gettime(CLOCK_MONOTONIC_RAW, &start);
  /* perform matrix-vector product */
  for(i=0; i<local_size; i++){
    for(j=0; j<size; j++){
      y[i] = y[i] + A[i][j] * x[j];
    }
  }
  if (world_size > 1){
    MPI_Gatherv(y, local_size, MPI_DOUBLE, rbuf, rcounts, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  }

  if (world_rank == 0){
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    elapsed_time_hr(start, end, "Double Dense Matrix-Vector product.");
    free(rbuf);
  }

  /* print result so compiler does not throw it away */
  if (r1==99999){
    printf("Result vector y[0] = %lf\n", y[0]);
  }

  free(x);
  free(y);
  for(i =0; i<local_size; i++) free(A[i]);
  free(A);

  return 0;

}

/* int double_spmatvec_product(unsigned long r){ */

/*   //int *m, *n, *nz; */
/*   int m, n, nz; */
/*   int *row_idx, *col_idx; */
/*   double *values; */
/*   double *x,*b; */

/*   struct timespec start,end; */

/*   char *filename = "matrix_in.csr"; */

/*   int i,j,rep; */

/*   FILE *f; */
/*   char line[64]; */

/*   if(r==ULONG_MAX) r=1000; */

/*   if ((f = fopen(filename, "r")) == NULL) { */
/*     printf ("can't open file <%s> \n", filename); */
/*     exit(1); */
/*   } */

/*   fgets(line, sizeof(line), f); */
/*   sscanf(line, "%d %d %d", &nz, &n, &m); */

/*   printf("Number of elements of values and col_idx: %d; number of values in row_idx: %d\n", nz, m); */

/*   row_idx = malloc(m * sizeof(int)); */
/*   col_idx = malloc(n * sizeof(int)); */
/*   values = malloc(nz * sizeof(double)); */
/*   x = malloc((m-1) * sizeof(double)); */
/*   b = malloc((m-1) * sizeof(double)); */

/*   if (!row_idx || !col_idx || !values || !x || !b){ */
/*     printf ("cannot allocate memory for sparse matrix and vectors\n"); */
/*     exit(1); */
/*   } */
/*   else{ */
/*     printf("memory allocated\n"); */
/*   } */

/*   // fill values */
/*   for(i=0;i<nz;i++){ */
/*     fgets(line, sizeof(line), f); */
/*     sscanf(line, "%lf", &values[i]); */
/*   } */

/*   // fill col_idx */
/*   for(i=0;i<nz;i++){ */
/*     fgets(line, sizeof(line), f); */
/*     sscanf(line, "%d", &col_idx[i]); */
/*   } */

/*   // fill row_idx */
/*   for(i=0;i<m;i++){ */
/*     fgets(line, sizeof(line), f); */
/*     sscanf(line, "%d", &row_idx[i]); */
/*   } */

/*   for (i=0; i<m-1; i++)  { */
/*     x[i]=i+1.5; // give basic values to vector x */
/*   } */
/*   rapl_init(); */
/*   rapl_start(); */
/*   clock_gettime(CLOCK_MONOTONIC_RAW, &start); */

/*   for(rep=0;rep<r;rep++){ */
/*     /\* Ax=b *\/ */

/* #pragma omp parallel for private(j) */
/*     for(i=0; i<m-1; i++){ */
/*       for(j=row_idx[i];j<row_idx[i+1];j++){ */
/*         b[i] = b[i] + values[j]* x[col_idx[j]]; */
/*       } */
/*     } */
/*   } */
/*   rapl_end(); */
/*   clock_gettime(CLOCK_MONOTONIC_RAW, &end); */

/*   elapsed_time_hr(start, end, "Sparse DMVs."); */
/*   rapl_deinit(); */
/*   free(x); */
/*   free(b); */
/*   free(row_idx); */
/*   free(col_idx); */
/*   free(values); */

/*   fclose(f); */

/*   return 0; */
/* } */


int double_spmatvec_product(unsigned long r){

  /* MPI_Init(NULL, NULL); */
  int world_size, world_rank;
  MPI_Comm_size(MPI_COMM_WORLD,&world_size);
  MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);
  MPI_Status status;



  /* double *values; */

  /* struct timespec start,end; */

  char *filename = "matrix_in.csr";

  int i,j,rep;

  if (world_rank==0){
    printf("Passed MPI Switch\n");
    int m, n, nz;

    FILE *f;
    char line[64];

    if(r==ULONG_MAX) r=1000;

    if ((f = fopen(filename, "r")) == NULL) {
      printf ("can't open file <%s> \n", filename);
      exit(1);
    }

    fgets(line, sizeof(line), f);
    sscanf(line, "%d %d %d", &nz, &n, &m);

    printf("Number of elements of values and col_idx: %d; number of values in row_idx: %d\n", nz, m);

    /* row_idx = malloc(m * sizeof(int)); */
    /* col_idx = malloc(n * sizeof(int)); */
    /* values = malloc(nz * sizeof(double)); */
    /* x = malloc((m-1) * sizeof(double)); */
    /* b = malloc((m-1) * sizeof(double)); */

    /* if (!row_idx || !col_idx || !values || !x || !b){ */
    /*   printf ("cannot allocate memory for sparse matrix and vectors\n"); */
    /*   exit(1); */
    /* } */
    /* else{ */
    /*   printf("memory allocated\n"); */
    /* } */






    /* From test_csr.c */

    /* double values[nz]; */
    double* values = malloc(nz * sizeof(double));
    int values_len = nz;

    // fill values
    for(i=0;i<nz;i++){
      fgets(line, sizeof(line), f);
      sscanf(line, "%lf", &values[i]);
    }

    /* int row_idx[nz+1]; */
    int* row_idx =  malloc(m * sizeof(int));
    int row_idx_len = m+1;
    // fill row_idx
    for(i=0;i<m;i++){
      fgets(line, sizeof(line), f);
      sscanf(line, "%d", &row_idx[i]);
    }

    /* int col_idx[nz]; */
    int* col_idx = malloc(nz * sizeof(int));
    int col_idx_len = values_len;

    // fill col_idx
    for(i=0;i<nz;i++){
      fgets(line, sizeof(line), f);
      sscanf(line, "%d", &col_idx[i]);
    }


    int x_len = m-1;
    double* x = (double*)malloc(sizeof(double)*x_len);
    /* double x[x_len]; */
    for(i=0;i<x_len;i++){
      x[i] = i+1;
    }

    fclose(f);



    double* b = (double*)malloc(sizeof(double)*x_len);
    memset(b, 0, sizeof(b));


    /*
     * Compute how many members of row_idx to go each rank
     * The last rank (world_size-1) gets the overflow
     */
    int local_row_idx_len_array[world_size];
    for (i=0;i<world_size;i++){
      local_row_idx_len_array[i]=(row_idx_len-1)/world_size;
    }
    local_row_idx_len_array[world_size-1]+=(row_idx_len-1)%world_size;


    /*
     * Send the number of items from row_idx they will receive to each rank
     * For this rank (rank 0), perform a local copy rather than via MPI
     */
    int remote_row_offset = 0;
    for(i=1;i<world_size;i++){
      MPI_Send(&local_row_idx_len_array[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD);
    }
    int local_row_idx_len = local_row_idx_len_array[0];

    /*
     * Send each rank the number of row_idx members it is due.
     * Send each rank the index in row_idx that it's first element came from,
     * this is important is placing the results in the correct place in the b
     * (result) vector.
     * Send each rank the first index that rank+1 should receive to use as a termination condition
     * For this rank (rank 0), perform a local copy rather than via MPI
     */
    for(i=1;i<world_size;i++){
      printf("pre0\n");
      printf("local_row_idx_len_array[i]=%d\n", local_row_idx_len_array[i]);
      MPI_Send(&row_idx[i*local_row_idx_len], local_row_idx_len_array[i], MPI_INT, i, 0, MPI_COMM_WORLD);
      printf("0\n");
      remote_row_offset = i*local_row_idx_len;
      MPI_Send(&remote_row_offset, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
      MPI_Send(&row_idx[(i+1)*local_row_idx_len], 1, MPI_INT, i, 0, MPI_COMM_WORLD);
    }


    int* local_row_idx = (int*)malloc(sizeof(int)*(local_row_idx_len+1));
    if (local_row_idx==NULL){
      printf("NULL\n");
    }else{
      printf("NOT NULL\n");
    }

    /* memcpy(&local_row_idx[0], &row_idx[0], sizeof(int)*local_row_idx_len_array[0]); */
    /* memcpy(local_row_idx, row_idx, sizeof(int)*local_row_idx_len_array[0]); */
    memcpy(&local_row_idx[local_row_idx_len], &row_idx[local_row_idx_len], sizeof(int));

    /*
     * Compute the number of values (members of values) for each rank
     */
    int vals_per_rank[world_size];
    for(i=0;i<world_size;i++){
      vals_per_rank[i] = row_idx[(i+1)*local_row_idx_len] - row_idx[i*local_row_idx_len];
    }

    /*
     * Send to each rank the number of values they will receive
     * Send to each rank the values
     * Send to each rank the column indices (col_idx) which will be equal
     * to the number of values.
     * For this rank (rank 0), perform a local copy rather than via MPI
     */
    int counter = 0;
    counter = vals_per_rank[0];
    for(i=1;i<world_size;i++){
      MPI_Send(&vals_per_rank[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD);
      MPI_Send(&values[counter], vals_per_rank[i], MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
      MPI_Send(&col_idx[counter], vals_per_rank[i], MPI_INT, i, 0, MPI_COMM_WORLD);
      counter += vals_per_rank[i];
    }
    double local_vals[vals_per_rank[0]];
    memcpy(&local_vals[0], &values[0], sizeof(double)*vals_per_rank[0]);
    int local_col_idx[vals_per_rank[0]];
    memcpy(&local_col_idx[0], &col_idx[0], sizeof(int)*vals_per_rank[0]);


    /*
     * Broadcast the length of the vector x and vector x to each rank
     */
    MPI_Bcast(&x_len, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&x[0], x_len, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    int ii, jj;
    int local_row_adjust = 0;

    /* Main algorithm */
    for(ii=0;ii<local_row_idx_len;ii++){
      for(jj=local_row_idx[ii];jj<local_row_idx[ii+1];jj++){
        b[ii+local_row_adjust] = b[ii+local_row_adjust] + (x[local_col_idx[jj]] * local_vals[jj]);
      }
    }

    /*
     * Reduce the b vector on all ranks to bb vector on this rank (rank 0).
     */
    double* bb = (double*)malloc(sizeof(double)*x_len);
    printf("x_len = %d\n", x_len);
    printf("0z\n");
    MPI_Reduce(b, bb, x_len, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    printf("0z\n");

    /* Print result for checking */
    printf("bb ");
    for(i=0;i<x_len;i++){
      printf("%lf ", bb[i]);
    }
    printf("\n");

    /* End from test_csr.c*/

  }
  else{

    printf("Passed MPI Switch\n");
    int local_row_idx_len = 0;
    MPI_Recv(&local_row_idx_len, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    local_row_idx_len++;
    int local_row_idx[local_row_idx_len];
    MPI_Recv(&local_row_idx[0], local_row_idx_len, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    int local_row_adjust = 0;

    MPI_Recv(&local_row_adjust, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    MPI_Recv(&local_row_idx[local_row_idx_len-1], local_row_idx_len, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    int lr_adjust = local_row_idx[0];
    for(i=0;i<local_row_idx_len;i++){
      local_row_idx[i] = local_row_idx[i] - lr_adjust;
    }

    int local_vals_len = 0;

    MPI_Recv(&local_vals_len, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

    double* local_vals = (double*)malloc(sizeof(double)*local_vals_len);
    /* double local_vals[local_vals_len]; */

    MPI_Recv(&local_vals[0], local_vals_len, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);

    int* local_col_idx = (int*)malloc(sizeof(int)*local_vals_len);
    /* int local_col_idx[local_vals_len]; */

    MPI_Recv(&local_col_idx[0], local_vals_len, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

    int x_len; // Length of the vector in SpMV
    MPI_Bcast(&x_len, 1, MPI_INT, 0, MPI_COMM_WORLD);

    /* double x[x_len]; */
    double* x = (double*)malloc(sizeof(double)*x_len);
    double* b = (double*)malloc(sizeof(double)*x_len);
    printf("x_len = %d\n", x_len);
    MPI_Bcast(&x[0], x_len, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    memset(b, 0, sizeof(b));
    int ii, jj;
    /* Main processing loop */
    for(ii=0;ii<local_row_idx_len-1;ii++){
      for(jj=local_row_idx[ii];jj<local_row_idx[ii+1];jj++){
        b[ii+local_row_adjust] = b[ii+local_row_adjust] + (x[local_col_idx[jj]] * local_vals[jj]);
      }
    }
    printf("1\n");
    MPI_Reduce(b, NULL, x_len, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    printf("1\n");
  }

  MPI_Barrier(MPI_COMM_WORLD);
  /* MPI_Finalize(); */

  /* rapl_init(); */
  /* rapl_start(); */
  /* clock_gettime(CLOCK_MONOTONIC_RAW, &start); */

  /* for(rep=0;rep<r;rep++){ */
  /* } */

  /* rapl_end(); */
  /* clock_gettime(CLOCK_MONOTONIC_RAW, &end); */

  /* elapsed_time_hr(start, end, "Sparse DMVs."); */
  /* rapl_deinit(); */


  /* fclose(f); */

  return 0;
}
