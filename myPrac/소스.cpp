
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

FMOD_SYSTEM *g_system; //�ý���
FMOD_SOUND *g_sound[5]; // �� �뷡 ����
FMOD_CHANNEL *g_channel[8]; //����ä��

void Init()
{
	FMOD_System_Create(&g_system);  //���� �ý��� ���� �ý����� �ּҸ� ����
	FMOD_System_Init(g_system, 10, FMOD_INIT_NORMAL, NULL); //���� �ý��� �ʱ�ȭ(�ʱ�ȭ�ҽý����ּ�,ä�μ�,�÷���,NULL)

	FMOD_System_CreateSound(g_system, "A.dll", FMOD_DEFAULT, 0, &g_sound[0]); //������ �Ҹ�
	//���带 �޸𸮷� �о���� �Լ� (���� �ý����ּ�, �����̸� mp3,wav,mid,pla....., 
	//��� FMOD_LOOP_NORMAL ���ѹݺ� �ַ� �����, FMOD_DEFAULT �ѹ��� ȿ����,0,������� �ּ�)
	FMOD_System_CreateSound(g_system, "gs.dll", FMOD_DEFAULT, 0, &g_sound[1]);

	FMOD_System_CreateSound(g_system, "B1.dll", FMOD_DEFAULT, 0, &g_sound[2]);

	FMOD_System_CreateSound(g_system, "b2.dll", FMOD_DEFAULT, 0, &g_sound[3]);

	FMOD_System_CreateSound(g_system, "brp.dll", FMOD_DEFAULT, 0, &g_sound[4]);
	//Ư��ä�λ��忡 ��� (���ý���, FMOD_CHANNEL_FREE �̿�� �˾Ƽ� �����ִ�ä�ΰ����,����һ���, �Ͻ����� �� ������.
	//���带 ����� ä��,
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
	int nFrame; //�̵��� �ӵ�

	int nStay; //��ŭ �� ��ǥ�� �ӹ����ΰ� 

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

	BOOL exist; //�����Ұ��� ������

	int Type; //� ��ü�� ������ ����

	int x, y;  //�̵���ǥ

	int Delta;  //�����̵� �Ǵ� �������̵�

	int nFrame; //�̵��� �ӵ�

	int nStay; //��ŭ �� ��ǥ�� �ӹ����ΰ� 

} Enemy[MAXENEMY];

struct tag_Ball

{

	BOOL exist; //����

	int x, y; //��ǥ

	int nFrame; //�̵��ӵ�
	 
	int nStay; //��ǥ �ӹ����ΰ�.

} Ball[MAXBALL];

char *arEnemy[] = { " ;:^:; ", " zZWZz ", " oO@Oo ", " <-=-> " };

int fx; //���� �̵� ��ǥ

int bx, by; //�Ѿ�

int Score; //����

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

		if (count % 5 == 0) { //�̵��ӵ� �̰� ���ָ� ĳ���� �� ���� ������

			if (IsKeyDown(VK_LEFT)) {

				if (fx > 6) fx--;

			}

			if (IsKeyDown(VK_RIGHT)) {

				if (fx < 72) fx++;

			}

		}

		if (kbhit()) //Ű���ȳ� �ȴ��ȳ�
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

		for (i = 0; i<MAXBULLET; i++) { //�Ѿ� �̵�

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



		if (random(100) == 0) //�� ����
		{
			for (i = 0; i < MAXENEMY && Enemy[i].exist == TRUE; i++){}

			if (i != MAXENEMY)
			{
				if (random(2) == 1)
				{
					Enemy[i].x = 5;
					Enemy[i].Delta = 1; //�����������̵��� ���
				}
				else {

					Enemy[i].x = 75;
					Enemy[i].Delta = -1; //�������� �̵��� ���

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

				Enemy[i].Type = random(sizeof(arEnemy) / sizeof(arEnemy[0])); //��ü ��� ����

				Enemy[i].exist = TRUE;
			}
		}



		for (i = 0; i < MAXENEMY; i++)  // ����ü �̵��� �Ѿ� ����
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

		for (i = 0; i < MAXBALL; i++) //�� �Ѿ� �̵�
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

		//�� �Ѿ˰� �浹
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

			if (Enemy[i].y == by && abs(Enemy[i].x - bx) <= 2) //����ü�� �� �Ѿ��� �浹
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
		puts("������!!!!!!!!!!!!!!!!!!!!!");
		cursor(0);
		Release();
	}
	return 0;
}