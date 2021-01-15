	int num_lines = 0;
	int num_words = 0;
	int num_capital_words = 0;
%option noyywrap
%%

\n	num_lines++;
([A-Za-z\-]+)	num_words++;
\b([A-Z]+[a-z\-]+)\b	num_capital_words++;

%%
main(void) {
	yylex();
	printf("\n\n# of lines = %d\n", num_lines);
	printf("# of words = %d\n", num_words);
	printf("# of capital words = %d\n", num_capital_words);
}
