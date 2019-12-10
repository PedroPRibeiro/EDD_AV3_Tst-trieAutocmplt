#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include <fstream>

using namespace std;
#define ALFABETO (57)

struct Arv {
	char gambiarra;
	//struct Arv* esq;
	//struct Arv* dir;
	unsigned fim: 1;


	struct Arv *esq, *mei, *dir;
};

Arv* arv_criavazia (void){
	return NULL;
}

Arv* arv_cria (char gambiarra, Arv* sae, Arv* sam, Arv* sad){
	Arv* p=(Arv*)malloc(sizeof(Arv));
	p->gambiarra= gambiarra;
	p->esq = sae;
	p->mei = sam;
	p->dir = sad;
	p->fim = 0;
	return p;
}

int arv_vazia (Arv* a){
	return a==NULL;
}

Arv* arv_libera (Arv* a){
	if (!arv_vazia(a)){
	  arv_libera (a->esq); /* libera sae */
	  arv_libera (a->dir); /* libera sad */
	  free(a); /* libera raiz */
	}
	return NULL;
}

bool arv_pertence (Arv* a, int c){
  if (arv_vazia(a))
	return false; /* árvore vazia: não encontrou */
  bool p = a->gambiarra==c;
  if( p ) return p;
  p = arv_pertence (a->esq,c);
  if( p ) return p;
  return arv_pertence (a->dir,c);
}




void adicona_arvore(Arv** a, char *palavra){
        if (arv_vazia(*a))
            *a = arv_cria(*palavra,arv_criavazia(),arv_criavazia(),arv_criavazia());
        else if ((*a)->gambiarra > *palavra)
            adicona_arvore(&((*a)->esq),palavra);

        else if((*a)->gambiarra < *palavra)
            adicona_arvore(&((*a)->dir),palavra);

        else
        {
            if (*(palavra+1))
                adicona_arvore(&((*a)->mei), palavra+1);
            else
                (*a)->fim = 1;
        }

}

int busca(struct Arv *a, char *palavra)
{
    if (!a)
        return 0;

    if (*palavra < (a)->gambiarra)
        return busca(a->esq, palavra);

    else if (*palavra > (a)->gambiarra)
        return busca(a->dir, palavra);

    else
    {
        if (*(palavra+1) == '\0')
            return a->fim;

        return busca(a->mei, palavra+1);
    }
}

void gotoxy(int x, int y){
    // COLL / ROW
     SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),(COORD){x-1,y-1});
}

void sugestao(struct Arv* a, string gambiarraPt2)
{

    //procurando complemento com o prefixo fornecido
    if (a->fim=1)
    {
        cout << gambiarraPt2;
        cout << endl;
    }
    if (a->fim=0)
        return;

    for (int i = 0; i < ALFABETO; i++)
    {
        if (a->mei)
        {
           // acrescenta o caractere atual à string prefixo
            gambiarraPt2.push_back(65 + i);
            sugestao(a->mei, gambiarraPt2);
            gambiarraPt2.pop_back();
        }
    }
}

int imprimir(Arv* a, const string j)
{
        sugestao(a,j );
        return 1;

}





void listaComPrefixo(Arv *a, string gambiarraPt2){
    gotoxy(1,5);
    sugestao(a,gambiarraPt2);
    gotoxy(1,2);
}


int main()
{


    clock_t t1, t2, t3;

    Arv *a = arv_criavazia();


    ifstream dicionario;
    dicionario.open("words.txt");
    string saida;

    if (dicionario.is_open()){

        while (getline(dicionario, saida)){
            char p[saida.length()];
            strcpy(p, saida.c_str());
            adicona_arvore(&a,p);
            cout<< p <<endl;

            t2=clock();
            busca(a, p);

        }

    }

    dicionario.close();
    t3 = difftime(t1,t2);
    cout<<"Tempo: "<<((float)t3)/CLOCKS_PER_SEC<<"segundos "<<endl;



    t1 = clock();

    char c;
    string gambiarraPt2 = "";

    char j[gambiarraPt2.length()];
    strcpy(j, gambiarraPt2.c_str());

    char *palavra = j;


    while (c != 13){
        system ("CLS");
        listaComPrefixo(a,palavra);
        cout<<"Tempo de pesquisa : "<<((float)t3)/CLOCKS_PER_SEC<<"segundos "<<endl;
        printf("%d",c);
        cout << "Texto: ";
        cout << gambiarraPt2;
        c = getche();
        if (c == 8 && gambiarraPt2.length()>0){
            gambiarraPt2.pop_back();
        }
        else if (c >= 65 && c <= 122){
            gambiarraPt2.push_back(c);
        }
    }

    return 0;
}
