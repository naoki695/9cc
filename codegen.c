#include "9cc.h"

Node* new_node(NodeKind kind) {
    Node* node = calloc(1, sizeof(Node));
    node->kind = kind;
    return node;
}

Node* new_binary(NodeKind kind, Node* lhs, Node* rhs) {
    Node* node = new_node(kind);
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

Node* new_num(int val) {
    Node* node = new_node(ND_NUM);
    node->val = val;
    return node;
}

//式の構文木：expr = equality
Node* expr() {
    return equality();
}

//等価演算の構文木：equality = relational ("==" relational | "!=" relational)*
Node* equality() {
    Node* node = relational();

    for(;;) {
        if(consume("=="))
            node = new_binary(ND_EQ, node, relational());
        else if(consume("!="))
            node = new_binary(ND_NE, node, relational());
        else
            return node;
    }
}

//比較演算の構文木：relational = add ("<" add | "<=" add | ">" add | ">=" add)*
Node* relational() {
    Node* node = add();

    for(;;) {
        if(consume("<"))
            node = new_binary(ND_LT, node, add());
        else if(consume("<="))
            node = new_binary(ND_LE, node, add());
        else if(consume(">"))
            node = new_binary(ND_LT, add(), node);
        else if(consume(">="))
            node = new_binary(ND_LE, add(), node);
        else
            return node;
    }
}

//加減算の構文木：add = mul ("+" mul | "-" mul)*
Node* add() {
    Node* node = mul();
    
    for(;;) {
        if(consume("+"))
            node = new_binary(ND_ADD, node, mul());
        else if(consume("-"))
            node = new_binary(ND_SUB, node, mul());
        else
            return node;
    }
}

//乗除算の構文木：mul = unary ("*" unary | "/" unary)*
Node* mul() {
    Node* node = unary();

    for(;;) {
        if(consume("*"))
            node = new_binary(ND_MUL, node, unary());
        else if(consume("/"))
            node = new_binary(ND_DIV, node, unary());
        else
            return node;
    }
}

//単項演算子の構文木：("+"|"-")? primary
Node* unary() {
    if(consume("-"))
        return new_binary(ND_SUB, new_num(0), unary());
    return primary();
}

//優先()の構文木：primary = "(" expr ")" | num
Node* primary() {
    //次のトークンが"("なら"(" expr ")"のはず
    if(consume("(")) {
        Node* node = expr();
        expect(")");
        return node;
    }

    //そうでなければ数値のはず
    return new_num(expect_number());
}

//スタックを利用して抽象構文木をおりながら四則演算のコードを生成する
void gen(Node* node) {
    if(node->kind == ND_NUM) {
        printf("    push %d\n", node->val);
        return;
    }

    gen(node->lhs);
    gen(node->rhs);

    printf("    pop rdi\n");
    printf("    pop rax\n");

    switch(node->kind) {
    case ND_ADD:
        printf("    add rax, rdi\n");
        break;
    case ND_SUB:
        printf("    sub rax, rdi\n");
        break;
    case ND_MUL:
        printf("    imul rax, rdi\n");
        break;
    case ND_DIV:
        printf("    cqo\n");        //raxに入っている64ビット値を128ビットにのばしてrdxとraxにセットする命令
        printf("    idiv rdi\n");   //x86-64における符号あり除算命令はRDXとRAXをとって128ビット整数とみなして、引数のレジスタの64ビット値で割り、商をrax、余りをrdxにセットする
        break;
    case ND_EQ:
        printf("    cmp rax, rdi\n");
        printf("    sete al\n");
        printf("    movzb rax, al\n");
        break;
    case ND_NE:
        printf("    cmp rax, rdi\n");
        printf("    setne al\n");
        printf("    movzb rax, al\n");
        break;
    case ND_LT:
        printf("    cmp rax, rdi\n");
        printf("    setl al\n");
        printf("    movzb rax, al\n");
        break;
    case ND_LE:
        printf("    cmp rax, rdi\n");
        printf("    setle al\n");
        printf("    movzb rax, al\n");
        break;
    }

    printf("    push rax\n");
}