%{
	// Olaru Gabriel Iulian
	// 334CC
	// Varianta A

	#include <stdio.h>
	#include <string.h>

	// Retine termenii ecuatiei automatului
	// pentru a putea fi printati mai usor
	typedef struct _equation {
		char left_x[5];
		char left_y[5];
		char left_z[5];

		char right_x[5];
		char right_y[10];
	} equation;

	// Retine numele autmatului / gramaticii
	char name[50];

	// Retine alfabetul gramaticii
	char grammar_alphabet[100];

	// Retine informatii despre state-urile automatului
	int number_of_states = 0;
	int found_count_states = 0;
	int found_initial_state = 0;
	int found_final_state = 0;
	int total_states = 0;

	//Retine stringuri de subreguli ale automatelor
	equation subrules[100];

	//Retine informatii despre gramatici pentru a determina tipul
	int left_side_len;
	int seq_len = 0;
	int min_right_len = 99999;
	int capital_letter = 0;
	int is_GR = 1;
	int is_GIC = 1;
	int is_GDC = 1;
	
%}

%option noyywrap

letter [a-zA-Z]
alphanum [a-zA-Z0-9]

%x TYPE ALPHABET_AUTOMATA TERMINALS STATES COUNT_STATES DONE_AUTOMATA
%x PRINT_INITIAL_STATE PRINT_FINAL_STATE
%x DONE_GRAMMAR ALPHABET_GRAMMAR GET_RULES GET_SUBRULES
%x GET_SUBRULES_LEFT_Y GET_SUBRULES_LEFT_X GET_SUBRULES_RIGHT_Z
%x CHECK_GRAMMAR CHECK_RULES GOTO_NEWLINE


%%

<INITIAL>"variable "[a-z]" ::= ""{"[a-z,? ]+"}" {
	// stare care se uita dupa definitia variabilelor
	BEGIN(INITIAL);
}

<INITIAL>"%" {
	// stare care se uita dupa comentarii pe o singura linie
	BEGIN(GOTO_NEWLINE);
}

<GOTO_NEWLINE>"\n" {
	// stare care sare peste comentariile pe o singura linie
	BEGIN(INITIAL);
}

<INITIAL>("/*"\n?(.*?)\n?"*/") {
	// stare care se uita dupa comentariile pe mai multe 
	// linii si le ignora
	BEGIN(INITIAL);
}

<INITIAL>{alphanum}+ {
	// stare care se uita dupa orice alfanumeric
	// folosit pentru a retine numele gramaticii / automatului
	strcpy(name, yytext);
	BEGIN(TYPE);
}

<DONE_GRAMMAR>";;" {
	// stare care se uita dupa finalul procesarii unei gramatici
	// afiseaza alfabetul si reseteaza flag-urile.
	printf("Alfabetul gramaticii: %s \n", grammar_alphabet);
	printf("\n");

	is_GR = 1;
	is_GIC = 1;
	is_GDC = 1;

	BEGIN(INITIAL);
}

<DONE_AUTOMATA>";;" {
	// stare care se uita dupa finalul procesarii unui automat
	// reseteaza flag-urile.
	printf("\n");

	found_count_states = 0;
	found_initial_state = 0;
	found_final_state = 0;

	number_of_states = 0;
	total_states = 0;

	BEGIN(INITIAL);
}

<TYPE>"Grammar" {
	// stare care se uita dupa o gramatica
	// afiseaza numele si porneste verificarea
	printf("Numele gramaticii: %s\n", name);
	BEGIN(ALPHABET_GRAMMAR);
}

<ALPHABET_GRAMMAR>"{"[a-z,? ]+"}" {
	// stare care cauta alfabetul gramaticii
	// il retine pentru a fi prnitat mai tarziu
	strcpy(grammar_alphabet, yytext);
	BEGIN(CHECK_GRAMMAR);
}

<ALPHABET_GRAMMAR>"%"(.*?)$ {
	// stare care ignora posibile definitii de alfabete comentate
	BEGIN(ALPHABET_GRAMMAR);
}


<CHECK_GRAMMAR>";\n\n" {
	// stare care se uita dupa incputul regulilor gramaticii (produciile)
	BEGIN(CHECK_RULES);
}

<CHECK_RULES>"\t"([a-zA-Z0-9])*" " {
	// stare care determina daca este GR sau GIC
	// verificand daca in stanga e doar un neterminal
	left_side_len = 1;	
	BEGIN(CHECK_RULES);
}

<CHECK_RULES>"\t"([a-zA-Z0-9]&?)*" " {
	// stare care determina daca este GDC sau GFR
	// verificand daca in stanga sunt mai multi neterminali/ terminali

	left_side_len = (yyleng - 2) / 2 + 1;
	is_GR = 0;
	is_GIC = 0;
	BEGIN(CHECK_RULES);
}

<CHECK_RULES>[a-zA-Z][0-9]? {
	// stare care parcurge membrul din dreapta (dupa ->)
	// element cu element si numara lungimea secventelor cuprinse intre "|"
	// tine cont daca a fost gasita o litera mare (neterminal).
	// verifica daca mai exista orice alt terminal/neterminal dupa un neterminal.

	if (capital_letter) {
		is_GR = 0;
	}
	
	int ascii_val = (int) yytext[0];

	if(ascii_val >= 65 && ascii_val <= 90) {
		capital_letter = 1;
	}

	seq_len++;

	BEGIN(CHECK_RULES);
}

<CHECK_RULES>" | " {
	// stare care reseteaza lungimea secventelor cuprinse intre '|'
	// reseteaza flagurile
	
	if (seq_len < min_right_len)
		min_right_len = seq_len;
	seq_len = 0;
	capital_letter = 0;

	BEGIN(CHECK_RULES);
}

<CHECK_RULES>";" {
	// stare care se uita dupa terminarea unei reguli a gramaticii
	// retine lungimea minima pentru a verifica GDC vs GFR
	// reseteaza flagurile

	if (seq_len < min_right_len)
		min_right_len = seq_len;
	seq_len = 0;
	capital_letter = 0;

	if (left_side_len > min_right_len)
		is_GDC = 0;

	min_right_len = 99999;

	BEGIN(CHECK_RULES);
}

<CHECK_RULES>")" {
	// stare care verifica incheierea unei gramatici si decide tipul exista

	if (is_GR) {
		printf("Tipul gramaticii: GR\n");
	} else if (is_GIC){
		printf("Tipul gramaticii: GIC\n");
	} else if (is_GDC) {
		printf("Tipul gramaticii: GDC\n");
	} else {
		printf("Tipul gramaticii: GFR\n");
	}

	BEGIN(DONE_GRAMMAR);
}


<TYPE>"PushDownAutomata" {
	// stare care se uita dupa un automat
	// afiseaza numele si porneste verificarea

	printf("Numele automatului: %s\n", name);
	printf("Tipul automatului: automat cu stiva\n");
	BEGIN(ALPHABET_AUTOMATA);
}

<ALPHABET_AUTOMATA>"{"[a-z,? ]+"}" {
	// stare care cauta alfabetul gramaticii
	// il retine pentru a fi prnitat mai tarziu
	printf("Alfabetul de intrare: %s \n", yytext);
	BEGIN(STATES);
}

<STATES>states {
	// stare care se uita dupa states si incepe numaratoarea lor
	BEGIN(COUNT_STATES);
}

<STATES>initial_state {
	// stare care se uita dupa initial state
	BEGIN(PRINT_INITIAL_STATE);
}

<STATES>final_states {
	// stare care se uita dupa final state
	BEGIN(PRINT_FINAL_STATE);
}

<STATES>";" {
	// stare care se uita dupa terminarea definitiilor de states
	if (found_count_states && found_initial_state && found_final_state) {
		printf("Functia de tranzitie:\n");
		BEGIN(GET_RULES);
	}
}

<STATES>"->" {
	// stare folosita pentru a verifica daca a fost definit sau nu
	// campul de final states

	printf("Starile finalae: {}\n");
	printf("Functia de tranzitie:\n");
	BEGIN(GET_RULES);
}

<PRINT_FINAL_STATE>"{"[a-z0-9, ]+"}" {
	// stare care printeaza starile finale

	printf("Starile finalae: %s\n", yytext);
	found_final_state = 1;
	BEGIN(STATES);
}

<PRINT_INITIAL_STATE>{alphanum}{alphanum} {
	// stare care printeaza starile initiale

	printf("Starea initiala: %s\n", yytext);
	found_initial_state = 1;
	BEGIN(STATES);
}

<COUNT_STATES>{alphanum}{alphanum} {
	// stare care numara states-urile uni automats
	// si retine states-urile pentru a fi printate mai tarziu

	strcpy( subrules[number_of_states].left_x, yytext);
	number_of_states ++;
	BEGIN(COUNT_STATES);
}

<COUNT_STATES>"}" {
	// stare care printeaza numarul de states

	printf("Numarul de stari: %d\n", number_of_states);
	found_count_states = 1;
	total_states = number_of_states;
	BEGIN(STATES);
}

<GET_RULES>"(" {
	// stare care se uita dupa regulile automatului

	BEGIN(GET_SUBRULES);
}

<GET_SUBRULES>[a-z]"," {
	// stare care retine un membru al ecuatiei
	
	strcpy( subrules[number_of_states].left_y, yytext);
	BEGIN(GET_SUBRULES_RIGHT_Z);
}

<GET_SUBRULES>"-> " {
	//stare care se uita in partea dreapta a regulii
	
	BEGIN(GET_SUBRULES_LEFT_X);
}

<GET_SUBRULES>"/ " {
	//stare care se uita in partea stanga, al doilea termen al regulii

	BEGIN(GET_SUBRULES_LEFT_Y);
}

<GET_SUBRULES_RIGHT_Z>[a-zA-Z0-9][0-9]? {
	// stare care retine un membru al ecuatiei

	strcpy( subrules[number_of_states].left_z, yytext);
	BEGIN(GET_SUBRULES);
}

<GET_SUBRULES_LEFT_Y>([a-zA-Z0-9]&?)+ {
	// stare care retine un membru al ecuatiei

	strcpy( subrules[number_of_states].right_y, yytext);
	BEGIN(GET_SUBRULES);
}

<GET_SUBRULES_LEFT_X>[a-zA-Z][0-9] {
	// stare care retine un membru al ecuatiei

	strcpy( subrules[number_of_states].right_x, yytext);
	BEGIN(GET_SUBRULES);
}

<GET_SUBRULES>";" {
	// stare care se uita dupa finalul unei reguli (ecuatii)
	// afiseaza ecuatia

	printf("\tm(%s, %s %s) = (%s, %s)\n",
		subrules[total_states - number_of_states].left_x,
		subrules[number_of_states].left_y,
		subrules[number_of_states].left_z,
		subrules[number_of_states].right_x,
		subrules[number_of_states].right_y
	);
	BEGIN(GET_SUBRULES);
}

<GET_SUBRULES>") ;" {
	// stare care se uita dupa finalul tuturor regulilor (ecuatiilor)

	number_of_states --;

	if (number_of_states == 0) {
		BEGIN(DONE_AUTOMATA);
	} else {
		BEGIN(GET_RULES);
	}
}

<TERMINALS>"{"[A-Z0-9, ]+"}" {
	// stare care se uita dupa terminalii automatului

	printf("Terminalii automatului: %s \n", yytext);
	BEGIN(INITIAL);
}

<INITIAL>.|\n {}
<DONE_AUTOMATA>.|\n {}
<DONE_GRAMMAR>.|\n {}
<TYPE>.|\n {}
<ALPHABET_AUTOMATA>.|\n {}
<ALPHABET_GRAMMAR>.|\n {}
<TERMINALS>.|\n {}
<STATES>.|\n {}
<COUNT_STATES>.|\n {}
<PRINT_INITIAL_STATE>.|\n {}
<PRINT_FINAL_STATE>.|\n {}
<GET_RULES>.|\n {}
<GET_SUBRULES>.|\n {}
<GET_SUBRULES_LEFT_Y>.|\n {}
<GET_SUBRULES_LEFT_X>.|\n {}
<GET_SUBRULES_RIGHT_Z>.|\n {}
<CHECK_GRAMMAR>.|\n {}
<CHECK_RULES>.|\n {}
<GOTO_NEWLINE>.|\n {}

%%

int main(void) {
	yylex();
	return 0;
}
