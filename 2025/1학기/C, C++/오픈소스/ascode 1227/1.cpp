//brute force(다중 반복 : 시간초과!)
/*
int getMAx(int arr[], int N)
{
	int best = 0;
	for (int i = 0; i < N; ++i)
	{
		for (int j = i; j < N; ++j)
		{
			int sum = 0;

			for (int k = i; k <= j; ++j)
				sum += arr[k];

			best = max(best, sum);
		}
	}
	return best;
}
*/

//방법 2 (반복문 2개로 줄임)
/*
int getMax(int arr[], int N)
{
	int best = 0;
	for (int i = 0; i < N; ++i)
	{
		int sum = 0;

		for (int j = i; j < N; ++j)
		{
			sum += arr[j];
			best = max(best, sum);
		}
	}
return best;
}
*/

//방법 3 (반복문 1개!)
#include <stdio.h>
#pragma warning(disable:4996)

//큰 값 반환
int max(int a, int b)
{
	if (a > b)
		return a;
	else
		return b;
}

//최대 부분합, 시작 끝점과 합 출력
int getMax(int arr[], int N)
{
	int best = -1000000000;
	int sum = 0;
	int start = 0, end = 0, temp_start = 0;

	for (int i = 0; i < N; ++i)
	{
		sum += arr[i];

		if (sum > best)
		{
			best = sum;
			start = temp_start;
			end = i;
		}

		else if (sum == best && temp_start > start)
		{
			start = temp_start;
			end = i;
		}

		if (sum < 0)
		{
			sum = 0;
			temp_start = i + 1;
		}
	}
	printf("%d %d %d\n", start + 1, end + 1, best);
}

int main()
{
	int t, N;
	int e[1001] = {};
	scanf("%d", &t);

	for (int i = 0; i < t; i++)
	{
		scanf("%d", &N);

		for (int k = 0; k < N; k++)
		{
			scanf("%d", &e[k]);
		}

		getMax(e, N);
	}
	return 0;
}
