###나무
##import turtle
##t = turtle.Turtle()
##
##
##def tree(n):
##    if n < 20:
##        return
##    
##    t.fd(n)
##    
##    t.right(40)
##    tree(n - 20)
##
##    t.left(40)
##    tree(n - 20)
##
##    t.left(40)
##    tree(n - 20)
##
##    t.right(40)
##    t.backward(n)
##    
##
##
##t.speed(0)
##
##t.up()
##t.goto(0, -200)
##t.down()
##
##t.left(90)
##tree(90)

#눈송이
import turtle
t = turtle.Turtle()
t.speed(0)

koch_size = 800
koch_step = 5
t.hideturtle()

def koch_line(length, step):
    if step > 0:
        length = length / 3;
        for angle in [60, -120, 60, 0]:
            koch_line(length, step - 1)
            t.left(angle)

    else:
        t.forward(length)

koch_line(koch_size, koch_step)
