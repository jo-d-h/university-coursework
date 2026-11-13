//#pragma warning(disable:4996)
#include <stdio.h>
#define SIZE 5

int Bingo[SIZE][SIZE] = {1,2,3,4,5,
						6,7,8,9,10,
						11,12,13,14,15,
						16,17,18,19,20}; //보드 크기

void init(); //초기화
void printBoard(); //보드판 출력
void deleteNum(int); //보드판에서 맞는 숫자 지움
int checkBoard();

int main() 
{

	//int num, lines;

	//init(); //초기화

	//printBoard(); //보드판 출력

	/*
	while (1) {
		printf("지울 번호를 입력하세요 : ");
		scanf("%d", &num);

		deleteNum(num); //숫자 지움

		printBoard();

		lines = checkBoard(); //빙고인지 판단

		if (lines == 1) {
			printf("한 줄 지워짐!!\n");
		}
		else if (lines == 2) {
			printf("두 줄 지워짐!!\n");
		}
		else if (lines >= 3) {
			printf("빙고!!!!!!\n");
			break;
		}
	}
	
	return 0;
	*/

//━┃┏ ┓┗ ┛┣ ┫┳ ┻ ╋

	void printBoard(); 

	printf("┏");
	for (int i = 0; i < SIZE - 1; i++) 
	{
		printf("━━┳");
	}
		printf("━━┓\n");

	for (int i = 1; i < SIZE-1; i++)



	return 0;
}