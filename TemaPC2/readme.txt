~Tema2PC~
        Realizat de: Butamlai Dan 323CC
        Timp de rezolvare: 8-9h
        Rezolvat: Integral

--- server.cpp ---

Contine clasa ce rezolva functionalitatea unui server descris de cerinta din tema.

Descriere scurta:
        Serverul conine un map ce pastreaza datele relevante pentru cleinti.
        In momentul in care este primit un mesaj de la un clinet tcp sau udp
        datele continute in obiectul server sunt updatate corespunzator.
        Dupa updatarea dateleor se verifica daca serverul contine mesaje ce trebuiesc
        epediate clientilor udp, itemurile din vector sunt golite dupa expedierea mesajului.

--- client.cpp ---

Contine clasa ce rezolva functionalitatea unui client tcp descris de cerinta din tema.

Descriere scurta:
        Clientul se conecteaza la portul specificat in argv.
        Dupa realizare conexiunii expediaza id-ul primit in argv.
        In final clientul asteapta mesaje de la stdin si server,
        executand update-urile si afisarile necesare. 

Fiecare functie contine o mica descriptie a functionalitatii indeplinite.
Codul mai dubios contine comentarii aditionale.

--- Makefile ---
make / make build - creaza executabilele server subscriber
make clean - sterge executabilele si fisierele *.o