#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                             parse.c
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//トークンの種類
typedef enum {
    TK_RESERVED,//記号
    TK_NUM,     //整数トークン
    TK_EOF,     //入力の終わりを示すトークン
} TokenKind;

typedef struct Token Token;

//トークン型
struct Token {
    TokenKind kind; //トークンの型
    Token *next;    //次の入力トークン
    int val;        //kindがTK_NUMの場合、その数値
    char* str;      //トークン文字列
    int len;        //トークンの長さ
};

extern Token *token;
extern char* user_input;

//エラーを報告するための関数
//printfと同じ引数をとる
void error(char *fmt, ...);

//エラー箇所を報告する関数
void error_at(char* loc, char* fmt, ...);

//次のトークンが期待している記号の時には、トークンを１つ読み進めて
//真を返す。それ以外の場合には偽を返す。
bool consume(char* op);

//次のトークンが期待している記号のときには、トークンを１つ読み進める。
//それ以外の場合にはエラーを報告する。
void expect(char* op);

//次のトークンが数値の場合、トークンを１つ読み進めてその数値を返す。
//それ以外の場合にはエラーを報告する。
int expect_number();

//現在のトークンが終端か
bool at_eof();

//新しいトークンを作成してcurにつなげる
Token* new_token(TokenKind kind, Token* cur, char* str, int len);

//文字列pが文字列qから始まっているか
bool startswith(char* p, char* q);

//入力文字列user_inputをトークナイズしてそれを返す。
Token* tokenize();

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                             codegen.c
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//抽象構文木のノードの種類
typedef enum {
    ND_ADD, // +
    ND_SUB, // -
    ND_MUL, // *
    ND_DIV, // /
    ND_EQ,  // ==
    ND_NE,  // !=
    ND_LT,  // <(>は反転して仕様)
    ND_LE,  // <=
    ND_NUM, // 整数
} NodeKind;

typedef struct Node Node;

//抽象構文木のノードの型
struct Node {
    NodeKind kind;  //ノードの型
    Node* lhs;      //左辺
    Node* rhs;      //右辺
    int val;        //kindがND_NUMの場合のみ使う
};

//指定した型のノードを生成する
Node* new_node(NodeKind kind);

//2項演算子を生成する
Node* new_binary(NodeKind kind, Node* lhs, Node* rhs);

//整数ノードを生成する
Node* new_num(int val);

//式の構文木：expr = equality
Node* expr();
//等価演算の構文木：equality = relational ("==" relational | "!=" relational)*
Node* equality();
//比較演算の構文木：relational = add ("<" add | "<=" add | ">" add | ">=" add)*
Node* relational();
//加減算の構文木：add = mul ("+" mul | "-" mul)*
Node* add();
//乗除算の構文木：mul = unary ("*" unary | "/" unary)*
Node* mul();
//単項演算子の構文木：("+"|"-")? primary
Node* unary();
//優先()の構文木：primary = "(" expr ")" | num
Node* primary();

//スタックを利用して抽象構文木をおりながら四則演算のコードを生成する
void gen(Node* node);