#7_2 PPT p.8 / 책 p.335 단어 카운터 만들기
"""
text_data ="Create the highest, grandest vision possible for your life, because you become what you believe"

word_dic = {}

for w in text_data.split():
    if w in word_dic:
        word_dic[w] += 1
    else:
        word_dic[w] = 1
        
for w, count in sorted(word_dic.items()):
    print(w, "의 등장 횟수=", count)
"""

#7_2 PPT p.14 / 책 p.343 회문 검사하기
"""
s = input("문자열을 입력하시오: ")
s1 = s[::--1]

if (s == s1):
    print("회문입니다.")
else:
    print("회문이 아닙니다.")
"""

#7_2 PPT p.19 / 책 p. 349 머리 글자어 만들기

#7_2 PPT p.20 / 책 p. 350 문자열 분석

#7_2 PPT p.20 / 책 p. 350 이메일 주소 분석
"""
address=input("이메일 주소를 입력하시오: ")
(id, domain) = address.split("@")

print(address)
print("아이디:"+id)
prnit("도메인:"+domain)
"""

#7_2 PPT p.21 / 책 p. 351 트위터 메시지 처리

#7_2 PPT p.21 / 책 p. 352 OTP 발생 프로그램

#7_2 PPT p.22~23 Solution(중요! 시험!)

#8_1 PPT p.8 / 책 p.370 TV 클래스 정의
"""
class Television:
    def __init__(self, channel, volume, on):
        self.channel = channel
        self.volume = volume
        self.on = on
    
    def show(self):
        print(f"Channel: {self.channel}, Volume: {self.volume}, On: {self.on}")

    def setChannel(self, channel):
        self.channel = channel

t = Television(9, 10, True)
t.show

t.setChannel(11)
t.show()
"""

#8_1 PPT p.9 / 책 p.378 원 클래스 작성
"""
import math

class Circle:
    def __init__(self, radius = 0):
        self.radius = radius
    
    def getArea(self):
        return math.pi * self.radius * self.radius

    def getParameter(self):
        return 2 * math.pi * self.radius
    
c = Circle(10)
print("원의 면적", c.getArea())
print("원의 둘레", c.getParameter())
"""

#8_1 PPT p.13 / 책 p.384 은행계좌
"""
class BankAccount:
    def __init__(self):
        self.__balance = 0
        
    def withdraw(self, amount):
        self.__balance -= amount
        print("통장에 ", amount, "가 입금되었음")
        return self.__balance
    
    def deposit(self, amount):
        self.__balance += amount
        print("통장에서 ", amount, "가 출금되었음")
        return self.__balance
    
a = BankAccount()
a.deposit(100)
a.withdraw(10)
"""




