#include<iostream>
#include <ctime>
using namespace std;

class Dice
{
private:
	int face; //주사위 값(1 ~ 6)
public:
	Dice()
	{
		face = 1; //초기 값
	}

	void roll() //주사위 던지기
	{
		face = (int) rand() % 6 + 1; //1 ~ 6
	}

	int getFace() //굴려진 주사위 값 출력
	{
		return face;
	}
};

void p3()
{
	Dice D1;
	srand(time(NULL));
	for (int i = 0; i < 10; i++) 
	{
		D1.roll(); // 주사위를 던지고 숫자를 출력
		cout<<"주사위의 숫자는 "<<D1.getFace()<<"입니다."<<endl;
	}
}

class Complex
{
private:


public:

};

void p6()
{
	double r1, i1, r2, i2; Complex c1, c2, c3;
	cout << "1번째 실수부와 허수부를 입력하세요 :"; cin >> r1 >> i1;
	cout << "2번째 실수부와 허수부를 입력하세요 :"; cin >> r2 >> i2;
	cout << "===================================" << endl;
	c1.setComplex(r1, i1); cout << "복소수는 : "; c1.Print();
	c2.setComplex(r2, i2); cout << "복소수는 : "; c2.Print();
	c3.Add(r1, r2, i1, i2); cout << "합은 : "; c3.Print();
	c3.Sub(r1, r2, i1, i2); cout << "뺄셈은 : "; c3.Print();
	cout << "===================================" << endl;
}

void main()
{
	p3();
	p6();
}