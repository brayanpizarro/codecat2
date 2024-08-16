#ifndef AST_H
#define AST_H

typedef enum {
    NODE_NUMBER,
    NODE_IDENTIFIER,
    NODE_BINARY_OP,
    NODE_ASSIGN,
    NODE_IF,
    NODE_WHILE,
    NODE_PRINT,
    NODE_BLOCK
} NodeType;

typedef struct ASTNode {
    NodeType type;
    union {
        int value;              // Para NODE_NUMBER
        char *name;             // Para NODE_IDENTIFIER
        struct {                // Para NODE_BINARY_OP
            char op;
            struct ASTNode *left;
            struct ASTNode *right;
        } op;
        struct {                // Para NODE_ASSIGN
            char *name;
            struct ASTNode *value;
        } assign;
        struct {                // Para NODE_IF
            struct ASTNode *condition;
            struct ASTNode *if_body;
            struct ASTNode *else_body;
        } if_stmt;
        struct {                // Para NODE_WHILE
            struct ASTNode *condition;
            struct ASTNode *body;
        } while_stmt;
        struct {                // Para NODE_PRINT
            struct ASTNode *expr;
        } print;
        struct {                // Para NODE_BLOCK
            struct ASTNode **statements;
            int count;
        } block;
    } data;
} ASTNode;

ASTNode *create_number_node(int value);
ASTNode *create_identifier_node(char *name);
ASTNode *create_binary_op_node(char op, ASTNode *left, ASTNode *right);
ASTNode *create_assign_node(char *name, ASTNode *value);
ASTNode *create_if_node(ASTNode *condition, ASTNode *if_body, ASTNode *else_body);
ASTNode *create_while_node(ASTNode *condition, ASTNode *body);
ASTNode *create_print_node(ASTNode *expr);
ASTNode *create_block_node(ASTNode **statements, int count);

void evaluate_ast(ASTNode *node);
void free_ast(ASTNode *node);

#endif // AST_H