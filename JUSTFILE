
run: build
  mpirun -n 4 ./out/exam

build:
  gcc -o out/exam -I ./src src/main.c src/steps.c -lmpi -lm
