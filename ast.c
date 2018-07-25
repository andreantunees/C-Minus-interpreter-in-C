#include "ast.h"

AST* new_node(NodeKind kind, int count, ...) {
    AST *node = (AST *) malloc(sizeof(AST));
    node->count = 0;
    node->data = -1;
    node->kind = kind;
    va_list ap;
    va_start(ap, count);
    int i;
    for (i = 0; i < count; i++) {
        add_child(node, va_arg(ap, AST*));
    }
    va_end(ap);
    return node;
}

void add_child(AST *parent, AST *child) {
    if (parent->count == CHILDREN_LIMIT) {
        fprintf(stderr, "Cannot add another child!\n");
        exit(1);
    }
    parent->child[parent->count] = child;
    parent->count++;
}

AST* new_leaf(NodeKind kind, int data) {
    AST *leaf = (AST *) malloc(sizeof(AST));
    leaf->count = 0;
    leaf->kind = kind;
    leaf->data = data;

    return leaf;
}

AST* get_child(AST *parent, int idx) {
    return parent->child[idx];
}

int get_data(AST *node) {
    return node->data;
}

NodeKind get_kind(AST* node){
    return node->kind;
}

int get_count(AST *node) {
    return node->count;
}

AST* make_node(NodeKind kind, int data, char* text) {
    AST* node = new_leaf(kind, data);
    node->text = (char *) malloc((strlen(text)) * sizeof(char) + 1);
    strcpy(node->text, text);

    return node;
}

char* node2str(NodeKind kind) {
    char* s = (char*) malloc(64 * sizeof(char));

    switch(kind) {
        case INPUT_NODE:                sprintf(s, "%s", "input");      break;
        case OUTPUT_NODE:               sprintf(s, "%s", "output");     break;

        case STMT_LIST_NODE:            sprintf(s, "%s", "block");      break;
        case ELSE_NODE:                 sprintf(s, "%s", "else");       break;
        case WHILE_NODE:                sprintf(s, "%s", "while");      break;
        case IF_NODE:                   sprintf(s, "%s", "if");         break;
        
        case USED_NODE_N:                 sprintf(s, "%s", "var_use");    break;
        case USED_NODE_V:                 sprintf(s, "%s", "var_use");    break;
        case VAR_DCL_N:                   sprintf(s, "%s", "var_decl");   break;
        case VAR_DCL_V:                   sprintf(s, "%s", "var_decl");   break;
        case VAR_LIST_NODE:             sprintf(s, "%s", "var_list");   break;

        case FUNC_HEADER_NODE:          sprintf(s, "%s", "func_header");break;
        case FUNC_DECL_NODE:            sprintf(s, "%s", "func_decl");  break;
        case FCALL_NODE:                sprintf(s, "%s", "fcall");      break;
        case ARG_LIST_NODE:             sprintf(s, "%s", "arg_list");   break;
        case FUN_NODE:                  sprintf(s, "%s", "func_name");  break;
        case FUNC_DECL_LIST_NODE:       sprintf(s, "%s", "func_list");  break;
        case FUNC_BODY_NODE:            sprintf(s, "%s", "func_body");  break;

        case PARAM_NODE:                sprintf(s, "%s", "param_list"); break;

        case NUM_NODE:                  sprintf(s, "%s", "num");        break;
        case STRING_NODE:               sprintf(s, "%s", "string");     break;

        case WRITE_NODE:                sprintf(s, "%s", "write");      break;
        case RETURN_NODE:               sprintf(s, "%s", "return");     break;

        case LT_NODE:                   sprintf(s, "%s", "<");          break;
        case LE_NODE:                   sprintf(s, "%s", "<=");         break;
        case PLUS_NODE:                 sprintf(s, "%s", "+");          break;
        case MINUS_NODE:                sprintf(s, "%s", "-");          break;
        case GE_NODE:                   sprintf(s, "%s", ">=");         break;
        case NEQ_NODE:                  sprintf(s, "%s", "!=");         break;
        case GT_NODE:                   sprintf(s, "%s", ">");          break;
        case TIMES_NODE:                sprintf(s, "%s", "*");          break;
        case ASSIGN_NODE:               sprintf(s, "%s", "=");          break;
        case EQ_NODE:                   sprintf(s, "%s", "==");         break;
        case OVER_NODE:                 sprintf(s, "%s", "/");          break;

        default:                        sprintf(s, "%s", "<error>");    break;
    }
    return s;
}


void free_tree(AST *tree) {
    if (tree != NULL) {
        int i;
        for(i = 0; i < tree->count; i++) {
            free_tree(tree->child[i]);
        }
        free(tree);
    }
}
