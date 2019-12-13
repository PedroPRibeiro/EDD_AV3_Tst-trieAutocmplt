#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include <fstream>

using namespace std;
#define ALFABETO (255)

struct Arv {
	char c;
	bool fim;
	int x;
	struct Arv *esq, *mei, *dir;
};

union Generico{//criar uma arvore com nome arv, com o intuito de passar valor = NULL no main, sem quebrar o codigo
    Arv *arv;
};

Arv* arv_cria (char c, unsigned fim){
	Arv* p=(Arv*)malloc(sizeof(Arv));
	p->c= c;
	p->fim = fim;
	p->x=0;
	p->esq = p->mei = p->dir = NULL;
	return p;
}

int altura(Arv *a){
    if(a == NULL) return -1;
    if(a->esq == NULL && a->dir == NULL){
        return 0;
    }else{
        if(altura(a->esq) > altura(a->dir)) return 1 + altura(a->esq);
        else return 1 + altura(a->dir);
    }
}

int nivelBalanceamento(Arv *a){
    if(a == NULL) return 0;
    if(a->esq == NULL && a->dir == NULL) return 0;
    return altura(a->esq) - altura(a->dir);
}

Arv **arvBuscaComBal(Arv **a){
    if(*a == NULL || (*a)->x == 3) return NULL;

    if((*a)->esq != NULL && (*a)->x == 0){
        Arv **tmp = arvBuscaComBal(&(*a)->esq);
        if(tmp != NULL) return tmp;
    }
    (*a)->x++;

    if((*a)->dir != NULL && (*a)->x == 1){
        Arv **tmp = arvBuscaComBal(&(*a)->dir);
        if(tmp != NULL) return tmp;
    }
    (*a)->x++;

    if(nivelBalanceamento(*a) > 1 || nivelBalanceamento(*a) < -1) return a;

    if((*a)->mei != NULL && (*a)->x == 2){
        Arv **tmp = arvBuscaComBal(&(*a)->mei);
        if(tmp != NULL) return tmp;
    }
    (*a)->x++;

    return NULL;
}


void adicionar(Arv** arv, const char* palavra){
    if(*arv == NULL){
        *arv = arv_cria(palavra[0], palavra[1] == '\0');
        if (palavra[1] == '\0') return;
    }

    if(palavra[0] == (*arv)->c){
        adicionar(&(*arv)->mei, palavra+1);
    }else  if(palavra[0] > (*arv)->c){
        adicionar(&(*arv)->dir, palavra);
    }else{
        adicionar(&(*arv)->esq, palavra);
    }
}
void adicionarOGenerico(int type, Generico* arv, const char* palavra){
            adicionar(&(arv->arv), palavra);

}


void mostrar(Arv *a, string tmp, int *i){
    if(a == NULL || *i >= 10) return;
    mostrar(a->esq, tmp, i);

    tmp.push_back(a->c);
    if(a->fim==true){
        cout << tmp << endl;
    }
    mostrar(a->mei, tmp, i);

    tmp.pop_back();
    mostrar(a->dir, tmp, i);
}

void buscaArv(const char *word, Arv *a, string tmp){
    if (a->c == word[0] && word[1] == '\0'){
        tmp.push_back(a->c);
        if(a->fim) cout << tmp << endl;
        int i = 0;
        mostrar(a->mei, tmp, &i);
    }
    else if (a->c == word[0]){
        tmp.push_back(a->c);
        word++;
        buscaArv(word, a->mei, tmp);
    }else if(word[0] < a->c) buscaArv(word, a->esq, tmp);
     else if(word[0] > a->c) buscaArv(word, a->dir, tmp);
}

void gotoxy(int x, int y){
    // COLL / ROW
     SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),(COORD){x-1,y-1});
}



int main()
{
    clock_t tempo = clock();

    Generico myArv;
    myArv.arv = NULL;

    string saida;

    tempo = clock();
    ifstream dicionario("wordds.txt");
    if(dicionario.is_open()){
        while(getline(dicionario, saida)){
            adicionarOGenerico(0, &myArv, saida.c_str());
        }
    }

    Arv **a;
    tempo = clock();
    do{
    a = arvBuscaComBal(&myArv.arv);
    }while(a != NULL);

    string tmp = "";
    char c;
    string s = "";

    while (c != 13){
        gotoxy(5,1);
        printf("%d    ", c);
        cout << "Texto: ";
        cout << s;
        c = getche();
        system ("CLS");
        if (c == 8 && s.length()>0){
            s.pop_back();
        }
        else if (c >= 10 && c <= 255){
            s.push_back(c);
        }
        tempo = clock();
        if(s != "") buscaArv(s.c_str(), myArv.arv, tmp);
        gotoxy(25,1);cout << "Tempo: " << float(clock() - tempo) / CLOCKS_PER_SEC << endl;
    }
    return 0;
}
