#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>

#define MAX 30
#define STR_MAX 100
#define STAGE_NUM 5

#define TRUE 1
#define FALSE 0

//Print Location define

#define HELLO_PRINT_X 10
#define HELLO_PRINT_Y 1

#define STATUS_PRINT_X 0
#define STATUS_PRINT_Y (HELLO_PRINT_Y + 2)

#define MAP_PRINT_X 0
#define MAP_PRINT_Y (HELLO_PRINT_Y + 5)

#define COMMAND_PRINT_X 0
#define COMMAND_PRINT_Y (mapY + MAP_PRINT_Y + 1)

#define COMMAND_SCAN_X 10
#define COMMAND_SCAN_Y (mapY + MAP_PRINT_Y + 1)

char name[STR_MAX];
int record[STAGE_NUM];

int mapX, mapY;
int playerX, playerY;

char map[MAX][MAX];

void nameInput(void);
void mapMaker(int stage);
int checkBoxStorage(void);
void mapPrinter(void);
void playerFinder(void);

//gotoxy(x,y) clear() defined
#ifdef _WIN32
//define something for Windows (32-bit and 64-bit, this part is common)
#include <Windows.h>
void gotoxy(int x, int y) {
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void clear(void) {
	system("cls");
}
#elif __linux__
// linux
#include <unistd.h>
#include <termio.h>
int getch(void)
{
	int c;
	struct termios oldattr, newattr;
	tcgetattr(STDIN_FILENO, &oldattr);           // ���� �͹̳� ���� ����
	newattr = oldattr;
	newattr.c_lflag &= ~(ICANON | ECHO);         // CANONICAL�� ECHO ��
	newattr.c_cc[VMIN] = 1;                      // �ּ� �Է� ���� ���� 1�� ����
	newattr.c_cc[VTIME] = 0;                     // �ּ� �б� ��� �ð��� 0���� ����
	tcsetattr(STDIN_FILENO, TCSANOW, &newattr);  // �͹̳ο� ���� �Է�
	c = getchar();                               // Ű���� �Է� ����
	tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);  // ������ �������� ����
	return c;
}
void gotoxy(int x, int y) {
	printf("\033[%d;%df", y, x);
	fflush(stdout);
}
void clear() {
	printf("\033[H\033[J");
}
#endif

int main(void)
{
	printf("Start. . . .\n");
	nameInput();
	clear();
	
	for(int i = 0;i < STAGE_NUM;i++)
	{
		gotoxy(HELLO_PRINT_X, HELLO_PRINT_Y);
		printf("\tHello %s", name);
		gotoxy(STATUS_PRINT_X, STATUS_PRINT_Y);
		printf("Stage %d", i + 1);

		
		mapMaker(i + 1);
		if (!checkBoxStorage())
		{
			printf("\n\n�ڽ��� ������� ������ �ٸ��ϴ�. ���α׷��� �����մϴ�.\n");
			return 0;
		}

		mapPrinter();
		playerFinder();

		char run = TRUE;

		gotoxy(COMMAND_PRINT_X, COMMAND_PRINT_Y);
		printf("(Command) ");

		while (run)
		{
			char commandArr[STR_MAX] = { 0 };
			char command;

			//input command
			gotoxy(COMMAND_SCAN_X, COMMAND_SCAN_Y);
			gets(commandArr);

			for (int i = 0; i < strlen(commandArr); i++)
			{
				if (commandArr[i] != ' ')	command = commandArr[i];
				else continue;

				switch (command)
				{
				case 'h':	case 'H':	//left
					//do something
					break;
				case 'j':	case 'J':	//down
					//do something
					break;
				case 'k':	case 'K':	//up
					//do something
					break;
				case 'l':	case 'L':	//right
					//do something
					break;

				case 'u':	case 'U':	//undo
					//do something
					break;
				case 'r':	case 'R':	//replay ���� ���� �ٽý���
					//do something
					break;
				case 'n':	case 'N':	//new 1��° �ʺ��� �ٽý���
					//do something
					break;
				case 'e':	case 'E':	//exit ���� ����
					//do something
					break;
				case 's':	case 'S':	//save ���� ���� ����
					//do something
					break;
				case 'f':	case 'F':	//save���� �������� ��������
					//do something
					break;
				case 'd':	case 'D':	//display help
					//do something
					break;
				case 't':	case 'T':	//top ���� ���� ������
					//do something
					break;

				default:
					//do something
					break;
				}

			}

			//clear
			gotoxy(COMMAND_SCAN_X, COMMAND_SCAN_Y);
			for (int i = 0; i < strlen(commandArr); i++)	printf(" ");
			
		}
		clear();
	}

	printf("SEE YOU %s. . . . \n", name);
	
	return 0;
}

void nameInput(void)
{
	printf("input name : ");
	gets(name);
}
void mapMaker(int stage)
{
	FILE* in;
	switch (stage)
	{
	case 1:
		in = fopen("map1.txt", "r");

		mapX = 22;
		mapY = 11;

		break;

	case 2:
		in = fopen("map2.txt", "r");

		mapX = 14;
		mapY = 10;

		break;

	case 3:
		in = fopen("map3.txt", "r");

		mapX = 17;
		mapY = 10;

		break;
	case 4:
		in = fopen("map4.txt", "r");

		mapX = 13;
		mapY = 11;

		break;
	case 5:
		in = fopen("map5.txt", "r");

		mapX = 8;
		mapY = 8;

		break;
	default:
		in = NULL;
		break;
	}

	for (int i = 0; i < mapY; i++)	fgets(map[i], MAX, in);

	if(in != NULL)	fclose(in);
}
int checkBoxStorage(void)
{
	int cntBox = 0, cntStorage = 0;
	for (int i = 0; i < mapY; i++)
	{
		for (int j = 0; j < mapX; j++)
		{
			if (map[i][j] == '$')	cntBox++;
			else if (map[i][j] == 'O')	cntStorage++;
		}
	}
	if (cntBox != cntStorage)	return 0;
	else return 1;
}
void mapPrinter(void)
{
	gotoxy(MAP_PRINT_X, MAP_PRINT_Y);

	for (int i = 0; i < mapY; i++)
	{
		for (int j = 0; j < mapX; j++)
		{
			if (map[i][j] == '.')	printf("  ");
			else printf("%c ", map[i][j]);
		}
		printf("\n");
	}
}
void playerFinder(void)
{
	for (int i = 0; i < mapY; i++)
	{
		for (int j = 0; j < mapX; j++)
		{
			if (map[i][j] == '@')
			{
				playerY = i;
				playerX = j;
				return;
			}
		}
	}
}