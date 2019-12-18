build:
	mpic++ tema3.cpp -o tema3
run: 
	make build
	mpirun -np 4 ./tema3 darth-vader.pgm out.pgm blur
	mpirun -np 4 ./tema3 darth-vader.pgm out1.pgm smooth
	mpirun -np 4 ./tema3 darth-vader.pgm out2.pgm mean
	mpirun -np 4 ./tema3 darth-vader.pgm out3.pgm emboss
	mpirun -np 4 ./tema3 darth-vader.pgm out3.pgm sharpen