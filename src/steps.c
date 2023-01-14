#include "steps.h"
#include <math.h>
#include <mpi.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int32_t calculate_row_and_column_length(int32_t size, int32_t rank_world) {
  double nd = sqrt(size);
  if (nd - (int32_t)nd != -0.00L) {
    if (rank_world == 0)
      printf("\nWith %d it is not possible to create a cartesian topology. "
             "Shutting down the application.\n\n",
             size);

    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
  }

  return (int32_t)nd;
}

/* Creates a cartesian topology, meaning a grid of n x n dimensions.
 * n**2 is here the passed size of the communicator, meaning the total
 * number of processes.
 */
MPI_Comm create_cart_topology(int32_t n) {

  int32_t dims[2] = {n, n}, periods[2] = {1, 1};
  bool reorder = false;
  MPI_Comm cart_comm;

  MPI_Cart_create(MPI_COMM_WORLD, n, dims, periods, reorder, &cart_comm);

  return cart_comm;
}

/* Creates a communicator for each row of the cartesian topology and assigns
 * processes to the right row communicator.
 */
MPI_Comm create_row_groups(MPI_Comm cart_comm, int32_t rank_cart, int32_t n) {
  // calculate the row to which the process belongs
  int32_t row = rank_cart / n;

  MPI_Comm row_comm;

  // split it into rows and create new communicators for each row
  MPI_Comm_split(cart_comm, row, rank_cart, &row_comm);

  return row_comm;
}

/* Generates a random number using rand() function of stdlib. */
int32_t generate_random_number(int32_t rank) {
  // set seed and add rank to generate different random numbers
  srand(time(NULL) + rank);

  // non uniform distribution, not required
  return rand() % 100;
}

int32_t find_max(MPI_Comm comm_row, int32_t rank_row, int32_t val) {
  int32_t send_buf[2] = {val, rank_row};
  int32_t recv_buf[2];

  MPI_Allreduce(send_buf, recv_buf, 1, MPI_2INT, MPI_MAXLOC, comm_row);
  // return the rank_row of the process with the highest random value
  return recv_buf[1];
}

int32_t sum_row(MPI_Comm comm_row, int32_t rank_row_max, int32_t value) {
  int32_t sum = 0;

  MPI_Reduce(&value, &sum, 1, MPI_INT, MPI_SUM, rank_row_max, comm_row);

  return sum;
}

/* Sends the summed result to the right process if not there yet and prints it.
 * The results have to be on the diagonal of the cartesian topology.
 *
 */
void send_and_print(MPI_Comm cart_comm, int32_t rank_row_max, int32_t rank_row,
                    int32_t row_sum, int32_t coords[2]) {
  // is the summing process but not on diagonal of cartesian topology ...
  if (rank_row_max == rank_row && coords[0] != coords[1]) {
    int32_t target_rank, target_coords[2] = {coords[0], coords[0]};
    // ... find cartesian rank of diagonal process of the same row and send
    // it the sum.
    MPI_Cart_rank(cart_comm, target_coords, &target_rank);
    MPI_Send(&row_sum, 1, MPI_INT, target_rank, 0, cart_comm);
    // is the summing process and on the diagonal of cartesian topology ...
  } else if (rank_row_max == rank_row && coords[0] == coords[1]) {

    // ... print result
    printf("Process at (row %d) and (column %d) has sum = %d\n", coords[0],
           coords[1], row_sum);
  }

  // is not the summing process but on the diagonal of cartesian topology ...
  else if (rank_row_max != rank_row && coords[0] == coords[1]) {
    // ... receive summation result and print it
    MPI_Recv(&row_sum, 1, MPI_INT, MPI_ANY_SOURCE, 0, cart_comm, NULL);
    printf("Process at (row %d) and (column %d) has sum = %d\n", coords[0],
           coords[1], row_sum);
  }
}
