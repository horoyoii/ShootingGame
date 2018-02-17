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
	int nCenterX, nCenterY;  // 주인공 중심좌표
	int nPosX, nPosY;          // 주인공 위치할 좌표
	int nX, nY;					//  주인공 출력기준 좌표 = (위치할좌표 - 중심좌표)에서부터 팔이 출력되고 위치할 좌표는 머리가 출력된다.

}PLAYER;
PLAYER Player01;
char strPlayer01[] = "■□●□■";

typedef struct _BALL {
	int nIsReady; // 투사체 준비상태
	int nPosX, nPosY; // 투사체 위치
	clock_t MoveTime; // 투사체 이동시간
	clock_t OldTime;   // 투사체 이전 이동시간
}BALL;
BALL Ball01;


typedef struct _ENEMY {
	int nPosX, nPosY; // 적 객체의 위치
	int nLength;       // 적 객체의  크기
	int nLineX[7];     // 적 개체의 개별크기의 위치
	int nDist;			 // 적 개체의 이동거리
	clock_t MoveTime;  // 적 객체의 이동시간
	clock_t OldTime;    // 적 객체의 이전이동시간
}ENEMY;
ENEMY enemy01; // 기본 적1

// 점수 계산
typedef struct _SCORE {
	int nSuccess;
	int nFail;
	int nTotal;
}SCORE;
SCORE Score01;

typedef struct _EFFECT { // 반짝임 이펙트 추가가능!!ㄷ
	clock_t StartTime;
	clock_t StayTime;
}EFFECT;
EFFECT IfSuccess;
int IsSuccess;

void SuccessMessage(int x, int y) {
	ScreenPrint(x, y, "   ★    ");
	ScreenPrint(x, y+1, "   Hit!!    ");
	ScreenPrint(x, y+2, "   ★    ");
}

void ReadyForGame(int x, int y) {
	ScreenPrint(x, y, "□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□");
	ScreenPrint(x, y+1, "□                                                              □");
	ScreenPrint(x, y+2, "□                                                              □");
	ScreenPrint(x, y+3, "□                                                              □");
	ScreenPrint(x, y+4, "□                                                              □");
	ScreenPrint(x, y+5, "□                                                              □");
	ScreenPrint(x, y+6, "□\t\tPress 'Enter' to Start Game!!\t                □");
	ScreenPrint(x, y+7, "□                                                              □");
	ScreenPrint(x, y+8, "□                                                              □");
	ScreenPrint(x, y+9, "□                                                              □");
	ScreenPrint(x, y+10, "□                                                              □");
	ScreenPrint(x, y+11, "□                                                              □");
	ScreenPrint(x, y+12, "□                                                              □");
	ScreenPrint(x, y+13, "□                                                              □");
	ScreenPrint(x, y+14, "□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□");
}

void ReadyForGame2(int x, int y) {
	ScreenPrint(x, y, "□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□");
	ScreenPrint(x, y + 1, "□                                                              □");
	ScreenPrint(x, y + 2, "□                                                              □");
	ScreenPrint(x, y + 3, "□                                                              □");
	ScreenPrint(x, y + 4, "□                                                              □");
	ScreenPrint(x, y + 5, "□                                                              □");
	ScreenPrint(x, y + 6, "□\t\t                         \t\t        □");
	ScreenPrint(x, y + 7, "□                                                              □");
	ScreenPrint(x, y + 8, "□                                                              □");
	ScreenPrint(x, y + 9, "□                                                              □");
	ScreenPrint(x, y + 10, "□                                                              □");
	ScreenPrint(x, y + 11, "□                                                              □");
	ScreenPrint(x, y + 12, "□                                                              □");
	ScreenPrint(x, y + 13, "□                                                              □");
	ScreenPrint(x, y + 14, "□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□");
}
clock_t OldTimeForTwinkle;

void GameResult(int x, int y) {
	ScreenPrint(x, y, "□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□");
	ScreenPrint(x, y + 1, "□                                                              □");
	ScreenPrint(x, y + 2, "□      Result                                                  □");
	ScreenPrint(x, y + 3, "□    ======                                                    □");
	char nSuccess[100];
	sprintf(nSuccess, "□      Success     : %d 회                                      □", Score01.nSuccess);
	ScreenPrint(x, y + 4, nSuccess);
	char nFail[100];
	sprintf(nFail, "□      Fail        : %d 회                                      □", Score01.nFail);
	ScreenPrint(x, y + 5, nFail);
	ScreenPrint(x, y + 6, "□                                                              □");
	ScreenPrint(x, y + 7, "□                                                              □");
	ScreenPrint(x, y + 8, "□                                                              □");
	ScreenPrint(x, y + 9, "□                                                              □");
	ScreenPrint(x, y + 10, "□                                                              □");
	ScreenPrint(x, y + 11, "□                                                              □");
	ScreenPrint(x, y + 12, "□                                                              □");
	ScreenPrint(x, y + 13, "□                                                              □");
	ScreenPrint(x, y + 14, "□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□");
}


void init() {
	// 주인공 초기화
	Player01.nCenterX = 4;
	Player01.nCenterY = 0;
	Player01.nPosX = 20;
	Player01.nPosY = 17;
	Player01.nX = Player01.nPosX - Player01.nCenterX;
	Player01.nY = Player01.nPosY - Player01.nCenterY;

	// 투사체 초기화
	Ball01.nIsReady = 1;
	Ball01.nPosX = Player01.nPosX;
	Ball01.nPosY = Player01.nPosY -1;
	Ball01.MoveTime = 100; // 투사체 속도는 0.1초 당 1colum

	// 적 객체 초기화
	int nLength, i;
	enemy01.nPosX = 20;
	enemy01.nPosY = 2;
	enemy01.nLength = 1;
	enemy01.MoveTime = 100; // 0.1초 마다 1칸이동 
	enemy01.OldTime = clock(); // 이전이동시간 - 프로그램실행 직후의 값을 가짐.
	enemy01.nDist = 1; // 1colum씩 움직인다.
	nLength = enemy01.nLength * 2 + 1;
	for (i = 0; i < nLength; i++) {
		enemy01.nLineX[i] = enemy01.nPosX + 2 * (i + 1);
	}

	// 점수 초기화
	Score01.nSuccess = 0;
	Score01.nFail = 0;
	Score01.nTotal = 0;

	// 이펙트 초기화
	IfSuccess.StayTime = 2000; // 2초 출력

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
		// 이펙트
		if (IsSuccess == 1) {
			if (CurTime - IfSuccess.StartTime > IfSuccess.StayTime) {
				IsSuccess = 0;
			}
		}


		// 적 객체 데이터 갱신
		int nLength = enemy01.nLength * 2 + 1;
		if (CurTime - enemy01.OldTime > enemy01.MoveTime) { // 객체가 가지는 이전이동시간을 기준으로 0.1초가 지나면 > 메모리값을 변경해 > 객체를 움직인다
			enemy01.OldTime = CurTime;
			if (enemy01.nPosX + enemy01.nDist >= 0 && ((enemy01.nLineX[nLength - 1] + 3) + enemy01.nDist) <= 79) {
				enemy01.nPosX += enemy01.nDist; // 경계값 안에서 움직일 시 이동거리만큼 움직임을 허용한다.
				for (int i = 0; i < nLength; i++) {
					enemy01.nLineX[i] = enemy01.nPosX + 2 * (i + 1);
				}
			}
			else {
				enemy01.nDist = enemy01.nDist * (-1);
			}
		}




		// 투사체 데이터 갱신
		if (Ball01.nIsReady == 0) {
			if (CurTime - Ball01.OldTime > Ball01.MoveTime) {
				if (Ball01.nPosY > 0) {
					Ball01.nPosY--;
					Ball01.OldTime = CurTime;
				}
				else {
					// 적 객체와 충돌되지 않고 나갈 경우 reset.
					if (Ball01.nPosY <= enemy01.nPosY) {
						Ball01.nIsReady = 1;
						Ball01.nPosX = Player01.nPosX;
						Ball01.nPosY = Player01.nPosY - 1;
						Score01.nFail++;
					}
				}
				// 적 객체와 충돌 처리
				if (Ball01.nPosX >= enemy01.nLineX[0] && Ball01.nPosX + 1 <= enemy01.nLineX[nLength - 1] + 1) {
					// 적 객체와 충돌
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
		//  깜빡임 구현
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


		// 주인공 출력코드
		ScreenPrint(Player01.nX, Player01.nY, strPlayer01);

		// 투사체 출력코드
		ScreenPrint(Ball01.nPosX, Ball01.nPosY, "◎");








		// 적 객체 출력코드

		ScreenPrint(enemy01.nPosX, enemy01.nPosY, "■");
		nLength = enemy01.nLength * 2 + 1;
		for (i = 0; i < nLength; i++) {
			ScreenPrint(enemy01.nLineX[i], enemy01.nPosY, "□");
		}
		ScreenPrint(enemy01.nLineX[nLength - 1] + 2, enemy01.nPosY, "■");

		// 점수 출력코드
		char score0[20] = { NULL, };
		sprintf(score0, "총 횟수 : %d 회", Score01.nTotal);
		ScreenPrint(80, 0, score0);
		char score[20] = { NULL, };
		sprintf(score, "성공 횟수 : %d 회", Score01.nSuccess);
		ScreenPrint(80, 1, score);
		char score1[20] = { NULL, };
		sprintf(score1, "실패 횟수 : %d 회", Score01.nFail);
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
	init(); // 초기화

	while (1) {
		int nKey;
		while (1) {
			if (_kbhit()) {
				nKey = _getch();
				if (nKey == 13) { // enter 아스키코드 = 13
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








			Update();  // 데이터 갱신
			Render();  // 화면출력
		}
		

		
	}
	Release(); // 해제
	ScreenRelease();
	return 0;
}