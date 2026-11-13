#3장 연습문제

#6번 문제 코드
"""
from random import randint

user = int(input("선택하시오(1: 가위 2:바위 3: 보) "))
com = randint(1, 3)

if user == com :
    result = "비겼음"
elif (user - com) % 3 == 1:
    result = "사용자가 이겼음"
else:
    result = "컴퓨터가 이겼음"
print(result)
"""

#14번 문제 코드
"""
from math import sqrt

a = int(input("a를 입력하시오: "))
b = int(input("b를 입력하시오: "))
c = int(input("c를 입력하시오: "))

r = b**2 - 4*a*c

if r < 0:
    print("실근이 존재하지 않습니다.")
elif r == 0:
    gap = -b / (2*a)
    print(f"실근은 {gap:.6f}입니다.")
else:
    gap1 = (-b + sqrt(r)) / (2*a)
    gap2 = (-b - sqrt(r)) / (2*a)
    print(f"실근은 {gap1:.6f}과 {gap2:.6f}입니다.")
"""

#4장 연습문제

#5번 문제 코드
"""
su = int(input("정수를 입력하시오: "))

for i in range(1, 1 + su):
    for j in range(1, i + 1):
        print(j, end=" ")
    print()
"""

#6번 문제 코드
"""
import math
print("각도   sin값     cos값")
for degree in range(0, 91, 10):
    radian = 3.141592 * degree / 180.0
    sin_value = math.sin(radian)
    cos_value = math.cos(radian)
    print(f"{degree:3d} {sin_value:7.3f} {cos_value:7.3f}")
"""

#7번 문제 코드
"""
import turtle
import math

t = turtle.Turtle()
turtle.bgcolor("white")

t.penup()
t.goto(-360, 0)   # x축 시작점
t.pendown()

for degree in range(-360, 361):
    radian = math.radians(degree)
    y = math.sin(radian) * 100
    t.goto(degree, y)

turtle.done()
"""

#13번 문제 코드

n = int(input("몇 번째 항까지 구할까요? "))

f0 = 0
f1 = 1

print("0", end="")

if (n > 0):
    print(f", {f1}", end="")

for i in range(2, n + 1):
    f2 = f0 + f1
    print(f", {f2}", end="")
    f0 = f1
    f1 = f2






