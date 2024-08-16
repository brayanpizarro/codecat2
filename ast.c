#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VARS 100

struct {
    char *name;
    int value;
} variables[MAX_VARS];

int var_count = 0;

int get_variable_value(const char *name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            return variables[i].value;
        }
    }
    printf("Error: Variable %s no definida\n", name);
    return 0;
}

void set_variable_value(const char *name, int value) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            variables[i].value = value;
            return;
        }
    }
    if (var_count < MAX_VARS) {
        variables[var_count].name = strdup(name);
        variables[var_count].value = value;
        var_count++;
    } else {
        printf("Error: Demasiadas variables\n");
    }
}

ASTNode *create_number_node(int value) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_NUMBER;
    node->data.value = value;
    return node;
}

ASTNode *create_identifier_node(char *name) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_IDENTIFIER;
    node->data.name = strdup(name);
    return node;
}

ASTNode *create_binary_op_node(char op, ASTNode *left, ASTNode *right) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_BINARY_OP;
    node->data.op.op = op;
    node->data.op.left = left;
    node->data.op.right = right;
    return node;
}

ASTNode *create_assign_node(char *name, ASTNode *value) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_ASSIGN;
    node->data.assign.name = strdup(name);
    node->data.assign.value = value;
    return node;
}

ASTNode *create_if_node(ASTNode *condition, ASTNode *if_body, ASTNode *else_body) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_IF;
    node->data.if_stmt.condition = condition;
    node->data.if_stmt.if_body = if_body;
    node->data.if_stmt.else_body = else_body;
    return node;
}

ASTNode *create_while_node(ASTNode *condition, ASTNode *body) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_WHILE;
    node->data.while_stmt.condition = condition;
    node->data.while_stmt.body = body;
    return node;
}

ASTNode *create_print_node(ASTNode *expr) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_PRINT;
    node->data.print.expr = expr;
    return node;
}

ASTNode *create_block_node(ASTNode **statements, int count) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_BLOCK;
    node->data.block.statements = statements;
    node->data.block.count = count;
    return node;
}

int evaluate_expr(ASTNode *node) {
    if (!node) return 0;

    switch (node->type) {
        case NODE_NUMBER:
            return node->data.value;
        case NODE_IDENTIFIER:
            return get_variable_value(node->data.name);
        case NODE_BINARY_OP:
            switch (node->data.op.op) {
                case '+': return evaluate_expr(node->data.op.left) + evaluate_expr(node->data.op.right);
                case '-': return evaluate_expr(node->data.op.left) - evaluate_expr(node->data.op.right);
                case '*': return evaluate_expr(node->data.op.left) * evaluate_expr(node->data.op.right);
                case '/': {
                    int divisor = evaluate_expr(node->data.op.right);
                    if (divisor == 0) {
                        printf("Error: División por cero\n");
                        return 0;
                    }
                    return evaluate_expr(node->data.op.left) / divisor;
                }
                case '<': return evaluate_expr(node->data.op.left) < evaluate_expr(node->data.op.right);
                case '>': return evaluate_expr(node->data.op.left) > evaluate_expr(node->data.op.right);
                case '=': return evaluate_expr(node->data.op.left) == evaluate_expr(node->data.op.right);
                default:
                    printf("Operador desconocido: %c\n", node->data.op.op);
                    return 0;
            }
        default:
            printf("Tipo de nodo no evaluable como expresión\n");
            return 0;
    }
}

void evaluate_ast(ASTNode *node) {
    if (!node) {
        printf("Nodo nulo encontrado\n");
        return;
    }

    printf("Evaluando nodo de tipo: %d\n", node->type);

    switch (node->type) {
        case NODE_NUMBER:
            printf("Número: %d\n", node->data.value);
            break;
        case NODE_IDENTIFIER:
            printf("Identificador: %s (valor: %d)\n", node->data.name, get_variable_value(node->data.name));
            break;
        case NODE_BINARY_OP:
            {
                int left = evaluate_expr(node->data.op.left);
                int right = evaluate_expr(node->data.op.right);
                int result;
                switch (node->data.op.op) {
                    case '+': result = left + right; break;
                    case '-': result = left - right; break;
                    case '*': result = left * right; break;
                    case '/': 
                        if (right == 0) {
                            printf("Error: División por cero\n");
                            result = 0;
                        } else {
                            result = left / right;
                        }
                        break;
                    case '<': result = left < right; break;
                    case '>': result = left > right; break;
                    case '=': result = left == right; break;
                    default:
                        printf("Operador desconocido: %c\n", node->data.op.op);
                        result = 0;
                }
                printf("Operación: %d %c %d = %d\n", left, node->data.op.op, right, result);
            }
            break;
        case NODE_ASSIGN:
            {
                int value = evaluate_expr(node->data.assign.value);
                set_variable_value(node->data.assign.name, value);
                printf("Asignación: %s = %d\n", node->data.assign.name, value);
            }
            break;
        case NODE_IF:
            {
                int condition = evaluate_expr(node->data.if_stmt.condition);
                printf("GATITO condición: %d\n", condition);
                if (condition) {
                    printf("Ejecutando bloque GATITO\n");
                    evaluate_ast(node->data.if_stmt.if_body);
                } else if (node->data.if_stmt.else_body) {
                    printf("Ejecutando bloque GATOTE\n");
                    evaluate_ast(node->data.if_stmt.else_body);
                }
            }
            break;
        case NODE_WHILE:
            printf("Iniciando bucle SIAMES\n");
            while (evaluate_expr(node->data.while_stmt.condition)) {
                printf("Iteración del bucle SIAMES\n");
                evaluate_ast(node->data.while_stmt.body);
            }
            printf("Fin del bucle SIAMES\n");
            break;
        case NODE_PRINT:
            {
                int value = evaluate_expr(node->data.print.expr);
                printf("PRINTCAT: %d\n", value);
            }
            break;
        case NODE_BLOCK:
            printf("Evaluando bloque de %d statements\n", node->data.block.count);
            for (int i = 0; i < node->data.block.count; i++) {
                evaluate_ast(node->data.block.statements[i]);
            }
            break;
        default:
            printf("Tipo de nodo desconocido: %d\n", node->type);
            break;
    }
}

void free_ast(ASTNode *node) {
    if (!node) return;

    switch (node->type) {
        case NODE_IDENTIFIER:
            free(node->data.name);
            break;
        case NODE_BINARY_OP:
            free_ast(node->data.op.left);
            free_ast(node->data.op.right);
            break;
        case NODE_ASSIGN:
            free(node->data.assign.name);
            free_ast(node->data.assign.value);
            break;
        case NODE_IF:
            free_ast(node->data.if_stmt.condition);
            free_ast(node->data.if_stmt.if_body);
            free_ast(node->data.if_stmt.else_body);
            break;
        case NODE_WHILE:
            free_ast(node->data.while_stmt.condition);
            free_ast(node->data.while_stmt.body);
            break;
        case NODE_PRINT:
            free_ast(node->data.print.expr);
            break;
        case NODE_BLOCK:
            for (int i = 0; i < node->data.block.count; i++) {
                free_ast(node->data.block.statements[i]);
            }
            free(node->data.block.statements);
            break;
        default:
            break;
    }
    free(node);
}