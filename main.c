#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define T 3

//STRUCT

typedef struct no{
    int chavesValidas;
    int chaves[T*2-1];
    bool folha;
    struct no* filhos[T*2];
}NO_ARV_B;

//INICIALIZAÇÃO

void InicializaArvB (NO_ARV_B* raiz){
    raiz->chavesValidas = 0;
    int i;
    for (i = 0; i < T*2; i++){
        raiz->filhos[i] = NULL;
    }
    raiz->folha = true;
}

//FUNÇÕES AUXILIÁRES

int ProcuraEmNO(NO_ARV_B* no, int ch){
    int i = 0;
    while (i < no->chavesValidas){
        if (no->chaves[i] == ch){
            printf("ERRO! Chave ja existente na arvore!");
            return -1;
        }
        if (no->chaves[i] > ch)
            break;
        i++;
    }
    return i;
}

void QuebraNO(NO_ARV_B** raiz ,NO_ARV_B* pai, NO_ARV_B* filho, int i){
    NO_ARV_B* novo = (NO_ARV_B*)malloc(sizeof(NO_ARV_B));
    InicializaArvB(novo);

    int j;
    for (j = (T*2)-2; j > T-1; j--){
        novo->chaves[j-T] = filho->chaves[j];
    }
    for (j = (T*2)-1; j > T-1; j--){
        novo->filhos[j-T] = filho->filhos[j];
    }
    novo->chavesValidas = T-1;
    novo->folha = filho->folha;

    if (*raiz == filho){
        NO_ARV_B* novoPai = (NO_ARV_B*)malloc(sizeof(NO_ARV_B));
        InicializaArvB(novoPai);
        pai = novoPai;
        i = 0;
        *raiz = novoPai;
    }

    for (j = pai->chavesValidas-1; j > i; j--){
        pai->chaves[j] = pai->chaves[j-1];
    }
    for (j = pai->chavesValidas; j > i; j--){
        pai->filhos[j] = pai->filhos[j-1];
    }
    pai->chaves[i] = filho->chaves[T-1];
    pai->chavesValidas = pai->chavesValidas+1;
    filho->chavesValidas = filho->chavesValidas-T;
}

void InsereEmNO(NO_ARV_B** raiz, NO_ARV_B* no, int ch, bool* modificar){
    if (no->chavesValidas == T*2-1){
        if (no == *raiz){
            QuebraNO(raiz,no,no,0);
        }else {
            *modificar = true;
            return;
        }
    }
    int i = ProcuraEmNO(no,ch);
    if (i == -1){
        return;
    }
    if (no->folha){
        int j;
        for (j = no->chavesValidas-1; j > i; j--){
            no->chaves[j] = no->chaves[j-1];
        }
        no->chaves[i] = ch;
        no->chavesValidas = no->chavesValidas+1;
        if (no == *raiz){
            no->folha = false;
        }
    }else {
        if (!no->filhos[i]){
            NO_ARV_B* novo = (NO_ARV_B*)malloc(sizeof(NO_ARV_B));
            InicializaArvB(novo);
            no->filhos[i] = novo;
        }
        InsereEmNO(raiz,no->filhos[i],ch,modificar);
        if (*modificar){
            QuebraNO(raiz,no,no->filhos[i],i);
            *modificar = false;
            i = ProcuraEmNO(no,ch);
            if (i == -1){
                return;
            }
            InsereEmNO(raiz,no->filhos[i],ch,modificar);
        }
    }
}

void EscreveArvB (FILE* saida, NO_ARV_B* no){
    fprintf(saida,"%s","(");
    int i;
    for(i = 0; i < no->chavesValidas; i++){
        if(no->filhos[i]){
            EscreveArvB(saida, no->filhos[i]);
            fprintf(saida," ");
        }
        fprintf(saida,"%d",no->chaves[i]);
        if (i < no->chavesValidas -1){
            fprintf(saida," ");
        }
    }
    if(no->filhos[i]){
        fprintf(saida," ");
        EscreveArvB(saida, no->filhos[i]);
    }
    fprintf(saida,"%s",")");
}

//FUNÇÕES PRINCIPAIS

void InsereArvB (NO_ARV_B* raiz, int ch){
    if (ch < 0){
        printf("ERRO! Valor da chave menor que o minimo permitido de 0!\n");
        return;
    }
    bool modificar = false;
    InsereEmNO(&raiz,raiz,ch,&modificar);
}

void RemoveArvB(NO_ARV_B* raiz, int ch){
    if (ch < 0){
        printf("ERRO! Valor da chave menor que o minimo permitido de 0!\n");
        return;
    }

}

void EscreveSaida (char* caminhoSaida, NO_ARV_B* raiz){
    FILE* saida = fopen(caminhoSaida,"w");
    EscreveArvB(saida,raiz);
    fclose(saida);
}

void LeEntrada (char* caminhoEntrada, char* caminhoSaida, NO_ARV_B* raiz){
    FILE* entrada = fopen(caminhoEntrada,"rt");
    int ch;
    char comando;

    while (true){
        fscanf(entrada,"%c",&comando);
        switch(comando){
        case 'f':
            return;
        case 'i':
            fscanf(entrada," %d\n",&ch);
            InsereArvB(raiz,ch);
            break;
        case 'p':
            EscreveSaida(caminhoSaida,raiz);
            break;
        case EOF:
            return;
        default:
            printf("ERRO! Entrada invalida!");
            break;
        }
    }
    fclose(entrada);
}

//MAIN

int main(int argc, char** argv){
    NO_ARV_B teste;
    InicializaArvB(&teste);
    LeEntrada(argv[1], argv[2],&teste);
    return 0;
}
