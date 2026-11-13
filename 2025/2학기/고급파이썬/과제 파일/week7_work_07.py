def f_7_1():
    ori = [80, 20, 20, 30, 60, 30]
    cha = sorted(set(ori))
    print("주어진 리스트:", ori)
    print("정리된 리스트:", cha)

def f_7_6():
    colors = ["red", "green", "blue"]
    values = ["#FF0000", "#008000", "#0000FF"]
    
    dic = dict(zip(colors, values))
        
    print(dic)

def f_7_7():
    mydict = {}
    
    for i in range(2):
        date = input("날짜를 입력하시오: ")     
        plan = input("일정을 입력하시오: ")
        
        if date in mydict:
            mydict[date].append(plan)
        else:
            mydict[date] = []
            mydict[date].append(plan)

    print(mydict)

def f_7_12():
    text = input("문자열을 입력하시오: ")
    no_text = input("금칙어를 입력하시오(공백으로 구분): ")
    
    no_list = no_text.split()

    for word in no_list:
        text = text.replace(word, '*' * len(word))

    print(text)

def f_7_13():
    text = input()
    letter_count = sum(1 for x in text if x.isalpha())
    digit_count = sum(1 for x in text if x.isdigit())
    result = {"LETTERS": letter_count, "DIGITS": digit_count}
    print(result)
    
def f_7_16():
    import random
    s = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890!@#$%^&*()"
    password = "".join(random.sample(s, 8))
    print("생성된 암호 = ", password)

f_7_1()
f_7_6()
f_7_7()
f_7_12()
f_7_13()
f_7_16()