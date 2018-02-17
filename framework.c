#define _CRT_SECURE_NO_WARNINGS
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DONW	 80

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<ctype.h>
#include<windows.h>
#include<time.h>
#include"Screen.h""

typedef enum _GAME_STAGE{READY, RUNNING, SUCCESS, FAIL, RESULT}GAME_STAGE;
GAME_STAGE  GameStage = READY;


typedef struct _PLAYER {
	int nCenterX, nCenterY;  // ���ΰ� �߽���ǥ
	int nPosX, nPosY;          // ���ΰ� ��ġ�� ��ǥ
	int nX, nY;					//  ���ΰ� ��±��� ��ǥ = (��ġ����ǥ - �߽���ǥ)�������� ���� ��µǰ� ��ġ�� ��ǥ�� �Ӹ��� ��µȴ�.

}PLAYER;
PLAYER Player01;
char strPlayer01[] = "���ܡ��";

typedef struct _BALL {
	int nIsReady; // ����ü �غ����
	int nPosX, nPosY; // ����ü ��ġ
	clock_t MoveTime; // ����ü �̵��ð�
	clock_t OldTime;   // ����ü ���� �̵��ð�
}BALL;
BALL Ball01;


typedef struct _ENEMY {
	int nPosX, nPosY; // �� ��ü�� ��ġ
	int nLength;       // �� ��ü��  ũ��
	int nLineX[7];     // �� ��ü�� ����ũ���� ��ġ
	int nDist;			 // �� ��ü�� �̵��Ÿ�
	clock_t MoveTime;  // �� ��ü�� �̵��ð�
	clock_t OldTime;    // �� ��ü�� �����̵��ð�
}ENEMY;
ENEMY enemy01; // �⺻ ��1

// ���� ���
typedef struct _SCORE {
	int nSuccess;
	int nFail;
	int nTotal;
}SCORE;
SCORE Score01;

typedef struct _EFFECT { // ��¦�� ����Ʈ �߰�����!!��
	clock_t StartTime;
	clock_t StayTime;
}EFFECT;
EFFECT IfSuccess;
int IsSuccess;

void SuccessMessage(int x, int y) {
	ScreenPrint(x, y, "   ��    ");
	ScreenPrint(x, y+1, "   Hit!!    ");
	ScreenPrint(x, y+2, "   ��    ");
}

void ReadyForGame(int x, int y) {
	ScreenPrint(x, y, "����������������������������������");
	ScreenPrint(x, y+1, "��                                                              ��");
	ScreenPrint(x, y+2, "��                                                              ��");
	ScreenPrint(x, y+3, "��                                                              ��");
	ScreenPrint(x, y+4, "��                                                              ��");
	ScreenPrint(x, y+5, "��                                                              ��");
	ScreenPrint(x, y+6, "��\t\tPress 'Enter' to Start Game!!\t                ��");
	ScreenPrint(x, y+7, "��                                                              ��");
	ScreenPrint(x, y+8, "��                                                              ��");
	ScreenPrint(x, y+9, "��                                                              ��");
	ScreenPrint(x, y+10, "��                                                              ��");
	ScreenPrint(x, y+11, "��                                                              ��");
	ScreenPrint(x, y+12, "��                                                              ��");
	ScreenPrint(x, y+13, "��                                                              ��");
	ScreenPrint(x, y+14, "����������������������������������");
}

void ReadyForGame2(int x, int y) {
	ScreenPrint(x, y, "����������������������������������");
	ScreenPrint(x, y + 1, "��                                                              ��");
	ScreenPrint(x, y + 2, "��                                                              ��");
	ScreenPrint(x, y + 3, "��                                                              ��");
	ScreenPrint(x, y + 4, "��                                                              ��");
	ScreenPrint(x, y + 5, "��                                                              ��");
	ScreenPrint(x, y + 6, "��\t\t                         \t\t        ��");
	ScreenPrint(x, y + 7, "��                                                              ��");
	ScreenPrint(x, y + 8, "��                                                              ��");
	ScreenPrint(x, y + 9, "��                                                              ��");
	ScreenPrint(x, y + 10, "��                                                              ��");
	ScreenPrint(x, y + 11, "��                                                              ��");
	ScreenPrint(x, y + 12, "��                                                              ��");
	ScreenPrint(x, y + 13, "��                                                              ��");
	ScreenPrint(x, y + 14, "����������������������������������");
}
clock_t OldTimeForTwinkle;

void GameResult(int x, int y) {
	ScreenPrint(x, y, "����������������������������������");
	ScreenPrint(x, y + 1, "��                                                              ��");
	ScreenPrint(x, y + 2, "��      Result                                                  ��");
	ScreenPrint(x, y + 3, "��    ======                                                    ��");
	char nSuccess[100];
	sprintf(nSuccess, "��      Success     : %d ȸ                                      ��", Score01.nSuccess);
	ScreenPrint(x, y + 4, nSuccess);
	char nFail[100];
	sprintf(nFail, "��      Fail        : %d ȸ                                      ��", Score01.nFail);
	ScreenPrint(x, y + 5, nFail);
	ScreenPrint(x, y + 6, "��                                                              ��");
	ScreenPrint(x, y + 7, "��                                                              ��");
	ScreenPrint(x, y + 8, "��                                                              ��");
	ScreenPrint(x, y + 9, "��                                                              ��");
	ScreenPrint(x, y + 10, "��                                                              ��");
	ScreenPrint(x, y + 11, "��                                                              ��");
	ScreenPrint(x, y + 12, "��                                                              ��");
	ScreenPrint(x, y + 13, "��                                                              ��");
	ScreenPrint(x, y + 14, "����������������������������������");
}


void init() {
	// ���ΰ� �ʱ�ȭ
	Player01.nCenterX = 4;
	Player01.nCenterY = 0;
	Player01.nPosX = 20;
	Player01.nPosY = 17;
	Player01.nX = Player01.nPosX - Player01.nCenterX;
	Player01.nY = Player01.nPosY - Player01.nCenterY;

	// ����ü �ʱ�ȭ
	Ball01.nIsReady = 1;
	Ball01.nPosX = Player01.nPosX;
	Ball01.nPosY = Player01.nPosY -1;
	Ball01.MoveTime = 100; // ����ü �ӵ��� 0.1�� �� 1colum

	// �� ��ü �ʱ�ȭ
	int nLength, i;
	enemy01.nPosX = 20;
	enemy01.nPosY = 2;
	enemy01.nLength = 1;
	enemy01.MoveTime = 100; // 0.1�� ���� 1ĭ�̵� 
	enemy01.OldTime = clock(); // �����̵��ð� - ���α׷����� ������ ���� ����.
	enemy01.nDist = 1; // 1colum�� �����δ�.
	nLength = enemy01.nLength * 2 + 1;
	for (i = 0; i < nLength; i++) {
		enemy01.nLineX[i] = enemy01.nPosX + 2 * (i + 1);
	}

	// ���� �ʱ�ȭ
	Score01.nSuccess = 0;
	Score01.nFail = 0;
	Score01.nTotal = 0;

	// ����Ʈ �ʱ�ȭ
	IfSuccess.StayTime = 2000; // 2�� ���

	OldTimeForTwinkle = clock();

}



void Update() {
	clock_t CurTime = clock();
	if (Score01.nSuccess + Score01.nFail == 10) {
		GameStage = RESULT;
	}
	switch (GameStage) {
	case READY:
		break;
	case RUNNING:
		// ����Ʈ
		if (IsSuccess == 1) {
			if (CurTime - IfSuccess.StartTime > IfSuccess.StayTime) {
				IsSuccess = 0;
			}
		}


		// �� ��ü ������ ����
		int nLength = enemy01.nLength * 2 + 1;
		if (CurTime - enemy01.OldTime > enemy01.MoveTime) { // ��ü�� ������ �����̵��ð��� �������� 0.1�ʰ� ������ > �޸𸮰��� ������ > ��ü�� �����δ�
			enemy01.OldTime = CurTime;
			if (enemy01.nPosX + enemy01.nDist >= 0 && ((enemy01.nLineX[nLength - 1] + 3) + enemy01.nDist) <= 79) {
				enemy01.nPosX += enemy01.nDist; // ��谪 �ȿ��� ������ �� �̵��Ÿ���ŭ �������� ����Ѵ�.
				for (int i = 0; i < nLength; i++) {
					enemy01.nLineX[i] = enemy01.nPosX + 2 * (i + 1);
				}
			}
			else {
				enemy01.nDist = enemy01.nDist * (-1);
			}
		}




		// ����ü ������ ����
		if (Ball01.nIsReady == 0) {
			if (CurTime - Ball01.OldTime > Ball01.MoveTime) {
				if (Ball01.nPosY > 0) {
					Ball01.nPosY--;
					Ball01.OldTime = CurTime;
				}
				else {
					// �� ��ü�� �浹���� �ʰ� ���� ��� reset.
					if (Ball01.nPosY <= enemy01.nPosY) {
						Ball01.nIsReady = 1;
						Ball01.nPosX = Player01.nPosX;
						Ball01.nPosY = Player01.nPosY - 1;
						Score01.nFail++;
					}
				}
				// �� ��ü�� �浹 ó��
				if (Ball01.nPosX >= enemy01.nLineX[0] && Ball01.nPosX + 1 <= enemy01.nLineX[nLength - 1] + 1) {
					// �� ��ü�� �浹
					if (Ball01.nPosY <= enemy01.nPosY) {
						Ball01.nIsReady = 1;
						Ball01.nPosX = Player01.nPosX;
						Ball01.nPosY = Player01.nPosY - 1;
						Score01.nSuccess++;
						IsSuccess = 1;
						IfSuccess.StartTime = clock();
					}
				}
			}
		}
		else {
			Ball01.nPosX = Player01.nPosX;
		}
		break;
	case RESULT :

		break;
	}



	
}

void Render() {
	clock_t Curtime = clock();
	int nLength, i;
	ScreenClear();
	switch (GameStage) {
	case READY:
		//  ������ ����
		if (Curtime %2000 >700) {
			ReadyForGame(0, 0);
		}
		else {
			ReadyForGame2(0, 0);
		}
		break;

	case RUNNING:
		if (IsSuccess == 1) {
			SuccessMessage(30, 10);
		}


		// ���ΰ� ����ڵ�
		ScreenPrint(Player01.nX, Player01.nY, strPlayer01);

		// ����ü ����ڵ�
		ScreenPrint(Ball01.nPosX, Ball01.nPosY, "��");








		// �� ��ü ����ڵ�

		ScreenPrint(enemy01.nPosX, enemy01.nPosY, "��");
		nLength = enemy01.nLength * 2 + 1;
		for (i = 0; i < nLength; i++) {
			ScreenPrint(enemy01.nLineX[i], enemy01.nPosY, "��");
		}
		ScreenPrint(enemy01.nLineX[nLength - 1] + 2, enemy01.nPosY, "��");

		// ���� ����ڵ�
		char score0[20] = { NULL, };
		sprintf(score0, "�� Ƚ�� : %d ȸ", Score01.nTotal);
		ScreenPrint(80, 0, score0);
		char score[20] = { NULL, };
		sprintf(score, "���� Ƚ�� : %d ȸ", Score01.nSuccess);
		ScreenPrint(80, 1, score);
		char score1[20] = { NULL, };
		sprintf(score1, "���� Ƚ�� : %d ȸ", Score01.nFail);
		ScreenPrint(80, 2, score1);
		break;

	case RESULT :
		GameResult(0,0);
		break;
	}




	ScreenFlipping();
}

void Release() {

}

int main(void) {
	
	ScreenInit();
	init(); // �ʱ�ȭ

	while (1) {
		int nKey;
		while (1) {
			if (_kbhit()) {
				nKey = _getch();
				if (nKey == 13) { // enter �ƽ�Ű�ڵ� = 13
					GameStage = RUNNING;
				}
				if (nKey == 224) {
					nKey = _getch();
					switch (nKey) {
					case LEFT :
						Player01.nPosX--;
						Player01.nX = Player01.nPosX - Player01.nCenterX;
						break;
					case RIGHT:
						Player01.nPosX++;
						Player01.nX = Player01.nPosX - Player01.nCenterX;
						break;
					}
				}
				else if (nKey == 'q') {
					if (Ball01.nIsReady) {
						Ball01.nPosX = Player01.nPosX;
						Ball01.nPosY = Player01.nPosY-1;
						Ball01.OldTime = clock();
						Ball01.nIsReady = 0;
						Score01.nTotal++;
					}
				}
			}








			Update();  // ������ ����
			Render();  // ȭ�����
		}
		

		
	}
	Release(); // ����
	ScreenRelease();
	return 0;
}