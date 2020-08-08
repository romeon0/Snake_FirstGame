#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <ctime>
#include <Windows.h>
#include <conio.h>

using namespace std;

class SnakeGame
{
private:
	char Field[25][25]; //Terenul de joaca
	vector<int> SnakeHead; //Coordonatele x,y a capului
	vector<int> SnakeTail; //Vectorul corpului
	char Apple; //Marul! Neam-neam...
	int AppleCoordX; //Coord marului X
	int AppleCoordY; //Corod marului Y
	int Score; //Punctajul
	float Time; //Timpul
	short int SnakeLength; // Lungimea sarpelui
	int SnakeSpeed; // Viteza sarpelui
	int x = 2, y = 2; //pozitia capului
public:
	SnakeGame()
	{
		for (int a = 0; a < 25; ++a)
			for (int b = 0; b < 25; ++b) {
				if (a == 0) Field[a][b] = '@';
				else if (b == 0) Field[a][b] = '@';
				else if (a == 24) Field[a][b] = '@';
				else if (b == 24) Field[a][b] = '@';
				else Field[a][b] = ' ';
			}
		Apple = '6';
		Score = 0;
		SnakeLength = 3;
		//Introducem coordonatele initiala e capului si a corpului
		SnakeTail.insert(SnakeTail.end(), x);
		SnakeTail.insert(SnakeTail.end(), y);
		SnakeHead.insert(SnakeHead.begin(), x);
		SnakeHead.insert(SnakeHead.end(), y + 1);

		Field[2][2] = '5';//Pozitia initiala corpului
		Field[2][3] = 'D'; //Pozitia initiala a capului
						   //Intializam marul
		AppleCoordX = rand() % 22 + 1;
		AppleCoordY = rand() % 22 + 1;

		Field[AppleCoordX][AppleCoordY] = '*';
		SnakeSpeed = 200;
	}
	~SnakeGame() {};

	void Move(); //Mutam sarpele
	void AddApple();//Adaogam marul
	bool FoundApple();//Am gasit marul! Punem altul, marin lungimea sarpelui
	bool Game_end(); //Am pierdut?
	void Draw_Field(char HeadSide); //Aratam tabla, inoim
	void GameInfo(); //Scorul, timpul si mere mancate
	void GameTable(); // Cele mai bune rezultate
	void SetCoord(char Side, char OldSide, int &x, int &y); //Schimbam coordonatele serpelui
	void OutsideCoord(); //Controlam daca suntem dupa granita
	void ClearTrack(char Side, char OldSide, int &contor);
};

void SnakeGame::Move() {
	static char Side = 'D'; //Directia in care se duce sarpele
	Time = clock() / CLOCKS_PER_SEC; //Timpul de joaca
	static char OldSide = 'D'; //Pozitia veche, pozitia initiala

							   //Inoim, afisam terenul; Introducem directia in care vrem sa mergem 
	system("cls");
	cout << "Score: " << Score << " || " << "Time: " << Time << " sec" << endl;
	Draw_Field(Side);
	cout << "Exit - E. W/A/S/D: ";
	char aux = 'r';
	//	Side = 'D';
	while (_kbhit()) {
		fflush(stdin);
		aux = _getch();
		//Daca am introdus litera mica, o "marim"; E - iesire; Controlam daca e introdusa litera corecta
		if (aux > 90) aux = aux - 32;
		//Iesire

		if (aux == 'W' || aux == 'D' || aux == 'A' || aux == 'S')
		{
			Side = aux; break;
		}
		else if (aux == 'E')
		{
			cout << "Exit...\n"; getchar();  exit(0);
		}
		else
		{
			Side = Side; break;
		}
	}
	Sleep(SnakeSpeed);



	SetCoord(Side, OldSide, x, y); //Schimbam, stergem, inoim coordonatele
	bool LostGame = Game_end(); //Am pierdut?
	LostGame == 1 ? GameTable() : 0; //Am pierdut? Da-afisam timpul si scorul; Nu - continuam
	OldSide = Side;
	bool Found = FoundApple(); //Am gasit marul?
	Found == 1 ? AddApple() : 0; //Am gasit marul! Adaogam altul, marim scorul, lungimea sarpelui
	Move();
}

void SnakeGame::SetCoord(char Side, char OldSide, int &x, int &y) {
	static int a = 2;
	//Setam noile coordonate, cele vechi le stergem
	if (Side == 'W') {
		Field[x - 1][y] = 'W';
		Field[x][y] = '5';
		ClearTrack(Side, OldSide, a); //Stergem "urma" sarpelui
		SnakeTail.insert(SnakeTail.end(), x); //Inseram coordonata noua X a corpului
		SnakeTail.insert(SnakeTail.end(), y); //Inseram coordonata noua Y a corpului
		x -= 1; //Cooordonata X noua a capului
		y = y; //Coordonata Y noua a capului
	}
	else if (Side == 'S') {
		Field[x + 1][y] = 'S';
		Field[x][y] = '5';
		ClearTrack(Side, OldSide, a);
		SnakeTail.insert(SnakeTail.end(), x);
		SnakeTail.insert(SnakeTail.end(), y);
		x += 1;
		y = y;

	}
	else if (Side == 'A') {
		Field[x][y - 1] = 'A';
		Field[x][y] = '5';
		ClearTrack(Side, OldSide, a);
		SnakeTail.insert(SnakeTail.end(), x);
		SnakeTail.insert(SnakeTail.end(), y);
		x = x;
		y -= 1;
	}
	else if (Side == 'D') {
		Field[x][y + 1] = 'D';
		Field[x][y] = '5';

		ClearTrack(Side, OldSide, a);
		SnakeTail.insert(SnakeTail.end(), x);
		SnakeTail.insert(SnakeTail.end(), y);
		x = x;
		y += 1;
	}
	else cout << "error!\n";

	SnakeHead.erase(SnakeHead.begin(), SnakeHead.end()); //Stergem coordonatele capului veche
	SnakeHead.insert(SnakeHead.begin(), x); //Stergem coordonata X noua a capului
	SnakeHead.insert(SnakeHead.end(), y); //Stergem coordonata Y noua a capului

	OutsideCoord(); //Am iesit din granita?
}

void SnakeGame::Draw_Field(char HeadSide) {
	//Afisam terenul de joaca
	//@ - peretii terenului; 
	//o - corpul sarpelui;
	//* - marul; 
	//>,<,A,V - capul in dependenta de directie
	for (int a = 0; a < 25; ++a) {
		for (int b = 0; b < 25; ++b) {
			if (a == 0) cout << "@";
			else if (b == 0) cout << "@";
			else if (a == 24) cout << "@";
			else if (b == 24) cout << "@";
			else if (Field[a][b] == 'D') cout << ">";
			else if (Field[a][b] == 'A') cout << "<";
			else if (Field[a][b] == 'W') cout << "A";
			else if (Field[a][b] == 'S') cout << "V";
			else if (Field[a][b] == '5') cout << "o";
			else if (Field[a][b] == '*') cout << "*";
			else cout << ' ';
		}
		cout << endl;
	}
}

void SnakeGame::AddApple() {
	//Adaogam marul
	AppleCoordX = rand() % 22 + 1; //Coordonata X a marului
	AppleCoordY = rand() % 22 + 1; //Coordonata Y a marului

								   //Controlam coord.Marului=coord.Capului. Daca da - schimbam coordonatele
	if (AppleCoordX == SnakeHead[0] && AppleCoordY == SnakeHead[1])
		AddApple();

	//Controlam coord.Marului = coord.Corpului. Daca da - schimbam coordonatele
	for (int a = 1; a < SnakeTail.capacity(); ++a)
		if (AppleCoordX == SnakeTail[a - 1] && AppleCoordY == SnakeTail[a])
		{
			AppleCoordX = rand() % 22 + 1;
			AppleCoordY = rand() % 22 + 1;
		}

	Field[AppleCoordX][AppleCoordY] = '*';

}

bool SnakeGame::FoundApple() {
	//Controlam daca am gasit marul. Returnam 1 daca e gasit, 0 - nu
	if (x == AppleCoordX && y == AppleCoordY) {
		Score += 10;
		SnakeLength++;
		if ((SnakeLength % 5) == 0) SnakeSpeed += 50;
		return 1;
	}
	else
		return 0;
}

void SnakeGame::OutsideCoord() {
	//Controlam daca am iesit din granita terenului. Daca da, trecem pe partea vizavida
	if (SnakeLength == 2) {
		if (x == 24) {
			Field[1][y] = 'V';
			Field[x - 1][y] = ' ';
			Field[x - 2][y] = ' ';
			x = 1; y = y;
		}
		else if (x <= 1) {
			Field[23][y] = 'A';
			Field[x + 1][y] = ' ';
			Field[x + 2][y] = ' ';
			x = 23; y = y;
		}
		else if (y >= 24) {
			Field[x][1] = '>';
			Field[x][y - 1] = ' ';
			Field[x][y - 2] = ' ';
			y = 1; x = x;
		}
		else if (y <= 1) {
			Field[x][23] = '<';
			Field[x][y + 1] = ' ';
			Field[x][y + 2] = ' ';
			y = 23; x = x;
		}
	}
	else
		if (x >= 24) { x = 1; y = y; }
		else if (x <= 0) { x = 23; y = y; }
		else if (y >= 24) { y = 1; x = x; }
		else if (y <= 0) { y = 23; x = x; }

}

void SnakeGame::ClearTrack(char Side, char OldSide, int &a) {
	//Stergem coordonatele vechi a sarpelui
	if (SnakeLength == 2) {
		//	cout << x << " : " << y << endl;
		SnakeTail.erase(SnakeTail.begin(), SnakeTail.begin() + 2); //Stergem ultima celula din coada

																   //Stergem urma coadei in dependenta de directie, daca lungimea sarpelui e 2
		if (OldSide == 'W' && (Side == 'W' || Side == 'A' || Side == 'D'))
			Field[x + 1][y] = ' ';
		else if (OldSide == 'A' && (Side == 'W' || Side == 'S' || Side == 'A'))
			Field[x][y + 1] = ' ';
		else if (OldSide == 'S' && (Side == 'D' || Side == 'A' || Side == 'S'))
			Field[x - 1][y] = ' ';
		else if (OldSide == 'D' && (Side == 'W' || Side == 'S' || Side == 'D'))
			Field[x][y - 1] = ' ';

		if (
			(OldSide == 'W' && Side == 'S') ||
			(OldSide == 'A' && Side == 'D') ||
			(OldSide == 'S' && Side == 'W') ||
			(OldSide == 'D' && Side == 'A')
			)
			GameTable();

	}
	else if (a < SnakeLength - 1) //Nu stergem nimic pana nu s-au desenat toate partile a sarpelui
		a++;
	else if (SnakeLength >= 3)
	{
		//Stergem urma coadei in dependenta de directie, daca lungimea sarpelui e >=3
		SnakeTail.shrink_to_fit();
		int TailX = SnakeTail[0];
		int TailY = SnakeTail[1];
		//	cout << "Primul X: " << TailX << endl;
		//	cout << "Primul Y: " << TailY << endl;
		Field[TailX][TailY] = ' ';
		SnakeTail.erase(SnakeTail.begin(), SnakeTail.begin() + 2);

		if (
			(OldSide == 'W' && Side == 'S') ||
			(OldSide == 'A' && Side == 'D') ||
			(OldSide == 'S' && Side == 'W') ||
			(OldSide == 'D' && Side == 'A')
			)
			GameTable();
	}

}

bool SnakeGame::Game_end() {
	//Controlam daca am pierdut
	SnakeTail.shrink_to_fit();
	SnakeHead.shrink_to_fit();
	char HeadX = SnakeHead[0];
	char HeadY = SnakeHead[1];
	//Daca capul = corpul - am pierdut
	if (HeadX == SnakeTail[0] && HeadY == SnakeTail[1]) return 1;

	for (int a = 1; a < SnakeTail.capacity(); ++a) {
		if (HeadX == SnakeTail[a - 1] && HeadY == SnakeTail[a])
			return 1;
	}
	return 0;
}

void  SnakeGame::GameTable() {
	//Afisam ca am pierdut + tabela de rezultate[in curs de dezvoltare]
	system("cls");
	fstream Champions("Champions.r0", ios_base::in | ios_base::out | ios_base::_Nocreate);
	cout << "\t\t\t\t\t\tScore: " << Score << " | Time: " << Time << endl;
	cout << "\t\t\t\t\t\t   |GAME OVER|\n";
	/*
	if (Champions.fail()){
	Champions.close();
	fstream Champions("Champions.r0", ios_base::out | ios_base::trunc);

	cout << "\t\t\t\t\t\t|   " << 1 << ". " << 0 << "\t|" << endl;
	Champions << Score << endl;
	for (int a = 0; a < 10; a++){
	cout << "\t\t\t\t\t\t|   " << a + 1 << ". " << "0" << "\t|" << endl;
	Champions << "0" << endl;
	}
	}
	else{
	vector <int> Punctele(10);
	int contor = 0;
	int a = 0;
	///////////////
	while (!Champions.eof()){
	Champions >> Punctele[contor];
	contor++;
	cout << Punctele[contor] << "\n";
	}
	cout << "hei!\n";
	Champions.close();
	//////////////
	for (int e = 9; e >= 0; --e)
	if (Score > Punctele[e]){
	Punctele[e] = Score;
	break;
	}
	///////////////
	contor = 1;
	while (contor != 0){
	contor = 0;
	for (int a = 1; a < 10; ++a)
	if (Punctele[a]>Punctele[a - 1]){
	swap(Punctele[a], Punctele[a - 1]);
	contor = 1;
	}
	}
	///////////
	fstream Champions("Champions.r0", ios_base::out);
	for (int a = 0; a < 10; a++){
	cout << "\t\t\t\t\t\t|   " << a + 1 << ". " << Punctele[a] << "\t|" << endl;
	Champions << Punctele[a] << endl;
	}

	}
	Champions.close();
	*/
	fflush(stdin);
	getchar();
	exit(0);
}


int main(void) {
	srand(time_t(NULL));
	SnakeGame SG1;
	SG1.Move();
	//	SG1.GameTable();

	getchar();
	return 0;
}