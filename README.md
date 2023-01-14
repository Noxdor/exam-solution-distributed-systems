# Exam Solution "Distributed Systems"

The solution to my distributed systems exam at Università degli Studi della Campania "Luigi Vanvitelli" about MPI (Message Passing Interface).

## The task

### English version
> To write an MPI program (with the number of processes `nproc = NxN`) that do the following:
>  1. Form a bidimensional topology of dimensions NxN.
>  2. Every process generates a random number in [0, 100].
>  3. The processes with the highest random number of each row sum up the random values of their row and send the result to the process on the
>  diagonal of the NxN cartesian topology matrix.
  
### Italian version
> Scrivere un programma MPI (con un numero di processi `nproc = NxN`) che effettua le seguenti operazioni:
>  1. Realizzare una topologia bidimensionale NxN.
>  2. Ciascun processo genera un numero casuale minore di 100.
>  3. I processi che, nella propria riga, hanno estratto il numero maggiore, effettuano la somma degli interi della propria riga e inviano la somma
>  all'elemento della propria riga posizionato sulla diagonale principale.
  
## The solution
  
The program had to be written in C.

My solution includes a `main.c` that calls functions handling smaller subtasks. The subtasks are implemented in `steps.c` and defined in `steps.h`.
The solution can be build with [just](https://github.com/casey/just). If `just` is installed, just run `just` in your terminal inside the directory to
build and run the application. The number of processes spawned can be adjusted by running `just run <number-of-processes>`.

If just is not available, you can build and run the project by running:
```bash
gcc -o out/exam -I ./src src/main.c src/steps.c -lmpi -lm
mpirun -n <number-of-processes> ./out/exam
```
