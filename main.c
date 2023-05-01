#include "9cc.h"

int main(int argc, char* argv[]){
    if(argc != 2){
        error("引数の個数が正しくありません\n");
        return 1;
    }

    //トークナイズしてパースする
    user_input = argv[1];
    token = tokenize();
    Node* node = expr();

    //アセンブリの前半部分を出力
    printf(".intel_syntax noprefix\n"); //アセンブラにIntel記法であることを知らせる
    printf(".globl main\n");    //グローバルスコープで見えている関数を知らせる
    printf("main:\n");  //関数の開始

    //抽象構文木を下りながらコード生成
    gen(node);

    //スタックトップに式全体の値が残っているはずなので
    //それをraxにロードして関数からの返り値とする
    printf("    pop rax\n");
    printf("    ret\n");    //return
    return 0;
}