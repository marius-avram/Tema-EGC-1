AVRAM Marius 334CB

TEMA 1 EGC: Fotbal static 

Toate cerintele din enunt au fost indeplinte. In plus fata de acestea: 
- am realizat un model ceva mai complex al jucatorilor adaugandu-le brate, 
- am facut ca portarii ambelor echipe sa fie translatabili la orice moment 
de timp pentru a permite o aparare cat mai buna a propriei porti,
- am realizat 2 tribune de spectatori, fiecare spectator imbracat intr-o 
nunata de culoare al echipei sale favorite. Atunci cand echipa lor favorita
marcheaza spectatorii se bucura si isi schimba culorile. 
- controale separate pentru fiecare echipa.
- framerate stabil indiferent de plaforma.

Controalele pentru echipa albastra sunt urmatoarele: 
- 'z' si 'c' pentru miscarea portarului stanga dreapta 
- 's' si 'x' pentru rotirea jucatorului ce detine mingea (daca o detine) in 
sens invers trigonometric si in sens trigonometric 
- 'a' pentru a suta 

Controlalele pentru echipa galbena sunt urmatoare: 
- '<-' si '->' pentru miscarea portarului stanga dreapta 
- sageata sus si sageata jos pentru rotirea jucatorului ce detine mingea 
(daca o detine) in sens invers trigonometric si in sens trigonometric 
- 'Enter' pentru a suta 

Am decis sa nu fac translatabili toti jucatorii pentru ca ar fi stricat mult
din experienta de joc, avand in vedere ca nu exista o inteligenta artificiala 
care sa rearanjeze jucatorii la pozitia lor initiala dupa ce au fost mutati 
de pe tabla. 


Detalii de implementare: 

Am creat 3 clase: 
Circle -  pentru a desena un cerc sau un jucator. Aceasta clasa
poate adauga unui cerc si un fel de maini/membre astfel el devine un jucator. 
Field - deseneaza toate elementele statice ale terenului precum gazonul, 
manta, portile, stalpii portilor si tribunele. 
Game - se ocupa de elementele dinamice/animate ale jocului si de logica 
din spate a acestuia. 

Nu prea am folosit multe elemente de POO desi ar fi existat locuri in care 
as fi putut profita de acestea. Mai multe detalii despre aceste clase se 
regasesc in comentariile din cod (fiecare metoda are explicatia aferenta). 
Am decis sa realizez comentariile in engleza pentru a nu fi constant in 
limba folosita pentru variabile si comentarii.

Compilare: 

Se deschide Tema1.sln cu Visual Studio 2008 si se compileaza in versiunea 
Debug sau Release. Se recomanda compilarea in versiunea Release. Proiectul 
a fost dezvoltat in Microsoft Visual Studio C++ Express 2008.
 