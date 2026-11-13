#25.09.03 제1장 - 1주차 실습 (설치, 버전 확인, 터틀 그래픽)

import tensorflow
print(tensorflow.__version__) # 2.18.0, Colab(2.18.0)

import keras
print(keras.__version__) # 3.8.9, Colab(3.8.0)

# 파이썬 version
import sys
print(sys.version) # 3.11, , Colab(3.11.3)

#p.19
print("Hello World!")
print(5, end = '\n')

#p.24 터틀 그래픽
import turtle # (1) “turtle” 모듈 불러오기헤더파일과 유사개념
t = turtle.Turtle() # (2) turtle 객체 생성
t.shape("turtle") # (3) 커서모양 arrow, circle, square, triangle, classic
t.forward(100) # (4) 머리방향으로 100 만큼 나가며 직선 그림
t.left(90) # (5) 머리가 좌회전 90도
t.right(45)
t.forward(50)
turtle.mainloop() # (6) 창이 없어지기 않고 유지, 클릭하면 종료
turtle.bye()

#p.25(삼각형)
import turtle
t = turtle.Turtle()
t.shape("turtle")
t.forward(100)
t.left(120)
t.forward(100)
t.left(120)
t.forward(100)
turtle.mainloop()
turtle.bye()

#p.25
import turtle

colors = ["red", "purple", "blue", "green", "yellow", "orange" ]
t = turtle.Turtle()

turtle.bgcolor("black")
t.speed(0) # 이동 속도 0(가장 느림) ~10(가장 빠름)
t.width(3) # 선 굵기
length = 10

while length < 300:
    t.forward(length)
    t.pencolor(colors[length%6])
    t.right (86) # 86도 우회전
    length += 5
    
turtle.mainloop()
turtle.bye()