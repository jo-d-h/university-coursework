def f_5_9():
    a = int(input("첫 번째 정수: "))
    b = int(input("두 번째 정수: "))

    while b != 0:
        a, b = b, a % b
    print("최대공약수:", a)
    
def f_5_13():
    import random
    
    while True:
        print("===========dice_game() 호출===========")
        human = random.randint(1, 6)
        computer = random.randint(1, 6)
        
        print("인간: 주사위값 = ", human)
        print("컴퓨터: 주사위값 = ", computer)
        
        if human > computer:
            print("인간승리")
        elif computer > human:
            print("컴퓨터승리")
        else:
            print("무승부")
        
        print("===========dice_game() 복귀===========")

        cont = input("중단할까요? Y/N: ")
        if cont.lower() == "y":
            break
    
    
def f_5_15():
    import turtle
    t = turtle.Turtle()
    t.shape("turtle")
    t.speed(0)
    
    def draw_square(size):
        for i in range(4):
            t.fd(size)
            t.left(90)
            size = size + 5
    
    for i in range(1, 11):
        t.penup()
        t.goto(-i*10, -i*10)
        t.pendown()
        draw_square(i * 20)

    turtle.done()
    
    
f_5_9()
f_5_13()
f_5_15()