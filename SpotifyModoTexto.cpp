#include<bits/stdc++.h>
#include <windows.h>
#include "conio_functions.h"

using namespace std;

#define columns 120
#define lines 36
#define horizontalCenter columns/2
#define verticalCenter lines/2
#define endSideBar columns/3

#define downArrow 80
#define upArrow 72
#define leftArrow 75
#define rightArrow 77

#define esc 27
#define enter 13
#define f1 59

#define artistsFile "./files/artists.dat"
#define musicsFile "./files/musics.dat"

//set struct
typedef struct{
	int mins, secs;
}TIME_;

typedef struct{
	int d, m, y;
}DATE_;

typedef struct{
	int id;
	char name[71];
	long int listeners;	
	bool deleted;
}ARTIST;

typedef struct{
	int id;
	ARTIST artist;
	TIME_ duration;
	char title[71];
	DATE_ release;
	bool deleted;
}MUSIC;

typedef struct{
	char name[71];
	TIME_ duration;
	int musicsQt;
	
	vector<MUSIC> musics;
}PLAYLIST;

//functions headers
///interface
void printHeader();
void clearMainMenuArea();
void clearMenuArea();

void printArtistInputMask(string header, int i);
void printMusicInputMask(string header, int i);

///menus
int mainMenuSelection(int rowMin, int rowMax, char options[][30]);
int mainMenu();
int menuSelection(int rowMin, int rowMax, char options[][30]);
void menuMusicExecute();
int menuMusic();
void menuArtistExecute();
int menuArtist();

///registers
////general

////artist
bool checkArtistId(int id); 
void readArtists(int i);
void repWriteArtist();
ARTIST setArtist();
void writeArtist(ARTIST artist);
void repEditArtist();
ARTIST findArtist(int id, FILE * file);
ARTIST editArtist(FILE * file);
void writeAlteredArtist(ARTIST artist, FILE * file);
void repDeleteArtist();
int deleteArtist(FILE * file);

////muscic
bool checkMusicId(int id); 
void readMusics(int i);
void writeMusic(MUSIC music);
void repWriteMusic();
MUSIC setMusic();
// void repEditMusic();
void repDeleteMusic();
MUSIC findMusic(int id, FILE * file);

int main(){
	system("mode con:cols=120 lines=36");
	setlocale(LC_CTYPE, "Portuguese");
	
	//struct's child
	ARTIST artist;
	MUSIC music;
	PLAYLIST playlist;
	
	int choice = 0;
	
	printHeader();
	
	while(choice >= 0){
		choice = mainMenu();
		
		if(choice == 5){
			choice = -1;
		}
		else{
			switch(choice){
				case 1:
					menuMusicExecute();
					break;
				case 2: 
					menuArtistExecute();
					break;
			}
		}
		
		clearMenuArea();
	}
	
	return 0;
}

//registers
///artists
ARTIST setArtist(){
	ARTIST artist;
	int aux;

	printArtistInputMask("Cadastro de Artista", 1);

	//input
	do {
		gotoxy(horizontalCenter + 16, 4); cout << "              ";
		gotoxy(horizontalCenter + 16, 4); cin >> aux;
		fflush(stdin);
	} while (!checkArtistId(aux) || aux <= 0 || aux > 10000); //check if id already exists or if its invalid
	artist.id = aux;

	gotoxy(horizontalCenter + 18, 5); fgets(artist.name, 70, stdin);
	gotoxy(horizontalCenter + 30, 6); cin >> artist.listeners;

	artist.deleted = false;
	
	return artist;
}

void writeArtist(ARTIST artist){
	FILE * file;
	file = fopen(artistsFile, "ab");
	fwrite(&artist, sizeof(ARTIST), 1, file);
	fclose(file);
}

void repWriteArtist(){
	ARTIST artist; int choice;
	char options[][30] = {"Sim", "Nao"};

	do{
		readArtists(12);
		artist = setArtist();
		writeArtist(artist);
		
		textcolor(15); textbackground(8);
		gotoxy(endSideBar+1, 8); cout << "Deseja cadastrar outro?";
		gotoxy(endSideBar+2, 9); cout << "Sim";
		gotoxy(endSideBar+2, 10); cout << "Nao";

		choice = menuSelection(9, 10, options);
	}while(choice == 1);
}

void readArtists(int i){
	clearMenuArea();
	
	ARTIST artist;
	FILE * file = fopen(artistsFile, "rb");
	if(!file){
		gotoxy(columns-10, lines); cout << "Erro na abertura de arquivo";
	}
	int j;
	
	textbackground(2); textcolor(15);
	for(j = endSideBar; j<columns; j++){
		gotoxy(j, i); cout << " ";
	}
	gotoxy(endSideBar, i); cout << "ID";
	gotoxy(endSideBar + 10, i); cout << "Nome do Artista";
	gotoxy(endSideBar + 60, i); cout << "Ouvintes mensais";

	textbackground(8); textcolor(0);
	while(fread(&artist, sizeof(artist), 1, file)){
		if(artist.deleted == false){
			gotoxy(endSideBar, ++i); cout << artist.id;
			gotoxy(endSideBar + 10, i); cout << artist.name;
			gotoxy(endSideBar + 60, i); cout << artist.listeners;
		}
	}

	fclose(file);
}

ARTIST editArtist(FILE * file){
	int id;
	ARTIST artistToAlter, artistAltered;
	char aux[11];
	
	printArtistInputMask("Alteracao de Artista", 1);
	
	textcolor(15); textbackground(8);
	
	do{
		gotoxy(endSideBar, 0); cout << " Digite o ID do artista que deseja alterar:                ";
		gotoxy(endSideBar + 44, 0); cin >> id;
		fflush(stdin);

		artistToAlter = findArtist(id, file);
	}while(artistToAlter.id == -1);

	artistAltered.id = artistToAlter.id;

	textbackground(0); textcolor(15);
	gotoxy(endSideBar, 1); cout << " Atualize os dados do artista.";
	gotoxy(endSideBar, 2); cout << " Se nao quiser alterar o campo, apenas pressione enter.";
	
	gotoxy(horizontalCenter + 16, 7); cout << artistToAlter.id;
	
	gotoxy(horizontalCenter + 18, 8); fgets(artistAltered.name, 70, stdin);
	if(artistAltered.name[0] == '\0') {
		strcpy(artistAltered.name, artistToAlter.name);
		gotoxy(horizontalCenter + 18, 8); cout << artistToAlter.name << "                      ";
	}
	gotoxy(horizontalCenter + 30, 9); fgets(aux, 10, stdin);
	if(aux[0] == '\0'){
		artistAltered.listeners = artistToAlter.listeners;
		gotoxy(horizontalCenter + 30, 9); cout << artistToAlter.listeners << "                      ";
	} 
	else artistAltered.listeners = atoi(aux);

	return artistAltered;
}

void writeAlteredArtist(ARTIST artist, FILE * file){
	fseek (file,-sizeof(ARTIST),SEEK_CUR);
	fwrite(&artist, sizeof(ARTIST), 1, file);
	fflush(file);
}

void repEditArtist(){
	FILE * file;
	ARTIST artist;
	int choice;
	char options[][30] = {"Sim", "Nao"};

	do{
		readArtists(12);
		file = fopen(artistsFile, "r+b");
		artist = editArtist(file);
		writeAlteredArtist(artist, file);
		fclose(file);
		
		textcolor(15); textbackground(8);
		gotoxy(endSideBar+1, 8); cout << "Deseja alterar outro?";
		gotoxy(endSideBar+2, 9); cout << "Sim";
		gotoxy(endSideBar+2, 10); cout << "Nao";

		choice = menuSelection(9, 10, options);
	}while(choice == 1);
}

int deleteArtist(FILE * file){
	int id;
	ARTIST artistToDelete;
	
	printArtistInputMask("Exclusao de Artista", 1);
	
	textcolor(15); textbackground(8);
	
	do{
		gotoxy(endSideBar, 0); cout << "Digite o ID do artista que deseja excluir (0 para sair):                ";
		gotoxy(endSideBar + 57, 0); cin >> id;
		if(id == 0) return -1;
		fflush(stdin);

		artistToDelete = findArtist(id, file);
	}while(artistToDelete.id == -1);

	textcolor(15); textbackground(0);
	gotoxy(horizontalCenter + 16, 7); cout << artistToDelete.id;
	gotoxy(horizontalCenter + 18, 8); cout << artistToDelete.name;
	gotoxy(horizontalCenter + 30, 9); cout << artistToDelete.listeners;

	char options[][30] = {"Sim", "Nao"};
	int choice;
	
	textcolor(15); textbackground(8);
	gotoxy(endSideBar+1, 8); cout << "Deseja excluir esse artista?";
	gotoxy(endSideBar+2, 9); cout << "Sim";
	gotoxy(endSideBar+2, 10); cout << "Nao";

	choice = menuSelection(9, 10, options);
	
	if(choice == 1) artistToDelete.deleted = true;
	else artistToDelete.deleted = false;

	writeAlteredArtist(artistToDelete, file);

	return 1;
}

void repDeleteArtist(){
	FILE * file;
	ARTIST artist;
	int choice;
	
	do{
		readArtists(12);
		file = fopen(artistsFile, "r+b");
		choice = deleteArtist(file);
		fclose(file);
	}while(choice == 1);
}


bool checkArtistId(int id) {
	FILE * file = fopen(artistsFile, "rb");
	
	bool flag = true;
	ARTIST artist;

	while(fread(&artist, sizeof(artist), 1, file)){
		if (id == artist.id && artist.deleted == false) {
			flag = false;
			break;
		}
	}
	fclose(file);

	return flag;
}

ARTIST findArtist(int id, FILE * file){
	rewind(file);
	ARTIST artist;

	while(fread(&artist, sizeof(artist), 1, file)){
		if (id == artist.id && artist.deleted == false) return artist;
	}

	artist.id = -1;
	strcpy(artist.name, " ");
	artist.listeners = 0;
	artist.deleted = false;

	return artist;
}

ARTIST checkArtistByName(char * name) {
	FILE * file = fopen(artistsFile, "rb");
	
	bool flag = false;
	ARTIST artist;
	ARTIST nullARTIST = {-1, "NULL", -1};
	char filteredArtistName[71], filteredName[71];

	flag = false;
	while(fread(&artist, sizeof(artist), 1, file)){
		for (int i = 0, j = 0; i < strlen(artist.name); i++) {
			char auxChar = tolower(artist.name[i]);
			if(auxChar >= 'a' && auxChar <= 'z' || auxChar >= '0' && auxChar <= 9) {
				filteredArtistName[j] = auxChar;
				j++;
			}
		}

		for (int i = 0, j = 0; i < strlen(name); i++) {
			char auxChar = tolower(name[i]);
			if(auxChar >= 'a' && auxChar <= 'z' || auxChar >= '0' && auxChar <= 9) {
				filteredName[j] = auxChar;
				j++;
			}
		}

		if (!strcmpi(filteredArtistName, filteredName) && artist.deleted == false) {
			flag = true;
			break;
		}
	}
	
	fclose(file);
	
	if (flag) return artist;
	else return nullARTIST;
}

bool checkMusicId(int id) {
	FILE * file = fopen(musicsFile, "rb");
	
	bool flag = true;
	MUSIC music;

	while(fread(&music, sizeof(music), 1, file)){
		if (id == music.id && music.deleted == false) {			
			flag = false;
			break;
		}
	}
	fclose(file);

	return flag;
}

///music session
MUSIC setMusic() {
	MUSIC music;
	int auxInt;
	char auxStr[71];
	ARTIST auxArtist;
	char trash;

	printMusicInputMask("Cadastro de música", 1);

	//input
	do {
		gotoxy(horizontalCenter + 8, 4); cout << "              ";
		gotoxy(horizontalCenter + 8, 4); cin >> auxInt;
		fflush(stdin);
	} while (!checkMusicId(auxInt)); //check if id already exists
	music.id = auxInt;

	do {
		gotoxy(horizontalCenter + 21, 5); cout << "                  ";
		gotoxy(horizontalCenter + 21, 5); fgets(auxStr, 70, stdin);
		fflush(stdin);
		auxArtist = checkArtistByName(auxStr);
	} while(auxArtist.id == -1);
	music.artist = auxArtist;

	gotoxy(horizontalCenter + 23, 6); cin >> music.duration.mins;
	gotoxy(horizontalCenter + 26, 6); cin >> music.duration.secs;
	fflush(stdin);
	gotoxy(horizontalCenter + 21, 7); fgets(music.title, 70, stdin);
	fflush(stdin);
	gotoxy(horizontalCenter + 37, 8);  cin >> music.release.d;
	gotoxy(horizontalCenter + 40, 8); cin >> music.release.m;
	gotoxy(horizontalCenter + 43, 8); cin >> music.release.y;
	
	return music;
}


void writeMusic(MUSIC music){
	FILE * file;
	file = fopen(musicsFile, "ab");
	fwrite(&music, sizeof(MUSIC), 1, file);
	fclose(file);
}

void repWriteMusic(){
	MUSIC music; int choice;
	char options[][30] = {"Sim", "Nao"};

	do{
		readMusics(12);
		music = setMusic();
		writeMusic(music);
		
		textcolor(15); textbackground(8);
		gotoxy(endSideBar+1, 8); cout << "Deseja cadastrar outro?";
		gotoxy(endSideBar+2, 9); cout << "Sim";
		gotoxy(endSideBar+2, 10); cout << "Nao";

		choice = menuSelection(9, 10, options);
	}while(choice == 1);
}

void readMusics(int i) {
	clearMenuArea();
	
	MUSIC music;
	FILE * file = fopen(musicsFile, "rb");
	if(!file){
		gotoxy(columns-10, lines); cout << "Erro na abertura de arquivo";
	}
	int j;
	
	textbackground(2); textcolor(15);
	for(j = endSideBar; j<columns; j++){
		gotoxy(j, i); cout << " ";
	}
	gotoxy(endSideBar, i); cout << "ID";
	gotoxy(endSideBar + 10, i); cout << "Musica";
	gotoxy(endSideBar + 35, i); cout << "Artista";
	gotoxy(endSideBar + 60, i); cout << "Duracao";

	textbackground(8); textcolor(0);
	while(fread(&music, sizeof(music), 1, file)){
		if(music.deleted == false){
			gotoxy(endSideBar, ++i); cout << music.id;
			gotoxy(endSideBar + 10, i); cout << music.title;
			gotoxy(endSideBar + 35, i); cout << music.artist.name;
			gotoxy(endSideBar + 60, i); cout << music.duration.mins << ":" << music.duration.secs;
		}
	}

	fclose(file);
}
//TODO:
// ARTIST editMusic(FILE * file){
// 	int id;
// 	MUSIC musicToAlter, musicAltered;
// 	char aux[11];
	
// 	printArtistInputMask("Alteracao de Musica", 1);
	
// 	textcolor(15); textbackground(8);
	
// 	do{
// 		gotoxy(endSideBar, 0); cout << " Digite o ID do musica que deseja alterar:                ";
// 		gotoxy(endSideBar + 44, 0); cin >> id;
// 		fflush(stdin);

// 		//musicToAlter = findArtist(id, file);
// 	}while(musicToAlter.id == -1);

// 	musicAltered.id = musicToAlter.id;

// 	textcolor(0); textcolor(15);
// 	gotoxy(endSideBar, 1); cout << " Atualize os dados do musica.";
// 	gotoxy(endSideBar, 2); cout << " Se nao quiser alterar o campo, apenas pressione enter.";
	
// 	gotoxy(horizontalCenter + 16, 7); cout << musicToAlter.id;
	
// 	gotoxy(horizontalCenter + 18, 8); fgets(musicAltered.name, 70, stdin);
// 	if(musicAltered.name[0] == '\0') {
// 		strcpy(musicAltered.name, musicToAlter.name);
// 		gotoxy(horizontalCenter + 18, 8); cout << musicToAlter.name << "                      ";
// 	}
// 	gotoxy(horizontalCenter + 30, 9); fgets(aux, 10, stdin);
// 	if(aux[0] == '\0'){
// 		musicAltered.listeners = musicToAlter.listeners;
// 		gotoxy(horizontalCenter + 30, 9); cout << musicToAlter.listeners << "                      ";
// 	} 
// 	else artistAltered.listeners = atoi(aux);

// 	return artistAltered;
// }
// //TODO:
// void writeAlteredMusic(ARTIST artist, FILE * file){
// 	fseek (file,-sizeof(ARTIST),SEEK_CUR);
// 	fwrite(&artist, sizeof(ARTIST), 1, file);
// 	fflush(file);
// }
// //TODO:
// void repEditMusic(){
// 	FILE * file;
// 	ARTIST artist;
// 	int choice;
// 	char options[][30] = {"Sim", "Nao"};

// 	do{
// 		readArtists(12);
// 		file = fopen(artistsFile, "r+b");
// 		artist = editArtist(file);
// 		writeAlteredArtist(artist, file);
// 		fclose(file);
		
// 		textcolor(15); textbackground(8);
// 		gotoxy(endSideBar+1, 8); cout << "Deseja alterar outro?";
// 		gotoxy(endSideBar+2, 9); cout << "Sim";
// 		gotoxy(endSideBar+2, 10); cout << "Nao";

// 		choice = menuSelection(9, 10, options);
// 	}while(choice == 1);
// }

MUSIC findMusic(int id, FILE * file){
	rewind(file);
	ARTIST nullARTIST = {-1, "NULL", -1};
	MUSIC music, nullMUSIC = {-1, nullARTIST, {0,0}, "NULL", {0,0,0}, false};

	while(fread(&music, sizeof(music), 1, file)){
		if (id == music.id && music.deleted == false) return music;
	}

	return nullMUSIC;
}

//menus
int mainMenuSelection(int rowMin, int rowMax, char options[][30]){
	cursor(0);
    int choice = 0;
    int aux = rowMin;
    char Cursor;

    gotoxy(3,aux);
    textbackground(15); textcolor(0); cout << options[aux - rowMin];
    while (!choice) {
        Cursor = getch();
        gotoxy(3,aux);
        textbackground(0); textcolor(15); cout << options[aux - rowMin];

        switch(Cursor){
        	case upArrow:
        		if(aux == rowMin) aux = rowMax;
                    else aux --;
                    break;
            case downArrow:
            	if(aux == rowMax) aux = rowMin;
                    else aux++;
                    break;
            case enter:
            	choice = aux-rowMin+1;
            	break;
            case esc:
            	choice = -1;
            	break;
		}
		
		gotoxy(3,aux);
		textbackground(15); textcolor(0); cout << options[aux - rowMin];
    }

    cursor(1);
    return choice;
}

int mainMenu(){
	char options[][30] = {"Menu de musicas", "Menu de artistas", "Menu de playlists", "Ajuda (F1)", "Sair"};
	
	clearMainMenuArea();
	gotoxy(2, 9); cout << "Opcoes: ";
	
	int i;
	for(i=10; i<15; i++){
		gotoxy(3, i); cout << options[i-10];
	}
	
	return mainMenuSelection(10, 14, options);
}

int menuSelection(int rowMin, int rowMax, char options[][30]){
	cursor(0);
    int choice = 0;
    int aux = rowMin;
    char Cursor;

    gotoxy(endSideBar + 2,aux);
    textbackground(2); textcolor(15); cout << options[aux - rowMin];
    while (!choice) {
        Cursor = getch();
        gotoxy(endSideBar + 2,aux);
        textbackground(8); textcolor(15); cout << options[aux - rowMin];

        switch(Cursor){
        	case upArrow:
        		if(aux == rowMin) aux = rowMax;
                    else aux --;
                    break;
            case downArrow:
            	if(aux == rowMax) aux = rowMin;
                    else aux++;
                    break;
            case enter:
            	choice = aux-rowMin+1;
            	break;
            case esc:
            	choice = -1;
            	break;
		}
		
		gotoxy(endSideBar + 2,aux);
		textbackground(2); textcolor(15); cout << options[aux - rowMin];
    }

    cursor(1);
    return choice;
}


void repDeleteMusic(){}

void menuMusicExecute(){
	int choice = 0;
	while(choice >= 0 && choice < 6){
		choice = menuMusic();
		switch(choice){
			case 1:
				repWriteMusic();
				break;
			case 2:
				readMusics(1);
				getch();
				break;
			case 3:
				//TODO:
				// repEditMusic();
				break;
			case 4:
				//TODO:
				// repDeleteMusic();
				break;
		}
	}
}

int menuMusic(){
	char options[][30] = {"Cadastrar Musicas", "Visualizar Musicas", "Alterar musica", "Excluir musica", "Ajuda (F1)", "Voltar"};
	int choice = 0;
	
	clearMenuArea();

	textbackground(8); textcolor(15);
	
	gotoxy(41, 1); cout << "Opcoes: ";
	
	int i;
	for(i=2; i<8; i++){
		gotoxy(42, i); cout << options[i-2];
	}
	
	choice = menuSelection(2, 7, options);
	return choice;
}



void menuArtistExecute(){
	int choice = 0;
	while(choice >= 0 && choice < 6){
		choice = menuArtist();
		switch(choice){
			case 1:
				repWriteArtist();
				break;
			case 2:
				readArtists(1);
				getch();
				break;
			case 3:
				repEditArtist();
				break;
			case 4:
				repDeleteArtist();
				break;
		}
	}
}

int menuArtist(){
	char options[][30] = {"Cadastrar artista", "Visualizar artistas", "Alterar artista", "Excluir artista", "Ajuda (F1)", "Voltar"};
	int choice = 0;
	
	clearMenuArea();

	textbackground(8); textcolor(15);
	
	gotoxy(41, 1); cout << "Opcoes: ";
	
	int i;
	for(i=2; i<8; i++){
		gotoxy(42, i); cout << options[i-2];
	}
	
	return menuSelection(2, 7, options);
}                   
                                                      
//interface
void printHeader(){
	system("cls");
	int i, j;
	
	//color on the screen
	for(i=0; i<lines; i++){
		for(j=0; j<columns; j++){
			if(j < endSideBar) textbackground(0);
			else textbackground(8);
			gotoxy(j, i); cout << " ";
		}
	}
	
	//header
	gotoxy(0, 0);
	textbackground(0); textcolor(15);
	cout << 
	"   _____             _   _  __       \n" <<
	"  /  ___|           | | (_)/ _|      \n" <<
	"  \\ `--.  ___  _ __ | |_ _| |_ _   _ \n" <<
	"   `--. \\/ _ \\| '_ \\| __| |  _| | | |\n" <<
	"  /\\__/ / (_) | |_) | |_| | | | |_| |\n" <<
	"  \\____/ \\___/| .__/ \\__|_|_|  \\__, |\n" <<
	"              | |               __/ |\n" <<
	"              |_|              |___/ \n";
}

void clearMainMenuArea(){
	textbackground(0); textcolor(15);
	int i, j;
	for(i=9; i<lines; i++){
		for(j=0;j<endSideBar; j++){
			gotoxy(j, i); cout << " ";
		}
	}
}

void clearMenuArea(){
	textbackground(8); textcolor(15);
	int i, j;
	for(i=0; i<lines; i++){
		for(j=endSideBar;j<columns; j++){
			gotoxy(j, i); cout << " ";
		}
	}
}

void printArtistInputMask(string header, int i){
	int j, k;
	textbackground(0); textcolor(15);
	for(k = i; k<i+7; k++){
		for(j = endSideBar+1; j<columns-1; j++){
			gotoxy(j, k); cout << " ";
		}
	}
	
	//input mask
	textbackground(2);
	gotoxy(horizontalCenter + header.size()/2, i+1); cout << header;
	textbackground(0);
	gotoxy(horizontalCenter + 12, i+3); cout << "ID: ";
	gotoxy(horizontalCenter + 12, i+4); cout << "Nome: ";
	gotoxy(horizontalCenter + 12, i+5); cout << "Ouvintes mensais: ";

}

void printMusicInputMask(string header, int i){
	int j, k;
	textbackground(0); textcolor(15);
	for(k = i; k<i+9; k++){
		for(j = endSideBar+1; j<columns-1; j++){
			gotoxy(j, k); cout << " ";
		}
	}
	
	//input mask
	textbackground(2);
	gotoxy(horizontalCenter + header.size()/2, i+1); cout << header;
	textbackground(0);
	gotoxy(horizontalCenter + 4, i+3); cout << "ID: ";
	gotoxy(horizontalCenter + 4, i+4); cout << "Nome do artista: ";
	gotoxy(horizontalCenter + 4, i+5); cout << "Duracao (min:seg):   :";
	gotoxy(horizontalCenter + 4, i+6); cout << "Titulo da faixa: ";
	gotoxy(horizontalCenter + 4, i+7); cout << "Data de lancamento (dd/mm/aaaa):   /  /";
}



