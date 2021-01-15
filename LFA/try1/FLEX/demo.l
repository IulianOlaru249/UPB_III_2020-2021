%{
    #include <stdio.h>
    #include <string.h>
    int a = 10;
    int states_counter = 0;
    char name[50];
%}

letter [A-Za-z]
alphanum [A-Za-z0-9]

%option noyywrap

%x TYPE ALPHABET TEMP VARIABLES ALPHABET_G NON_TERM ELEMENT AUTOMATA_ELEMENT
%x AUTOMATA_STATES INITIAL_STATE

%%
<INITIAL>{alphanum}+ {
    strcpy(name, yytext);
    BEGIN(TYPE);
}

<TYPE>"Grammar" {
    states_counter = 0;
    printf("Numele gramaticii: %s\n", name);
    BEGIN(ELEMENT);
}

<ELEMENT>"alphabet" {
    BEGIN(ALPHABET_G);
}

<ELEMENT>"nonterminals" {
    BEGIN(NON_TERM);
}

<ELEMENT>{letter}+ {
    BEGIN(TEMP);
}

<ALPHABET_G>"{"[^}]+"}" {
    printf("Alfabetul gramaticii : %s\n", yytext);
    BEGIN(TEMP);
}

<NON_TERM>"{"[^}]+"}" {
    BEGIN(ELEMENT);
}

<TYPE>"PushDownAutomata" {
    printf("Numele automatului: %s \n", name);
    printf("Tipul automatului: %s \n", yytext);
    BEGIN(ALPHABET);
}

<ALPHABET>"{"[^}]+"}" {
    printf("Alfabetul de intrare : %s\n", yytext);
    BEGIN(AUTOMATA_ELEMENT);
}

<AUTOMATA_ELEMENT>"states" {
    BEGIN(AUTOMATA_STATES);
}

<AUTOMATA_ELEMENT>"initial_state" {
    BEGIN(INITIAL_STATE);
}

<AUTOMATA_STATES>{alphanum}{alphanum} {
    states_counter++;
}

<AUTOMATA_STATES>"}" {
    printf("Numarul de stari: %d\n", states_counter);
    BEGIN(AUTOMATA_ELEMENT);
}

<INITIAL_STATE>{alphanum}{alphanum} {
    printf("Starea initiala: %s\n", yytext);
    BEGIN(AUTOMATA_ELEMENT);
}

<AUTOMATA_ELEMENT>";;" {
    printf("\n");
    BEGIN(INITIAL);
}

<TEMP>";;" {
    printf("\n");
    BEGIN(INITIAL);
}

<INITIAL>.|\n {}
<TYPE>.|\n {}
<ALPHABET>.|\n {}
<ALPHABET_G>.|\n {}
<TEMP>.|\n {}
<VARIABLES>.|\n {}
<ELEMENT>.|\n {}
<AUTOMATA_ELEMENT>.|\n {}
<AUTOMATA_STATES>.|\n {}
<NON_TERM>.|\n {}
<INITIAL_STATE>.|\n {}
%%

int main(int argc, char *argv[]){
    yylex();  
    return 0;
}
