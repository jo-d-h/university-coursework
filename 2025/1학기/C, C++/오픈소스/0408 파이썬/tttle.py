import turtle
t = turtle.Turtle()

size = 50

###정사각형
##t.fd(size)
##t.right(90)
##t.fd(size)
##t.right(90)
##t.fd(size)
##t.right(90)
##t.fd(size)
##t.right(90)
##
###정삼각형
##t.fd(size)
##t.left(120)
##t.fd(size)
##t.left(120)
##t.fd(size)
##t.left(120)
##
##t.penup() #펜 들고 이동
##t.fd(150)
##t.pendown() #펜 내리기
##
###원
##t.circle(90)
##
###2)
##def square (length):
##    for i in range(4):
##        t.forward(length)
##        t.left(90)
##
##t.up()
##t.goto(-200, 0)
##t.down()
##
##square(100)
##
##t.up()
##t.goto(0, 0)
##t.down()
##
##square(100)
##
##t.up()
##t.goto(200, 0)
##t.down()
##
##square(100)

#3)
def n_polygon(n, length):
    for i in range(n):
        t.forward(length)
        t.left(360//n) #정수 나눗셈은 '//'
        t.speed(0)

for i in range(17):
    t.left(20)
    n_polygon(6, 100)
