Butmalai Dan 333CC

Descriere rezolvare:
Pentru fiecare linie care contine cuvantul "Obiect" vom selecta tot textul din linia data.
Acum ca avem denumirile obiectelor selectate se pune problema: Cum aflam parintele si adancimea 
la care se afla obiectul in arbore?
Pentru a rezolva cerinta vom numara cate caractere "{" (paranteze deschise si neinchise) am intalnit pana acum [variabila open].
La intalnirea unui caracter "}" vom scadea numarul de (paranteze deschise si neinchise). 
Astfel aceasta variabila [open] ne va comunica adancimea la care se afla un nod in arbore. 
Parintele nodului curent pentru afisare nu poarta nici un rol, putem afisa x '\t', unde x e adancimea nodului.
Pentru a afla daca un nod  e complex sau nu vom verifica daca nivelul succesorului sau e mai mare decat nivelul curent.

Detalii de implementare pot fi regasite in cod "TemaC.l".

Pentru Rulare:
make run va builda si rula executabilul pentru toate inputurile atasate in arhiva