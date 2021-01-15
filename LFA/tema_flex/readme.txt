Olaru Gabriel Iulian
334 CC
Varianta A


*******************************************************************************
                    Mod de rulare
*******************************************************************************

Rulare teste automat:
    ./check.sh

Rulare manuala:
    make; ./hw < tests/file.txt > out/file.out; make clean;


*******************************************************************************
                    Suita de teste
*******************************************************************************

Testele se gasesc in directorul tests/ iar referintele in directorul ref/ .

Suita te deste:

    - automata:
        Un exemplu de automat.

    - multiple automata:
        Mai multe exemple de automate cu stiva.

    - grammar:
        Un exemplu de gramatica fara restrictii.

    - multiple grammar:
        Mai multe exemple de gramatici, dupa cum urmeaza:
            * Gramatica Regulata
            * Gramatica Independenta de Context
            * Gramatica Gramatica Dependenta de Context
            * Gramatica Fara Restrictii

    - commentaries:
        Un exemplu de gramatica regulata si automat insotite de comentarii
        pe o singura linie si comentarii pe mai multe linii.

    - variables:
        Mai multe exemple de gramatici si un automat insotide
        de definitii de variabile

    - grammar_and_automata:
        Mai multe exemple de gramatici si automate, dupa cum urmeaza:
            * Gramatica Regulata
            * Automat
            * Gramatica Dependenta de Context
            * Automat

    - grammar_automata_commentaries_variables:
        Mai multe exemple de gramatici si automate, insotite de 
        comentarii pe o singura linie sau pe mia multe linii si
        definitii de variabile dupa cum urmeaza:
            * Gramatica Regulata
            * Automat
            * Gramatica Dependenta de Context
            * Automat


*******************************************************************************
                    Flow-ul Programului
*******************************************************************************
    
    *incepe din starea initial unde se uita dupa comentarii 
        single-line/ multi-line si le ignora. Se uita apoi dupa primul cuvant
        gasit. Acesta este fie numele unui automat fie al unei gramatici.
    
    * urmeaza sa se uite fie dupa cuvantul cheie PushDownAutomata fie dupa
        Grammer. In functie de ce gasete incepe o verificare corespunzatoare.

    *Pentru gramatica:
        - se uita dupa alfabet si il retine
        - ignora comentariile
        - cicleaza regula cu regula (delimitate de ; ):
            - se uita in stanga sagetii dupa & pentru a decide daca are unul
            sau mai multi terminali/ neterminali. In functie de asta de decide
            daca se face o verificare pentru GR si GIC sau pentru GDC si GFR
            - retine minimul lungimilor subsecventelor din dreapta sagetii
            - retine lungimea subsecventei din stanga
            - retine daca a aparut o litera mare (neterminal) si daca dupa el
            mai apar si alte litere pentru a determina daca e sau nu GR.
            - compara lungimile retinute si verifica daca este GDC sau nu.
        - la finalul regulilor (marcat de ;;), in functie de ce conditii au
            fost intalnite, decide tipul gramaticii
        - printeaza tipul gramaticii si alfabetul

    *Pentru automate:
        - se uita dupa alfabet si il retine
        - se uita dupa states, le numara si le retine
        - se uita dupa final si initial state si le retine
        - printeaza informatiile

        - Cicleaza regula cu regula:
            - parcurge regula a.i. sa retina fiecare termen al ecuatiei intr-o
                structura. 1 regula = o ecuatie;
            - la finalul unei reguli afiseaza ecuatia corespunzatoare.

