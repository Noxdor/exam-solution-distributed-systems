#ifndef __STEPS_H
#define __STEPS_H
#include <mpi.h>
#include <stdint.h>

int32_t calculate_row_and_column_length(int32_t size, int32_t rank_world);
MPI_Comm create_cart_topology(int32_t size);
MPI_Comm create_row_groups(MPI_Comm cart_comm, int32_t rank_cart, int32_t n);
int32_t generate_random_number(int32_t rank);
int32_t find_max(MPI_Comm comm_row, int32_t rank_row, int32_t val);
int32_t sum_row(MPI_Comm comm_row, int32_t rank_row_max, int32_t value);
void send_and_print(MPI_Comm cart_comm, int32_t rank_row_max, int32_t rank_row,
                    int32_t row_sum, int32_t coords[2]);

#endif // __STEPS_H
