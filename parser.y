%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

extern int yylex();
extern int yylineno;
extern FILE* yyin;
void yyerror(const char* s);
ASTNode* root = NULL;
%}

%define parse.error verbose

%union {
    int num;
    char* id;
    struct ASTNode* node;
}

%token <num> NUMBER
%token <id> IDENTIFIER
%token PLUS MINUS TIMES DIVIDE
%token LT GT LE GE EQ NE
%token AND OR
%token ASSIGN
%token IF ELSE WHILE PRINT
%token LPAREN RPAREN

%type <node> program statement_list statement expr
%type <node> assignment_statement if_statement while_statement print_statement

%left OR
%left AND
%left EQ NE
%left LT GT LE GE
%left PLUS MINUS
%left TIMES DIVIDE
%right UMINUS

%%

program
    : statement_list { root = $1; }
    ;

statement_list
    : statement { $$ = create_block_node(&$1, 1); }
    | statement_list statement {
        ASTNode** new_list = malloc(sizeof(ASTNode*) * ($1->data.block.count + 1));
        memcpy(new_list, $1->data.block.statements, sizeof(ASTNode*) * $1->data.block.count);
        new_list[$1->data.block.count] = $2;
        $$ = create_block_node(new_list, $1->data.block.count + 1);
        free($1->data.block.statements);
        free($1);
    }
    ;

statement
    : assignment_statement
    | if_statement
    | while_statement
    | print_statement
    | '{' statement_list '}' { $$ = $2; }
    ;

assignment_statement
    : IDENTIFIER ASSIGN expr ';' { $$ = create_assign_node($1, $3); free($1); }
    ;

if_statement
    : IF LPAREN expr RPAREN statement { $$ = create_if_node($3, $5, NULL); }
    | IF LPAREN expr RPAREN statement ELSE statement { $$ = create_if_node($3, $5, $7); }
    ;

while_statement
    : WHILE LPAREN expr RPAREN statement { $$ = create_while_node($3, $5); }
    ;

print_statement
    : PRINT LPAREN expr RPAREN ';' { $$ = create_print_node($3); }
    ;

expr
    : NUMBER { $$ = create_number_node($1); }
    | IDENTIFIER { $$ = create_identifier_node($1); free($1); }
    | expr PLUS expr { $$ = create_binary_op_node('+', $1, $3); }
    | expr MINUS expr { $$ = create_binary_op_node('-', $1, $3); }
    | expr TIMES expr { $$ = create_binary_op_node('*', $1, $3); }
    | expr DIVIDE expr { $$ = create_binary_op_node('/', $1, $3); }
    | expr LT expr { $$ = create_binary_op_node('<', $1, $3); }
    | expr GT expr { $$ = create_binary_op_node('>', $1, $3); }
    | expr LE expr { $$ = create_binary_op_node('l', $1, $3); }
    | expr GE expr { $$ = create_binary_op_node('g', $1, $3); }
    | expr EQ expr { $$ = create_binary_op_node('=', $1, $3); }
    | expr NE expr { $$ = create_binary_op_node('!', $1, $3); }
    | expr AND expr { $$ = create_binary_op_node('&', $1, $3); }
    | expr OR expr { $$ = create_binary_op_node('|', $1, $3); }
    | LPAREN expr RPAREN { $$ = $2; }
    | MINUS expr %prec UMINUS { $$ = create_binary_op_node('-', create_number_node(0), $2); }
    ;

%%

void yyerror(const char* s) {
    fprintf(stderr, "Error de sintaxis en la línea %d: %s\n", yylineno, s);
    exit(1);  // Termina el programa en caso de error
}

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s archivo_de_entrada\n", argv[0]);
        return 1;
    }

    FILE* input_file = fopen(argv[1], "r");
    if (!input_file) {
        fprintf(stderr, "No se pudo abrir el archivo %s\n", argv[1]);
        return 1;
    }

    yyin = input_file;
    printf("Analizando el archivo...\n");
    int parse_result = yyparse();
    fclose(input_file);

    printf("Análisis completado. Resultado: %d\n", parse_result);

    if (parse_result == 0 && root != NULL) {
        printf("AST generado correctamente. Evaluando:\n");
        evaluate_ast(root);
        free_ast(root);
    } else {
        printf("No se generó un AST válido o hubo un error en el análisis.\n");
    }

    return 0;
}