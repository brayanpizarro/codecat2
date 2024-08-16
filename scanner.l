%{
#define _POSIX_C_SOURCE 200809L
#include <string.h>
#include "ast.h"
#include "parser.tab.h"
%}

%option yylineno
%option noyywrap

%%

[0-9]+          { printf("TOKEN: NUMBER (%s)\n", yytext); yylval.num = atoi(yytext); return NUMBER; }
"GATITO"        { printf("TOKEN: IF\n"); return IF; }
"GATOTE"        { printf("TOKEN: ELSE\n"); return ELSE; }
"SIAMES"        { printf("TOKEN: WHILE\n"); return WHILE; }
"PRINTCAT"      { printf("TOKEN: PRINT\n"); return PRINT; }
"+"             { printf("TOKEN: PLUS\n"); return PLUS; }
"-"             { printf("TOKEN: MINUS\n"); return MINUS; }
"*"             { printf("TOKEN: TIMES\n"); return TIMES; }
"/"             { printf("TOKEN: DIVIDE\n"); return DIVIDE; }
"<"             { printf("TOKEN: LT\n"); return LT; }
">"             { printf("TOKEN: GT\n"); return GT; }
"<="            { printf("TOKEN: LE\n"); return LE; }
">="            { printf("TOKEN: GE\n"); return GE; }
"=="            { printf("TOKEN: EQ\n"); return EQ; }
"!="            { printf("TOKEN: NE\n"); return NE; }
"and"           { printf("TOKEN: AND\n"); return AND; }
"or"            { printf("TOKEN: OR\n"); return OR; }
"="             { printf("TOKEN: ASSIGN\n"); return ASSIGN; }
"("             { printf("TOKEN: LPAREN\n"); return LPAREN; }
")"             { printf("TOKEN: RPAREN\n"); return RPAREN; }
"{"             { printf("TOKEN: {\n"); return '{'; }
"}"             { printf("TOKEN: }\n"); return '}'; }
";"             { printf("TOKEN: ;\n"); return ';'; }
[a-zA-Z_][a-zA-Z0-9_]* { printf("TOKEN: IDENTIFIER (%s)\n", yytext); yylval.id = strdup(yytext); return IDENTIFIER; }
[ \t\n]         { /* ignorar espacios en blanco */ }
.               { printf("Caracter no reconocido: %s\n", yytext); }

%%