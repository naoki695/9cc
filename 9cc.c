#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    if(argc != 2){
        fprintf(stderr, "引数の個数が正しくありません\n");
        return 1;
    }

    printf(".intel_syntax noprefix\n"); //アセンブラにIntel記法であることを知らせる
    printf(".globl main\n");    //グローバルスコープで見えている関数を知らせる
    printf("main:\n");  //関数の開始
    printf("    mov rax, %d\n", atoi(argv[1])); //raxレジスタ(returnで返される値を格納するcpu内のメモリ)に数値をコピー
    printf("    ret\n");    //return
    return 0;
}