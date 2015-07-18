
#include<string.h>
#include<conio.h>
#include<stdio.h>
#include<Windows.h>
#include<time.h>
#include<fmod.h>
#pragma comment(lib, "fmodex_vc.lib")
#define MAX 1024

#define LEFT 75

#define RIGHT 77

#define UP 72

#define DOWN 80
#define B 98
#define ESC 27

#define randomize() srand((unsigned)time(NULL))

#define random(n) (rand() % (n))


#define putchxy(x,y,c) {gotoxy(x,y);putch(c);}
#define ESC 27

#define MAXENEMY 10

#define MAXBALL 20
#define MAXBULLET 2
#define MAXBOMB 3

FMOD_SYSTEM *g_system; //시스템
FMOD_SOUND *g_sound[5]; // 들어갈 노래 종류
FMOD_CHANNEL *g_channel[8]; //구별채널

void Init()
{
	FMOD_System_Create(&g_system);  //사운드 시스템 생성 시스템의 주소를 대입
	FMOD_System_Init(g_system, 10, FMOD_INIT_NORMAL, NULL); //사운드 시스템 초기화(초기화할시스템주소,채널수,플래그,NULL)

	FMOD_System_CreateSound(g_system, "A.dll", FMOD_DEFAULT, 0, &g_sound[0]); //끝날때 소리
	//사운드를 메모리로 읽어오는 함수 (읽을 시스템주소, 파일이름 mp3,wav,mid,pla....., 
	//모드 FMOD_LOOP_NORMAL 무한반복 주로 배경음, FMOD_DEFAULT 한번만 효과음,0,사운드넣을 주소)
	FMOD_System_CreateSound(g_system, "gs.dll", FMOD_DEFAULT, 0, &g_sound[1]);

	FMOD_System_CreateSound(g_system, "B1.dll", FMOD_DEFAULT, 0, &g_sound[2]);

	FMOD_System_CreateSound(g_system, "b2.dll", FMOD_DEFAULT, 0, &g_sound[3]);

	FMOD_System_CreateSound(g_system, "brp.dll", FMOD_DEFAULT, 0, &g_sound[4]);
	//특정채널사운드에 출력 (사용시스템, FMOD_CHANNEL_FREE 이용시 알아서 여유있는채널골라줌,출력할사운드, 일시정지 할 것인지.
	//사운드를 출력할 채널,
}
void Release()
{
	FMOD_Sound_Release(g_sound[0]);
	FMOD_Sound_Release(g_sound[1]);
	FMOD_Sound_Release(g_sound[2]);
	FMOD_Sound_Release(g_sound[3]);
	FMOD_Sound_Release(g_sound[4]);
	FMOD_System_Close(g_system);
	FMOD_System_Release(g_system);
}


struct tag_Bullet

{

	BOOL exist;

	int x, y;

} Bullet[3];

typedef struct{
	BOOL exist;
	int x, y;
	int nFrame; //이동할 속도

	int nStay; //얼만큼 그 좌표에 머물것인가 

}Bomb;

void gotoxy(int x, int y)
{
	COORD Pos = { x - 1, y - 1 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}


void cursor(int visible)
{
	CONSOLE_CURSOR_INFO ConsoleCursor;
	ConsoleCursor.bVisible = visible;
	ConsoleCursor.dwSize = 20;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ConsoleCursor);
}



struct tag_Enemy

{

	BOOL exist; //생성할건지 말건지

	int Type; //어떤 기체로 생성할 건지

	int x, y;  //이동좌표

	int Delta;  //왼쪽이동 또는 오른쪽이동

	int nFrame; //이동할 속도

	int nStay; //얼만큼 그 좌표에 머물것인가 

} Enemy[MAXENEMY];

struct tag_Ball

{

	BOOL exist; //존재

	int x, y; //좌표

	int nFrame; //이동속도
	 
	int nStay; //좌표 머물것인가.

} Ball[MAXBALL];

char *arEnemy[] = { " ;:^:; ", " zZWZz ", " oO@Oo ", " <-=-> " };

int fx; //내꺼 이동 좌표

int bx, by; //총알

int Score; //점수

BOOL IsKeyDown(int Key)

{

	return ((GetAsyncKeyState(Key) & 0x8000) != 0);

}

int main()
{
	int ch;

	int i, j;
	Bomb Bomb[3];
	BOOL bFound;
	int bombCount = 3;
	int count;
	int c = -2;
	int isPlay = 0;
	Init();
	randomize();

	system("cls");

	cursor(0);

	fx = 40;

	bx = -1;

	Score = 0;

	for (count = 0;; count++)
	{

		if (count % 5 == 0) { //이동속도 이거 없애면 캐릭터 개 빨리 움직임

			if (IsKeyDown(VK_LEFT)) {

				if (fx > 6) fx--;

			}

			if (IsKeyDown(VK_RIGHT)) {

				if (fx < 72) fx++;

			}

		}

		if (kbhit()) //키눌렸나 안눌렸나
		{
			ch = getch();

			if (ch == 0xE0 || ch == 0) {

				getch();

			}
			else
			{
				switch (ch)
				{
				case B:
					if (bombCount <= 0) break;
					else
					{
						bombCount--;
						Bomb[bombCount].nFrame = Bomb[bombCount].nStay = 15;
						Bomb[bombCount].exist = TRUE;
						Bomb[bombCount].x = fx;
						Bomb[bombCount].y = 23;
						FMOD_System_PlaySound(g_system, FMOD_CHANNEL_FREE, g_sound[4], 0, &g_channel[7]);
					}
					break;

				case ' ':
					for (i = 0; i<MAXBULLET && Bullet[i].exist == TRUE; i++) { ; }
					
					FMOD_System_PlaySound(g_system, FMOD_CHANNEL_FREE, g_sound[1], 0, &g_channel[0]);
					
						Bullet[0].x = fx-2;

						Bullet[0].y = 23;

						Bullet[0].exist = TRUE;
	
						Bullet[1].x = fx+2;

						Bullet[1].y = 23;

						Bullet[1].exist = TRUE;
					
					break;

				case ESC:
					goto end;
				}
			}
		}

		for (i = bombCount; i >= bombCount; i--)
		{
			if (Bomb[i].exist == FALSE) continue;
			if (--Bomb[i].nStay==0)
			{
				Bomb[i].nStay = Bomb[i].nFrame;
				gotoxy(Bomb[i].x, Bomb[i].y - 1); putch(' ');
				gotoxy(Bomb[i].x, Bomb[i].y); putch(' ');
				
				if (Bomb[i].y == 5)
				{
					system("cls");
					Bomb[i].exist = FALSE;
					for (j = 0; j < MAXENEMY; j++)
					{
						if (Enemy[j].exist == FALSE) continue;
						else
						{
							Score += (7 - Enemy[j].nFrame);
							Enemy[j].exist = FALSE;
						}
					}

					for (j = 0; j < MAXBALL; j++)
					{
						if (Ball[j].exist == FALSE) continue;
						else
							Ball[j].exist = FALSE;
					}

					for (j = 0; j < MAXBULLET; j++)
					{
						if (Bullet[j].exist == FALSE) continue;
						else
							Bullet[j].exist = FALSE;
					}
				}
				else
				{
					Bomb[i].y -= 2;

					gotoxy(Bomb[i].x, Bomb[i].y - 1); putch('@');
					gotoxy(Bomb[i].x, Bomb[i].y); putch('@');
				}
			}
		}

		for (i = 0; i<MAXBULLET; i++) { //총알 이동

			if (Bullet[i].exist) {

				gotoxy(Bullet[i].x, Bullet[i].y); putch(' ');

				if (Bullet[i].y == 0) {

					Bullet[i].exist = FALSE;
					system("cls");
				}
				else {

					Bullet[i].y--;
					//FMOD_Sound_Release(g_sound[1]);
					gotoxy(Bullet[i].x, Bullet[i].y); putch('i');

				}

			}

		}



		if (random(100) == 0) //적 생성
		{
			for (i = 0; i < MAXENEMY && Enemy[i].exist == TRUE; i++){}

			if (i != MAXENEMY)
			{
				if (random(2) == 1)
				{
					Enemy[i].x = 5;
					Enemy[i].Delta = 1; //오른쪽으로이동시 사용
				}
				else {

					Enemy[i].x = 75;
					Enemy[i].Delta = -1; //왼쪽으로 이동시 사용

				}

				for (;;)
				{
					Enemy[i].y = random(10) + 1;

					for (bFound = FALSE, j = 0; j < MAXENEMY; j++) {

						if (Enemy[j].exist == TRUE && Enemy[j].y == Enemy[i].y) {

							bFound = TRUE;

							break;

						}

					}

					if (bFound == FALSE)
						break;
				}

 				Enemy[i].nFrame = Enemy[i].nStay = random(6) + 1;

				Enemy[i].Type = random(sizeof(arEnemy) / sizeof(arEnemy[0])); //기체 모양 결정

				Enemy[i].exist = TRUE;
			}
		}



		for (i = 0; i < MAXENEMY; i++)  // 적기체 이동및 총알 생성
		{
			if (Enemy[i].exist == FALSE) continue;

			if (--Enemy[i].nStay == 0)
			{
				if (Enemy[i].Type == -1) {

					gotoxy(Enemy[i].x - 3, Enemy[i].y);

					puts("       ");

					Enemy[i].exist = FALSE;

					continue;

				}


				Enemy[i].nStay = Enemy[i].nFrame;

				if (Enemy[i].x >= 76 || Enemy[i].x <= 4) {

					Enemy[i].exist = FALSE;

					gotoxy(Enemy[i].x - 3, Enemy[i].y);

					puts("       ");

				}
				else
				{
					Enemy[i].x += Enemy[i].Delta;
					gotoxy(Enemy[i].x - 3, Enemy[i].y);
					puts(arEnemy[Enemy[i].Type]);

					if (random(40) == 0)
					{
						for (j = 0; j < MAXBALL && Ball[j].exist == TRUE; j++) { ; }

						if (j != MAXBALL)
						{
							FMOD_System_PlaySound(g_system, FMOD_CHANNEL_FREE, g_sound[2], 0, &g_channel[1]);
							Ball[j].exist = TRUE;
							Ball[j].x = Enemy[i].x + 2;
							Ball[j].y = Enemy[i].y + 1;
							Ball[j].nFrame = Ball[j].nStay = Enemy[i].nFrame * 6;
						}
					}
				}
			}

		}

		for (i = 0; i < MAXBALL; i++) //적 총알 이동
		{
			if (Ball[i].exist == FALSE) continue;

			if (--Ball[i].nStay == 0)
			{
				Ball[i].nStay = Ball[i].nFrame;
				gotoxy(Ball[i].x, Ball[i].y);
				putch(' ');

				if (Ball[i].y >= 23)
					Ball[i].exist = FALSE;
				else
				{
					Ball[i].y++;
					gotoxy(Ball[i].x, Ball[i].y);
					putch('*');
				}
			}
		}

		//내 총알과 충돌
		for (i = 0; i<MAXENEMY; i++) {

			if (Enemy[i].exist == FALSE || Enemy[i].Type == -1) continue;

			for (j = 0; j < MAXBULLET; j++) {

				if (Bullet[j].exist == FALSE) continue;

				if (Enemy[i].y == Bullet[j].y && abs(Enemy[i].x - Bullet[j].x) <= 2) {

					gotoxy(Bullet[j].x, Bullet[j].y); putch(' ');

					Bullet[j].exist = FALSE;


					Enemy[i].exist = FALSE;
					Enemy[i].nFrame = Enemy[i].nStay;
					Enemy[i].Type = -1;

					 gotoxy(Enemy[i].x - 3, Enemy[i].y);
					FMOD_System_PlaySound(g_system, FMOD_CHANNEL_FREE, g_sound[3], 0, &g_channel[2]);
					puts(" .,:,. ");

					Score += (7- Enemy[i].nFrame);

					break;

				}
			}
		}
		/*

		for (i = 0; i < MAXENEMY; i++)
		{
			if (Enemy[i].exist == FALSE) continue;

			if (Enemy[i].y == by && abs(Enemy[i].x - bx) <= 2) //적기체와 내 총알의 충돌
			{
				gotoxy(bx, by); putch(' ');
				bx = -1;

				Enemy[i].exist = FALSE;
				gotoxy(Enemy[i].x - 3, Enemy[i].y);
				puts("      ");
				Score += 7 - Enemy[i].nFrame;

				break;
			}
		}
		*/

		for (i = 0; i < MAXBALL; i++)
		{
			if (Ball[i].exist == FALSE) continue;

			if (Ball[i].y == 23 && abs(Ball[i].x - fx) <= 2)
			{
				gotoxy(fx - 3, 21); puts("   .   ");

				gotoxy(fx - 3, 22); puts(" .  . .");

				gotoxy(fx - 3, 23); puts("..:V:..");
				FMOD_System_PlaySound(g_system, FMOD_CHANNEL_FREE, g_sound[0], 0, &g_channel[3]);
				Sleep(2000);

				goto end;
			}
		}

		gotoxy(fx-3, 23);
		puts(" <<A>> ");
		gotoxy(0, 24);
		printf("Score %d     Bomb : %d", Score,bombCount);
		gotoxy(23, 24);
		printf("SpaceBar = FIRE    B= Bomb    Move : <-  ->");
		Sleep(10);

	}
	end:
	{
		system("cls");
		gotoxy(15, 15);
		puts("끝났따!!!!!!!!!!!!!!!!!!!!!");
		cursor(0);
		Release();
	}
	return 0;
}