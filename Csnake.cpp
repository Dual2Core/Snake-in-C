#include <iostream>
#include <string>
#include <cstdio>
#include <windows.h>
#include <conio.h>
#include <time.h>
using namespace std;

#define W 119
#define A 97
#define S 115
#define D 100

// USTAWIENIA
#define MAX_WIDTH 64
#define MAX_HEIGHT 24
#define SPEED 2 // Mniejsza wartosc = szybciej
#define MAX_TAIL_LENGTH 100
void ZeroBackground(char scr[][MAX_HEIGHT])
{
	for(int x = 1; x<MAX_WIDTH-1; x++)
	for(int y = 1; y<MAX_HEIGHT-1; y++)
	{
		scr[x][y] = ' ';
	}
	
}

struct TCords
{
	short x;
	short y;
};

void SetCursorPosition(short x, short y)
{
	COORD crds;
	crds.X = x;
	crds.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),crds);
}

void DrawScreen(char scr[][MAX_HEIGHT])
{
	scr[MAX_WIDTH/2][MAX_HEIGHT/2] = 'o';
	for(int y = 0; y<MAX_HEIGHT; y++)
	{
		for(int x = 0; x<MAX_WIDTH; x++)
		printf("%c",scr[x][y]);
		
		printf("\n");
	}
	
	SetCursorPosition(MAX_WIDTH+5,MAX_HEIGHT/5);
	printf("PUNKTY 0");
}

void AddPoint(int TailLength)
{
	SetCursorPosition(MAX_WIDTH+5,MAX_HEIGHT/5);
	char buf[33];
	itoa(TailLength-1,buf,10);
	printf("PUNKTY %s",buf);
}

void Draw(short x, short y, char c, char scr[][MAX_HEIGHT])
{
	SetCursorPosition(x,y);
	printf("%c",c);
	scr[x][y] = c;
}

void End()
{
	SetCursorPosition(((MAX_WIDTH/2)-5),MAX_HEIGHT/2);
	printf("PRZEGRANA");
	getch();
}

bool ColliderControl(short x, short y, TCords TailCords[MAX_TAIL_LENGTH], int &TailLength, char scr[][MAX_HEIGHT],short old_cx,short old_cy)
{
	if(x==0 || x==MAX_WIDTH-1 || y==0 || y==MAX_HEIGHT-1 || scr[x][y] == 'o')
	{
		End();
		return true;
	}
	else
	if(scr[x][y] == '$')
	{
		TailLength++;
		AddPoint(TailLength);
	}

	return false;
}



int main()
{
	char Screen[MAX_WIDTH][MAX_HEIGHT];
	
	//Narysuj granice
	for(int x = 0; x<MAX_WIDTH; x++)
	for(int y = 0; y<MAX_HEIGHT; y++)
	{
		if(x==0 && y==0)
		Screen[x][y] = 'X';
		else
		if(y==0 || y==MAX_HEIGHT-1)
		Screen[x][y] = 'X';
		else
		if(x==0 || x==MAX_WIDTH-1)
		Screen[x][y] = 'X';
	}
	
	//Wyczysc grywalne pole
	ZeroBackground(Screen);
	
	//Narysuj pole
	DrawScreen(Screen);
	
	//Inicjacja generatora liczb losowych
	srand(time(NULL));
	
	//Zmienne
	int TailLength = 1;
	int tmpTailLength = 1;
	TCords TailCords[MAX_TAIL_LENGTH];
	char Direction;
	
	short cx = MAX_WIDTH/2;
	short cy = MAX_HEIGHT/2;
	
	//Snake Engine
	for(int tick = 0;;tick++,Sleep(SPEED*100))
	{
		//Pobierz kierunek
		int _dir = Direction;
		if(kbhit())
		{
			Direction = getch();
			Direction = tolower(Direction);
		}
		//Kontrola ruchu
		switch(_dir)
		{
			case W:
				if(Direction == S)
				Direction = W;
				break;
			case A:
				if(Direction == D)
				Direction = A;
				break;
			case S:
				if(Direction == W)
				Direction = S;
				break;
			case D:
				if(Direction == A)
				Direction = D;
				break;
		}
		Draw(cx,cy,' ',Screen);
		short old_cx = cx,old_cy = cy;
		/*    Dorysuj nowy punkt   */
		switch(Direction)
		{
			case W:
				cy--;
				break;
			case A:
				cx--;
				break;
			case S:
				cy++;
				break;
			case D:
				cx++;
				break;
		}
		//Kontrola zderzeñ
		if(ColliderControl(cx,cy,TailCords,TailLength,Screen,old_cx,old_cy))
			break;
			
		//Kontrola ogonu
		if(TailLength>1)
		{
			Draw(old_cx,old_cy,'o',Screen);
			tmpTailLength++;
			TailCords[tmpTailLength].x = old_cx;
			TailCords[tmpTailLength].y = old_cy;

			if(tmpTailLength>=TailLength)
			{
				Draw(TailCords[1].x,TailCords[1].y,' ',Screen);
				tmpTailLength--;
				//Przesuñ wsprolrzedne ogonu
				for(int i = 1; i<=tmpTailLength; i++)
				{
					TailCords[i].x = TailCords[i+1].x;
					TailCords[i].y = TailCords[i+1].y;
				}
			}

			
		}
		//----------------
		Draw(cx,cy,'o',Screen);
		/*--------------------------*/
		
		//Punkty
		if(tick%32==0)
		{
			short diax = (rand()%MAX_WIDTH-2)+1;
			short diay = (rand()%MAX_HEIGHT-2)+1;
			Draw(diax,diay,'$',Screen);
		}
		//------
	}
	
	
	
}
