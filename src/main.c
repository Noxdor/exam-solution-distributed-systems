#include "steps.h"
#include <assert.h>
#include <math.h>
#include <mpi.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  int32_t rank_world, rank_cart, rank_row, size, n, coords[2], randn,
      rank_row_max, row_sum;

  // Cartesian and row communicators
  MPI_Comm cart_comm, row_comm;

  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank_world);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // possible failure point, if sqrt(size) is not a natural number
  n = calculate_row_and_column_length(size, rank_world);

  // create a N x N cartesian topology
  cart_comm = create_cart_topology(n);

  // get rank and position of this process in cart communicator
  MPI_Comm_rank(cart_comm, &rank_cart);
  MPI_Cart_coords(cart_comm, rank_cart, n, coords);

  // get a random number between 0 and 100
  randn = generate_random_number(rank_world);

  // generate group communicators for each row
  row_comm = create_row_groups(cart_comm, rank_cart, n);
  MPI_Comm_rank(row_comm, &rank_row);

  printf("In row %d process %d has value = %d.\n", coords[0], rank_row, randn);

  // find the process in each row with the maximum value
  rank_row_max = find_max(row_comm, rank_row, randn);

  // sum up values of each row
  row_sum = sum_row(row_comm, rank_row_max, randn);

  // make sure that summation results are on the right process and print result
  send_and_print(cart_comm, rank_row_max, rank_row, row_sum, coords);

  return MPI_Finalize();
}
