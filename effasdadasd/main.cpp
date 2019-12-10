#include<bits/stdc++.h>
#include <string>
#include<windows.h>
#include<stdio.h>
#include<conio.h>
#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;
//  ALFABETO
#define ALFABETO (26)

// Converte o caractere atual para o valor da tabela ASCII
#define CHAR_TO_INDEX(c) ((int)c - (int)'a')

struct Arv
{
    struct Arv *sub_arv[ALFABETO];
    bool  finalisador;
};

int imprimir(Arv* raiz, const string a);

Arv* criando_no(void)
{
    Arv* tmp = (Arv*)malloc(sizeof(Arv));
    tmp->finalisador = false;

    for (int i = 0; i < 26; i++)
        tmp->sub_arv[i] = NULL;

    return tmp;
}

void inserir_arv(struct Arv *raiz,  const string palavra)
{
    Arv* R = raiz;

    for (int i = 0; i < palavra.length(); i++)
    {
        int posicao = CHAR_TO_INDEX(palavra[i]);
        //cout<<posicao<<endl;
        if (R->sub_arv[posicao] == NULL)
            R->sub_arv[posicao] = criando_no();

        R = R->sub_arv[posicao];
    }


    // marca o �ltimo n� como folha
    R->finalisador = true;
}


//Retorna 0 se tiver filho e 1 se nao tiver
bool  ultimono (struct Arv* raiz)
{
    for (int i = 0; i < ALFABETO; i++)
        if (raiz->sub_arv[i])
            return 0;
    return 1;
}

// Recursive function to print auto-suggestions for given
// node.
void sugestao(struct Arv* raiz, string prefixo)
{

    //procurando complemento com o prefixo fornecido
    if (raiz->finalisador)
    {
        cout << prefixo;
        cout << endl;
    }
    if (ultimono(raiz))
        return;

    for (int i = 0; i < ALFABETO; i++)
    {
        if (raiz->sub_arv[i])
        {
           // acrescenta o caractere atual � string prefixo
            prefixo.push_back(65 + i);
            sugestao(raiz->sub_arv[i], prefixo);
            prefixo.pop_back();
        }
    }
}


void gotoxy(int x, int y){
    // COLL / ROW
     SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),(COORD){x-1,y-1});
}

void listaComPrefixo(Arv *raiz,string prefixo){
    gotoxy(1,5);
    imprimir(raiz, prefixo);
    gotoxy(1,1);
}
int imprimir(Arv* raiz, const string a)
{
    Arv* R =raiz;
    // ver se tem alguma palavra com o prefixo
    int n = a.length();
    for (int i = 0; i < n; i++)
    {
        int posicao = CHAR_TO_INDEX(a[i]);

        // nenhuma palavra encontrada com o prefixo
        if (R->sub_arv[posicao] == NULL)
            return 0;

        R = R->sub_arv[posicao];
    }
    // se encontrar a palavra com o prefixo
    bool EncontrouP = (R->finalisador == true);
    //se o prefixo for o ultimo no
    bool Ultimo_arv = ultimono(R);
    //se encontrar palavra com o prefixo no ultimo no
    if (EncontrouP && Ultimo_arv)
    {
        cout << a << endl;
        return -1;
    }
    // Mas se tiver outros no
    if (Ultimo_arv == NULL)
    {
        string prefixo = a;
        sugestao(R, prefixo);
        return 1;
    }
}
int main()
{

    clock_t t1, t2, t3;
    struct Arv *raiz = criando_no();

    ifstream dicionario;
    t1 = clock();
    dicionario.open("words.txt");
    string saida;

    if (dicionario.is_open()){

        while (getline(dicionario, saida)){
            cout<<"ok"<<endl;
            inserir_arv(raiz, saida );
            t2 = clock();

        }

    }

    dicionario.close();
    t3 = difftime(t2,t1);


    char c;
    string prefixo = "";


    while (c != 13){
        system ("CLS");
        listaComPrefixo(raiz,prefixo);

        cout<<"Tempo de pesquisa : "<<((float)t3)/CLOCKS_PER_SEC<<"segundos "<<endl;
        cout << "Texto: ";
        cout << prefixo;
        c = getche();
        if (c == 8 && prefixo.length()>0){
            prefixo.pop_back();
        }
        else if (c >= 97 && c <= 122){
            prefixo.push_back(c);
        }cout<<"Tempo: "<<((float)t3)/CLOCKS_PER_SEC<<"segundos "<<endl;
    }

    return 0;
}
