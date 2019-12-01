build: request.cpp utils.cpp 
	g++ -Wall -g -c utils.cpp
	g++ -Wall -g -c request.cpp
	g++ -Wall -g -c client.cpp
	gcc -Wall -g -c parson.c
	g++ -Wall -g parson.o client.o utils.o request.o -o client 

run: client
	./client

clean:
	rm *.o
	rm client