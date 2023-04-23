#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    if(argc != 2){
        fprintf(stderr, "引数の個数が正しくありません\n");
        return 1;
    }

    char* p = argv[1];

    printf(".intel_syntax noprefix\n"); //アセンブラにIntel記法であることを知らせる
    printf(".globl main\n");    //グローバルスコープで見えている関数を知らせる
    printf("main:\n");  //関数の開始
    printf("    mov rax, %ld\n", strtol(p, &p, 10)); //raxレジスタ(returnで返される値を格納するcpu内のメモリ)に数値をコピー
    
    while(*p){
        if(*p == '+'){
            p++;
            printf("    add rax, %ld\n", strtol(p, &p, 10));    //strtolでは、数値を読み込んだ後、第２引数のポインタをアップデートして、次の文字を指すように更新する
            continue;
        }
        if(*p == '-'){
            p++;
            printf("    sub rax, %ld\n", strtol(p, &p, 10));
            continue;
        }

        fprintf(stderr, "予期しない文字です: '%c'\n", *p);
        return 1;
    }

    printf("    ret\n");    //return
    return 0;
}