#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 30
#define STR_MAX 100
#define STAGE_NUM 5
#define MAX_LOG 1000

#define UNDO_INIT 5

#define TRUE 1
#define FALSE 0

//Print Location define

#define HELLO_PRINT_X 10
#define HELLO_PRINT_Y 2

#define STATUS_PRINT_X 0
#define STATUS_PRINT_Y (HELLO_PRINT_Y + 2)

#define MAP_PRINT_X 0
#define MAP_PRINT_Y (STATUS_PRINT_Y + 5)

#define PLAYER_PRINT_X (MAP_PRINT_X + 2 * playerX)
#define PLAYER_PRINT_Y (MAP_PRINT_Y + playerY)

#define COMMAND_PRINT_X 0
#define COMMAND_PRINT_Y (mapY + MAP_PRINT_Y + 1)

#define COMMAND_SCAN_X 10
#define COMMAND_SCAN_Y (mapY + MAP_PRINT_Y + 1)

#define DISHELP_PRINT_X 0
#define DISHELP_PRINT_Y (COMMAND_PRINT_Y + 2)

#define PROMPT_PRINT_X 0
#define PROMPT_PRINT_Y (COMMAND_PRINT_Y + 2)

char name[STR_MAX];
int record[STAGE_NUM];

char log[STAGE_NUM][MAX_LOG];
int logcnt = 0;

int stage;
int mapX, mapY;
int playerX, playerY;
char map[MAX][MAX];

int remainUndo = UNDO_INIT;

void nameInput(void);
void mapMaker(int stage);
int checkBoxStorage(void);

void statPrint(void);
int printNewStage(int stage);
void printStage(int stage);

void mapPrinter(void);
void playerFinder(void);

void moveplayer(int xplus, int yplus);
int move(int xplus, int yplus);
void loadMove(int repeat);

int success(int stage);

void gotoxy(int x, int y)
{
	printf("\033[%dd\033[%dG", y, x + 1);
}
void clear()
{
	printf("\033[H\033[J");
}

int main(void)
{
	FILE* save;
	clear();
	printf("Start. . . .\n");
	nameInput();
	clear();
	
	for(stage = 1;stage <= STAGE_NUM;stage++)
	{
		if (!printNewStage(stage))	return 0;

		char run = TRUE;
		char disHelp = FALSE;
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
					//left
				case 'h':	case 'H':
					if (move(-1, 0))	log[stage - 1][record[stage - 1] - 1] = 'h';
					break;
					
					//down
				case 'j':	case 'J':
					if (move(0, 1))	log[stage - 1][record[stage - 1] - 1] = 'j';
					break;

					//up
				case 'k':	case 'K':
					if (move(0, -1))	log[stage - 1][record[stage - 1] - 1] = 'k';
					break;

					//right
				case 'l':	case 'L':
					if (move(1, 0))	log[stage - 1][record[stage - 1] - 1] = 'l';
					break;

				case 'u':	case 'U':	//undo
					if (remainUndo > 0 && record[stage - 1] > 0)
					{
						remainUndo--;
						if (!printNewStage(stage))	return 0;
						loadMove(record[stage - 1] - 1);
					}
					else if(remainUndo <= 0)
					{
						//prompt
						gotoxy(PROMPT_PRINT_X, PROMPT_PRINT_Y);
						printf("Undo 기회 모두사용함");
					}
					else if (record[stage - 1] <= 0)
					{
						gotoxy(PROMPT_PRINT_X, PROMPT_PRINT_Y);
						printf("Nothing to Undo");
					}

					break;

				case 'r':	case 'R':	//replay 현재 맵을 다시시작 (움직임 횟수 유지)
					//do something
					if (!printNewStage(stage))	return 0;
					break;

				case 'n':	case 'N':	//new 1번째 맵부터 다시시작 (움직임 횟수 삭제)
					//do something
					for (int i = 0; i < STAGE_NUM; i++)	record[i] = 0;
					stage = 1;
					if (!printNewStage(stage))	return 0;
					break;

				case 'e':	case 'E':	//exit 게임 종료
					clear();
					printf("SEE YOU %s. . . . \n", name);

				case 's':	case 'S':	//save 현재 상태 저장
					//do something
					save = fopen("sokoban.txt", "w");

					//Line 1: NAME
					//Line 2: Current Stage
					//Line 3: Stage Records
					//Line 4~STAGE_NUM + 4: Stage Logs
					//Line STAGE_NUM + 5: remainUndo

					fprintf(save, "%s", name);
					fprintf(save, "\n");
					fprintf(save, "%d\n", stage);
					for (int i = 0; i < STAGE_NUM;i++)	fprintf(save, "%d ", record[i]);
					fprintf(save, "\n");
					for (int i = 0; i < STAGE_NUM; i++)
					{
						for (int j = 0; j < record[i]; j++)	fprintf(save, "%c", log[i][j]);
						fprintf(save, "\n");
					}
					fprintf(save, "%d\n", remainUndo);

					fclose(save);

					if (command == 'e' || command == 'E')	return 0;
					break;

				case 'f':	case 'F':	//save파일 시점부터 게임진행

					/////////////////////////////////////////////////
					//BBBBBBBBBBBBBBBUUUUUUUUUUUUUUUUUGGGGGGGGGGGGGGG
					/////////////////////////////////////////////////
					
					save = fopen("sokoban.txt", "r");

					fgets(name,STR_MAX, save);
					rewind(save);

					fscanf(save, "%d", &stage);
					rewind(save);

					for (int i = 0; i < STAGE_NUM; i++)	fscanf(save, "%d", &record[i]);
					rewind(save);

					for (int i = 0; i < STAGE_NUM; i++)
					{
						for (int j = 0; j < record[i]; j++)	fgets(log[i], MAX_LOG, save);
						rewind(save);
					}
					fscanf(save, "%d", &remainUndo);

					printNewStage(stage);
					loadMove(record[stage - 1]);

					fclose(save);

					break;

				case 'd':	case 'D':	//display help
					printStage(stage);
					if (disHelp == FALSE)
					{
						disHelp = TRUE;
						gotoxy(DISHELP_PRINT_X, DISHELP_PRINT_Y + 0);	printf("%10s : 왼쪽", "h");
						gotoxy(DISHELP_PRINT_X, DISHELP_PRINT_Y + 1);	printf("%10s : 아래쪽", "j");
						gotoxy(DISHELP_PRINT_X, DISHELP_PRINT_Y + 2);	printf("%10s : 위쪽", "k");
						gotoxy(DISHELP_PRINT_X, DISHELP_PRINT_Y + 3);	printf("%10s : 오른쪽", "l");
						gotoxy(DISHELP_PRINT_X, DISHELP_PRINT_Y + 4);	printf("%10s : Undo (최대 5번)", "u");
						gotoxy(DISHELP_PRINT_X, DISHELP_PRINT_Y + 5);	printf("%10s : Replay 현재 맵을 처음부터 다시 시작 (움직임 횟수는 유지)", "r");
						gotoxy(DISHELP_PRINT_X, DISHELP_PRINT_Y + 6);	printf("%10s : New 첫번째 맵부터 다시 시작 (움직임 횟수 기록 삭제)", "n");
						gotoxy(DISHELP_PRINT_X, DISHELP_PRINT_Y + 7);	printf("%10s : Exit 저장 & 게임 종료", "e");
						gotoxy(DISHELP_PRINT_X, DISHELP_PRINT_Y + 8);	printf("%10s : Save 현재 상태 파일에 저장", "s");
						gotoxy(DISHELP_PRINT_X, DISHELP_PRINT_Y + 9);	printf("%10s : File Load 저장된 파일의 시점부터 다시 시작", "f");
						gotoxy(DISHELP_PRINT_X, DISHELP_PRINT_Y + 10);	printf("%10s : 명령 내용 보여줌", "d");
						gotoxy(DISHELP_PRINT_X, DISHELP_PRINT_Y + 11);	printf("%10s : 게임 순위 보여줌", "t");
					}
					else
					{
						disHelp = FALSE;
						printStage(stage);
					}

					break;

				case 't':	case 'T':	//top 게임 순위 보여줌
					//do something
					break;

				default:
					//do something
					break;
				}
				statPrint();
			}

			//clear
			gotoxy(COMMAND_SCAN_X, COMMAND_SCAN_Y);
			for (int i = 0; i < strlen(commandArr); i++)	printf(" ");
			if (success(stage))	run = FALSE;
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
void statPrint(void)
{
	gotoxy(STATUS_PRINT_X, STATUS_PRINT_Y);
	printf("%7s: %4d", "STAGE", stage);
	gotoxy(STATUS_PRINT_X, STATUS_PRINT_Y + 1);
	printf("%7s: % 4d", "UNDO", remainUndo);
	gotoxy(STATUS_PRINT_X, STATUS_PRINT_Y + 2);
	printf("%7s: % 4d", "RECORD", record[stage - 1]);
}
int printNewStage(int stage)
{
	clear();
	gotoxy(HELLO_PRINT_X, HELLO_PRINT_Y);
	printf("\tHello %s", name);
	gotoxy(STATUS_PRINT_X, STATUS_PRINT_Y);
	printf("Stage %d", stage);

	mapMaker(stage);
	if (!checkBoxStorage())
	{
		printf("\n\n박스와 보관장소 개수가 다릅니다. 프로그램을 종료합니다.\n");
		return 0;
	}

	mapPrinter();
	playerFinder();

	gotoxy(COMMAND_PRINT_X, COMMAND_PRINT_Y);
	printf("(Command) ");
	statPrint();

	return 1;
}
void printStage(int stage)
{
	clear();
	gotoxy(HELLO_PRINT_X, HELLO_PRINT_Y);
	printf("\tHello %s", name);
	gotoxy(STATUS_PRINT_X, STATUS_PRINT_Y);
	printf("Stage %d", stage);

	mapPrinter();

	gotoxy(COMMAND_PRINT_X, COMMAND_PRINT_Y);
	printf("(Command) ");

	statPrint();
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

void moveplayer(int xplus, int yplus)
{
	gotoxy(PLAYER_PRINT_X, PLAYER_PRINT_Y);
	printf(" ");
	map[playerY][playerX] = '.';
	playerX += xplus;
	playerY += yplus;



	gotoxy(PLAYER_PRINT_X, PLAYER_PRINT_Y);
	printf("@");
	map[playerY][playerX] = '@';

	record[stage - 1]++;
}
int move(int xplus, int yplus)
{
	int bef_playerX = playerX, bef_playerY = playerY;

	if (playerX + xplus >= 0 && playerY + yplus >= 0 && playerX + xplus < mapX && playerY + yplus < mapY)
	{
		if (map[playerY + yplus][playerX + xplus] == '.')	moveplayer(xplus, yplus);
		else if (map[playerY + yplus][playerX + xplus] == '$' && map[playerY + 2 * yplus][playerX + 2 * xplus] == '.'
			&& playerX + 2 * xplus >= 0 && playerY + 2 * yplus >= 0 && playerX + 2 * xplus < mapX && playerY + 2 * yplus < mapY)
		{
			moveplayer(xplus, yplus);

			gotoxy(PLAYER_PRINT_X + 2 * xplus, PLAYER_PRINT_Y + yplus);
			printf("$");
			map[playerY + yplus][playerX + xplus] = '$';
		}
		else if (map[playerY + yplus][playerX + xplus] == '$' && map[playerY + 2 * yplus][playerX + 2 * xplus] == 'O'
			&& playerX + 2 * xplus >= 0 && playerY + 2 * yplus >= 0 && playerX + 2 * xplus < mapX && playerY + 2 * yplus < mapY)
		{
			moveplayer(xplus, yplus);

			gotoxy(PLAYER_PRINT_X + 2 * xplus, PLAYER_PRINT_Y + yplus);
			printf("$");
			map[playerY + yplus][playerX + xplus] = '$';

			//print O as remaining
			//do something
		}
		else if (map[playerY + yplus][playerX + xplus] == 'O')
		{
			moveplayer(xplus, yplus);

			//print O as remaining
			//do something
		}
	}

	if (bef_playerX != playerX || bef_playerY != playerY)	return 1;
	else return 0;
}
void loadMove(int repeat)
{
	record[stage - 1] = 0;
	for (int i = 0; i < repeat; i++)
	{
		switch (log[stage - 1][i])
		{
			//left
		case 'h':	case 'H':
			move(-1, 0);
			break;

			//down
		case 'j':	case 'J':
			move(0, 1);
			break;

			//up
		case 'k':	case 'K':
			move(0, -1);
			break;

			//right
		case 'l':	case 'L':
			move(1, 0);
			break;
		}
	}
}

int success(int stage)
{
	//returns 1 when succeed
	//else returns 0

	//do something
}