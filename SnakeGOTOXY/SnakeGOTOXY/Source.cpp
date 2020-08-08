#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <ctime>
#include <Windows.h>
#include <conio.h>

using namespace std;

void gotoxy(int xpos, int ypos)
{
	COORD scrn;

	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);

	scrn.X = xpos; scrn.Y = ypos;

	SetConsoleCursorPosition(hOuput, scrn);
}

class SnakeGame
{
private:
	vector<int> SnakeHead; //Coordonatele x,y a capului
	vector<int> SnakeTail; //Vectorul corpului
	vector <int> Mines;
	vector <int> Enemy1;
	vector <int> Enemy2;
	int AppleCoordX; //Coord marului X
	int AppleCoordY; //Corod marului Y
	char MineCoordX;
	char MineCoordY;
	int Score; //Punctajul
	float Time; //Timpul
	int Difficulty;
	short int SnakeLength; // Lungimea sarpelui
	int SnakeSpeed; // Viteza sarpelui
	int x, y; //pozitia capului
public:
	SnakeGame()
	{
		Initialization();
	}
	~SnakeGame(){};

	void Initialization();
	void CheatPanel();
	void Move(); //Mutam sarpele
	void AddObjects();//Adaogam marul
	bool FoundApple();//Am gasit marul! Punem altul, marin lungimea sarpelui
	bool Game_end(); //Am pierdut?
	void Draw_Field(char HeadSide); //Aratam tabla, inoim
	void GameTable(); // Cele mai bune rezultate
	void SetCoord(char Side, char OldSide, int &x, int &y); //Schimbam coordonatele serpelui
	void OutsideCoord(); //Controlam daca suntem dupa granita
	void ClearTrack(char Side, char OldSide, int &contor);
	void Continue();
};

void SnakeGame::CheatPanel(){
	int choose = 0;
	system("cls");
	cout << "Modul: " << Difficulty << " | Viteza: " << SnakeSpeed << "| Lungimea: " << SnakeLength << endl;
	cout << "1. Marire viteza\n";
	cout << "2. Schimbam modul\n";
	cout << "3. Schimbam lungimea sarpelui\n";
	cout << "4. Iesire\n";
	cin >> choose;
	switch(choose){
	case 1: {
				int alegere = 0;
				cout << "Viteza noua? ";
				cin >> alegere;
				SnakeSpeed = alegere;
				break;
	}
	case 2:{
			   int alegere = 0;
			/*  cout << "Alege modul nou\n";
			   cout << "1. Easy\n";
			   cout << "2. Normal\n";
			   cout << "3. Hard\n";
			   cin >> alegere;*/
			   Initialization();
			   break;
	}
	case 3:{
			   int alegere = 0;
			   cout << "Lungimea noua: ";
			   cin >> alegere;
			   SnakeLength = alegere;
			   break;
	}
	case 4:{
			   cout << "Iesim...\n";
			   system("cls");
			   Move();
	}
	default: {
				 cout << "Opeatie inexistenta!\n";
	}
	}
	CheatPanel();
}

void SnakeGame::Initialization(){
	x = 2;
	y = 2;
	cout << "Hello! Choose difficulty\n";
	cout << "1. Easy\n";
	cout << "2. Normal\n";
	cout << "3. Hard\n";
	cin >> Difficulty; 
	system("cls");
	fflush(stdin);

//Daca numarul e gresit
	if (Difficulty != 1 && Difficulty != 2 && Difficulty != 3){
		system("cls");  Initialization();
	}

//Adaogam marul
	AppleCoordX = rand() % 22 + 1; //Coordonata X a marului
	AppleCoordY = rand() % 22 + 1; //Coordonata Y a marului
//Controlam coord.Marului=coord.Capului. Daca da - schimbam coordonatele
	if (AppleCoordX == x && AppleCoordY == y)
		AddObjects();
	else if (AppleCoordX == x && AppleCoordY == y+1)
		AddObjects();
	else
		gotoxy(AppleCoordX, AppleCoordY); cout << '*';

//////////////////DACA [Modul == 2]
	if (Difficulty == 2){
//Daca vectorul cu mini nu e gol - golim
		if (!Mines.empty())
			Mines.erase(Mines.begin(), Mines.end());

//Alegem coordonatele minelor[3 mini] si le adaogam in vector, verificand daca nu nu sunt egale cu coord. capului
		for (int a = 1; a <= 5; a+=2){
			MineCoordX = rand() % 23 + 1;
			MineCoordY = rand() % 23 + 1;
			if (MineCoordX == x && MineCoordY == y) AddObjects();
			if (MineCoordX == x && MineCoordY == y+1) AddObjects();
			Mines.insert(Mines.end(), MineCoordX);
			Mines.insert(Mines.end(), MineCoordY);
		}

//Afisam minele
		for (int a = 1; a <= 5; a += 2){
			gotoxy(Mines[a - 1], Mines[a]); cout << 'x';
		}
	}
//Stergem vectorii corpului si capului, daca are elemente
	if (!SnakeHead.empty())
		SnakeHead.erase(SnakeHead.begin(), SnakeHead.end());
	if (!SnakeTail.empty())
		SnakeTail.erase(SnakeTail.begin(), SnakeTail.end());

//Initializam Scorul, viteza si lungimea
	Score = 0;
	SnakeLength = 3;
	SnakeSpeed = 300;
//Introducem coordonatele initiala e capului si a corpului
	
	SnakeTail.insert(SnakeTail.end(), x);
	SnakeTail.insert(SnakeTail.end(), y);
	SnakeHead.insert(SnakeHead.begin(), x);
	SnakeHead.insert(SnakeHead.end(), y + 1);
	SnakeTail.shrink_to_fit();
	SnakeHead.shrink_to_fit();
}

void SnakeGame::Move(){
	SnakeHead.shrink_to_fit();
	SnakeTail.shrink_to_fit();
	Mines.shrink_to_fit();
	static char Side = 'D'; //Directia in care se duce sarpele
	Time = clock() / CLOCKS_PER_SEC; //Timpul de joaca
	static char OldSide = 'D'; //Pozitia veche, pozitia initiala
//Aratam scorul, timpul, control-pad	
	gotoxy(30, 0);  cout << "Score: " << Score << " || " << "Time: " << Time << " sec" << endl;
	gotoxy(30, 1); cout << "--Control--";
	gotoxy(30, 2); cout << "W - up";
	gotoxy(30, 3); cout << "S - down";
	gotoxy(30, 4); cout << "A - left";
	gotoxy(30, 5); cout << "D - right";
	Draw_Field(Side);
	
	char aux = 'r';
//Introducem directia in care vrem sa mergem 
	while (_kbhit()){
		fflush(stdin);
		aux = _getch();
//Daca am introdus litera mica, o "marim"; E - iesire; Controlam daca e introdusa litera corecta
		if (aux > 90) aux = aux - 32;
//Daca e W/A/S/D - schimbam directia; daca E - iesim; daca nu introducem nimic - mergem spre directia veche
		if (aux == 'W' || aux == 'A' || aux == 'S' || aux == 'D')
		{
			Side = aux; break;
		}
		else if (aux == 'E')
		{
			cout << "Exit...\n"; getchar();  exit(0);
		}
		else if (aux == 'P' || aux == 'p')
			CheatPanel();
		else
		{
			Side = Side; break;
		}
	}

	Sleep(SnakeSpeed); //Asteptam [SnakeSpeed] milisecunde

//Daca directia e contrara, nu o schimbam
	if (
		(OldSide == 'W' && Side == 'S') ||
		(OldSide == 'A' && Side == 'D') ||
		(OldSide == 'S' && Side == 'W') ||
		(OldSide == 'D' && Side == 'A')
		)
	{	Side = OldSide;  Move();  }
	SetCoord(Side, OldSide, x, y); //Schimbam, stergem, inoim coordonatele
	bool LostGame = Game_end(); //Am pierdut?
	LostGame == 1 ? GameTable() : 0; //Am pierdut? Da-afisam timpul si scorul; Nu - continuam
	OldSide = Side; //Schimbam starea vechii directii
	bool Found = FoundApple(); //Am gasit marul?
	Found == 1 ? AddObjects() : 0; //Am gasit marul! Adaogam altul, marim scorul, lungimea sarpelui
	Move(); //....si iarasi din nou...
}

void SnakeGame::SetCoord(char Side, char OldSide, int &x, int &y){
	static int a=2;
	if (Score == 0) a = 2;
	//Setam noile coordonate, cele vechi le stergem
	if (Side == 'W'){
		gotoxy(x, y-1); cout << 'A';
		gotoxy(x, y);  cout << 'o';
		ClearTrack(Side, OldSide, a); //Stergem "urma" sarpelui
		SnakeTail.insert(SnakeTail.end(), x); //Inseram coordonata noua X a corpului
		SnakeTail.insert(SnakeTail.end(), y); //Inseram coordonata noua Y a corpului
		x = x; //Cooordonata X noua a capului
		y -= 1; //Coordonata Y noua a capului
	}
	else if (Side == 'S'){
		gotoxy(x, y+1); cout << 'V';
		gotoxy(x, y);  cout << 'o';
		ClearTrack(Side, OldSide, a);
		SnakeTail.insert(SnakeTail.end(), x);
		SnakeTail.insert(SnakeTail.end(), y);
		x = x;
		y = y+1;

	}
	else if (Side == 'A'){
		gotoxy(x-1, y); cout << '<';
		gotoxy(x, y);  cout << 'o';
		ClearTrack(Side, OldSide, a);
		SnakeTail.insert(SnakeTail.end(), x);
		SnakeTail.insert(SnakeTail.end(), y);
		x -= 1;
		y = y;
	}
	else if (Side == 'D'){
		gotoxy(x+1, y); cout << '>';
		gotoxy(x, y);  cout << 'o';
		ClearTrack(Side, OldSide, a);
		SnakeTail.insert(SnakeTail.end(), x);
		SnakeTail.insert(SnakeTail.end(), y);
		x += 1;
		y = y;
	}

	SnakeHead.erase(SnakeHead.begin(), SnakeHead.end()); //Stergem coordonatele capului veche
	SnakeHead.insert(SnakeHead.begin(), x); //Stergem coordonata X veche a capului
	SnakeHead.insert(SnakeHead.end(), y); //Stergem coordonata Y veche a capului

	OutsideCoord(); //Am iesit din granita?
}

void SnakeGame::Draw_Field(char Side){
	for (int a = 0; a < 25; ++a){
		gotoxy(a, 0); cout << "@";
		gotoxy(a, 24); cout << "@";
		gotoxy(0, a); cout << "@";
		gotoxy(24, a); cout << "@";
	}
}

void SnakeGame::ClearTrack(char Side, char OldSide, int &a){
	//Stergem coordonatele vechi a sarpelui
	if (SnakeLength == 2){
		//	cout << x << " : " << y << endl;
		SnakeTail.erase(SnakeTail.begin(), SnakeTail.begin() + 2); //Stergem ultima celula din coada
		gotoxy(50, 10); cout << "D";
		gotoxy(51, 10); cout << "A";
		Sleep(50);
		gotoxy(50, 10); cout << " ";
		gotoxy(51, 10); cout << " ";
		//Stergem urma coadei in dependenta de directie, daca lungimea sarpelui e 2
		if (OldSide == 'W' && (Side == 'W' || Side == 'A' || Side == 'D')){
			gotoxy(x+1, y); cout << ' ';
		}
		else if (OldSide == 'A' && (Side == 'W' || Side == 'S' || Side == 'A')){
			gotoxy(x, y + 1); cout << ' ';
		}
		else if (OldSide == 'S' && (Side == 'D' || Side == 'A' || Side == 'S')){
			gotoxy(x - 1, y); cout << ' ';
		}
		else if (OldSide == 'D' && (Side == 'W' || Side == 'S' || Side == 'D')){
			gotoxy(x, y - 1); cout << ' ';
		}
	}
	else if (a < SnakeLength-1) //Nu stergem nimic pana nu s-au desenat toate partile a sarpelui
		a++;
	else if (SnakeLength >= 3){
//Stergem urma coadei in dependenta de directie, daca lungimea sarpelui e >=3
		SnakeTail.shrink_to_fit();
		int TailX = SnakeTail[0];
		int TailY = SnakeTail[1];
		gotoxy(TailX, TailY); cout << ' ';
		SnakeTail.erase(SnakeTail.begin(), SnakeTail.begin() + 2);
	}
}

void SnakeGame::AddObjects(){
	static int contor = 0;

//Adaogam marul
		AppleCoordX = rand() % 22 + 1; //Coordonata X a marului
		AppleCoordY = rand() % 22 + 1; //Coordonata Y a marului
//Controlam coord.Marului=coord.Capului. Daca da - schimbam coordonatele
		if (AppleCoordX == SnakeHead[0] && AppleCoordY == SnakeHead[1]) AddObjects();

		for (int a = 1; a != SnakeTail.capacity()-1; a+=2){
			if (AppleCoordX == SnakeTail[a - 1] && AppleCoordY == SnakeTail[a]){
				AppleCoordX = rand() % 22 + 1;
				AppleCoordY = rand() % 22 + 1;
			}
		}
		gotoxy(AppleCoordX, AppleCoordY); cout << '*';

//////////////////DACA [Modul == 2]
		if ((Difficulty == 2) && (contor == 6)){
//Adaogam inca o mina
			MineCoordX = rand() % 22 + 1;
			MineCoordY = rand() % 22 + 1;
			
//Controlam coord.minelor = coord.Corpului SAU coord. Capului. Daca da - schimbam coordonatele
			if ((MineCoordX == SnakeHead[0]) && (MineCoordY == SnakeHead[1])) AddObjects();

			for (int a = 1; a != SnakeTail.capacity()-1; a += 2)
			if ((MineCoordX == SnakeTail[a - 1]) && (MineCoordY == SnakeTail[a]))
				AddObjects();
//Daca totul e bine, inseram in vector
			Mines.insert(Mines.end(), MineCoordX);
			Mines.insert(Mines.end(), MineCoordY);
//Afisam mina
				gotoxy(MineCoordX, MineCoordY); cout << 'x';
			contor = 0;
		}
	contor++;
}

bool SnakeGame::FoundApple(){
	//Controlam daca am gasit marul. Returnam 1 daca e gasit, 0 - nu
	if (x == AppleCoordX && y == AppleCoordY){
		Score += 10;
		SnakeLength++;
		if ((SnakeLength % 5) == 0 && Difficulty == 1) SnakeSpeed -= 20;
		else if ((SnakeLength % 5) == 0 && Difficulty == 2) SnakeSpeed -= 30;
		else if ((SnakeLength % 5) == 0 && Difficulty == 3) SnakeSpeed -= 40;
		return 1;
	}
	else
		return 0;
}

void SnakeGame::OutsideCoord(){
	//Controlam daca am iesit din granita terenului. Daca da, trecem pe partea vizavida
	if (SnakeLength == 2){
		if (x == 24){
			gotoxy(1, y); cout << 'V';
			gotoxy(x - 1, y); cout << ' ';
			gotoxy(x - 2, y); cout << ' ';
			x = 1; y = y;
		}
		else if (x <= 1){
			gotoxy(23, y); cout << 'A';
			gotoxy(x + 1, y); cout << ' ';
			gotoxy(x + 2, y); cout << ' ';
			x = 23; y = y;
		}
		else if (y >= 24){
			gotoxy(x, 1); cout << '>';
			gotoxy(x, y - 1); cout << ' ';
			gotoxy(x, y - 2); cout << ' ';
			y = 1; x = x;
		}
		else if (y <= 1){
			gotoxy(x, 23); cout << '<';
			gotoxy(x, y + 1); cout << ' ';
			gotoxy(x, y + 2); cout << ' ';
			y = 23; x = x;
		}
	}
	else
	if (x >= 24)	  { x = 1; y = y; }
	else if (x <= 0)  { x = 23; y = y; }
	else if (y >= 24) { y = 1; x = x; }
	else if (y <= 0)  { y = 23; x = x; }
}

bool SnakeGame::Game_end(){
	//Controlam daca am pierdut
	SnakeTail.shrink_to_fit();
	SnakeHead.shrink_to_fit();
	char HeadX = SnakeHead[0];
	char HeadY = SnakeHead[1];
	//Daca capul = corpul - am pierdut
	if ((HeadX == SnakeTail[0]) && (HeadY == SnakeTail[1])) return 1;

	for (int it = 1; it != Mines.capacity()-1; it+=2)
		if (Mines[it - 1] == HeadX && Mines[it] == HeadY)
			return 1;

	for (int a = 1; a != SnakeTail.capacity()-1; a+=2){
		if ( (HeadX == SnakeTail[a - 1]) && (HeadY == SnakeTail[a]) )
			return 1;
	}
	return 0;
}

void  SnakeGame::GameTable(){
	//Afisam ca am pierdut + tabela de rezultate[in curs de dezvoltare]
	system("cls");
	fstream Champions("Champions.r0", ios_base::in | ios_base::out | ios_base::_Nocreate);
	cout << "\t\t\t\t\t\tScore: " << Score << " | Time: " << Time << endl;
	cout << "\t\t\t\t\t\t   |GAME OVER|\n";

	if (Champions.fail()){
		Champions.close();
		fstream Champions("Champions.r0", ios_base::out | ios_base::trunc);

		cout << "\t\t\t\t\t\t|   " << 1 << ". " << Score << "\t|" << endl; //Nu sunt rezultate, al nostru e primul
		Champions << Score << endl; //Scriem in file
//Introducem toate rezultatele in file
//Find ca e prima joaca, va fi primul scor si restul tot 0
		for (int a = 0; a < 9; a++){
			cout << "\t\t\t\t\t\t|   " << a + 1 << ". " << "0" << "\t|" << endl;
			Champions << "0" << endl;
		}
	}
	else{
		fstream Champions("Champions.r0", ios_base::in | ios_base::out | ios_base::_Nocreate);
		vector <int> Punctele(11);
		int contor = 0;
		int a = 0;

/////////////// Scriem punctele in vector
		while (!(Champions.eof())){
			Champions >> Punctele[contor];
			contor++;
		}
//////////////Sortam descrescator elementele
		contor = 1;
		
		while (contor != 0){
			contor = 0;
			for (int a = 1; a < 10; ++a)
			if (Punctele[a]>Punctele[a - 1]){
				swap(Punctele[a], Punctele[a - 1]);
				contor = 1;
			}
		}
		
		for (int a = 0; a < 10; ++a){
			if (Punctele[a] >= 10 && Punctele[a + 1] == 0){
				Punctele[a + 1] = Score; break;
			}
			else if (Score > Punctele[a]){
				Punctele[a] = Score;
				break;
			}
		}
		
///////////////
		Champions.close();
		fstream Champions2("Champions.r0", ios_base::out | ios_base::trunc);

		for (int a = 0; a < 10; a++){
			cout << "\t\t\t\t\t\t|.   " << a + 1 << ". " << Punctele[a] << "\t\t\t.|" << endl;
			Champions2 << Punctele[a] << endl;
		}
	}
	Champions.close();

	Continue();

	fflush(stdin);
	getchar();
	exit(0);
}

void SnakeGame::Continue(){
	char Again = 't';
	cout << "Again/Exit(A/E): ";
	cin >> Again;
	if (Again == 'a' || Again == 'A'){
		Initialization();
		Move();
	}
	else if (Again == 'E' || Again == 'e'){
		system("cls");
		cout << "GOOD BYE!\n";
		system("pause");
		exit(0);
	}
	else{
		cout << "Error! Wrong character\n";
		Continue();
	}
}

int main(void){
	srand(time_t(NULL));
	SnakeGame SG1;
	SG1.Move();
//	SG1.GameTable();

	getchar();
	return 0;
}