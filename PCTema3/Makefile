build: request.cpp utils.cpp 
	g++ -Wall -c utils.cpp
	g++ -Wall -c request.cpp
	g++ -Wall -c client.cpp
	gcc -Wall -c parson.c
	g++ -Wall parson.o client.o utils.o request.o -o client 

run: client
	./client

clean:
	rm *.o
	rm client