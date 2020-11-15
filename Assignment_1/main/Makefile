all: matrix_creator shortest_path

matrix_creator: matrix_creator.c
	mpicc -o matrix_creator matrix_creator.c -lm

shortest_path: shortest_path.c
	mpicc -o shortest_path shortest_path.c -lm

clean:
	rm shortest_path matrix_creator
