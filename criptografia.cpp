#include<bits/stdc++.h>
using namespace std;

char criptCh(int key, char ch){
    const int start = 65, end = 122;
    char chCript;

    if(ch < start || ch > end) return ch;
    chCript = ch + key;
    if(chCript > end) chCript = chCript - end + start - 1;

    return chCript;
}

char decriptCh(int key, char ch){
    const int start = 65, end = 122;
    char chCript;

    if(ch < start || ch > end) return ch;
    chCript = ch - key;
    if(chCript < start) chCript = chCript + end - start + 1;

    return chCript;
}

void criptStr(int key, char * phrase){
    const int start = 65, end = 122;
    
    int size = strlen(phrase);
    char cript[size+1] = "", aux;

    for (int i = 0; i < strlen(phrase); i++)
    {   
        aux = phrase[i];
        if(aux < start || aux > end) {
            strncat(cript, &aux, 1);
            continue;
        }
        aux += key;
        if(aux > end) aux = aux - end + start - 1;
        strncat(cript, &aux, 1);
    }

    strcpy(phrase, cript);
}

void decriptStr(int key, char * cript){
    const int start = 48, end = 122;

    int size = strlen(cript);
    char phrase[size] = "", aux;

    for(int i=0; i<size; i++){
        aux = cript[i];
        if(aux < start || aux > end) {
            strncat(phrase, &aux, 1);
            continue;
        }
        aux -= key;
        if(aux < start) aux = aux + end - start + 1;
        strncat(phrase, &aux, 1);
    }
}

void fileCriptWrite(){
    FILE * file = fopen("./files/help.txt", "wt");
    char text[13][1000] = 
    {"Soptify: Ajuda\n\n", 
    "1) Navegue nos menus utilizando as setas do teclado (cima e baixo)\n",
    "2) Em qualquer menu, para voltar ou sair, pressione <ESC>\n", 
    "3) Se quiser acessar a ajuda novamente, pressione <F1>\n\n", 
    "Sobre o cadastro de Artistas: \n", 
    "1) O ID do artista deve ser numerico\n", 
    "2) Um ID se refere apenas a um artista\n\n",
    "Sobre o cadastro de Musicas:\n",
    "1) Os mesmos requisitos do ID de artista devem ser atendidos no ID de musica\n",
    "2) As musicas sao dependentes dos artistas\n", 
    "\t2.a) O cadastro de musica depende do cadastro de um artista\n", 
    "\t2.b) Quando um artista eh deletado, suas musicas tambem sao",
    "\n\n\n\t\t\tDesenvolvedores:\n\t\t\t\tSofia Azevedo Rosa\t22102558\n\t\t\t\tVinicius Casimiro da Silveira\t221026321\n"};

    for(int i=0; i<13;i++){
        criptStr(3, text[i]);
        fputs(text[i], file);
    }

    fclose(file);
}

void fileCriptRead(){
    FILE * file = fopen("./files/help.txt", "rt");
    char ch;
    if(!file){
        cout << "Error" << endl;
        return;
    }

    while (!feof(file)) {
        ch = getc(file);
        printf ("%c",decriptCh(3, ch));
    };
}

int main(){ 
    fileCriptWrite();
    fileCriptRead();
}