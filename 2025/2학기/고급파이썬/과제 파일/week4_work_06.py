def f_6_3():
    num = [20, 1, 12, 9, 18]
    
    for asterisk in num:
        print(asterisk, "\t", "*" * asterisk) 


def f_6_4():
    data = [i for i in range(1, 11)]
    print("실행전", data)
    
    data = [i if i < 3 or i > 8 else -i for i in data]
    print("실행후", data)
 
    
def f_6_8():
    a = [1, 2, 3, 4, 5]
    b = [1, 3, 3, 4, 5, 6, 7]
    print("a = ", a)
    print("b = ", b)
    
    result = [i for i in a if i in b]
    print("결과 = ", result)


def f_6_12():
    array = [[ ['#' for col in range(4)] for k in range(3)] for row in range(2)]
    print(array)    


def f_6_14():
    import random

    board = [[False for x in range(10)] for y in range(10)]
    
    for r in range(10):
        for c in range(10):
            if random.random() < 0.3:
                board[r][c] = True
    
    for r in range(10):
        for c in range(10):
            if board[r][c]:
                print("# ", end="")
            else:
                print(". ", end="")
        print()

    
    
def f_6_66():
    string = input("문자열을 입력하세요: ")
    
    for i in range(1, len(string) + 1):
        print(string[:i])
        
    for i in range(len(string) - 1, 0, -1):
        print(string[:i])
    
f_6_3()
f_6_4()
f_6_8()
f_6_12()
f_6_14()
f_6_66()