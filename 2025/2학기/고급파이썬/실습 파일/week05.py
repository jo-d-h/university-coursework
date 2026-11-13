#09/29 실습 - 6장 Lab 풀어보기
#6_2 PPT p.4 / 책 p.284 리스트 함축 사용하기
"""
numbers = [x for x in range(100) if x % 2 == 0 and x % 3 == 0]
print(numbers)
"""

#6_2 PPT p.5 / 책 p.285 누적값 리스트 만들기
"""
original = [10, 20, 30, 40, 50]
new = [sum(original[0: x + 1]) for x in range(len(original))]
print("원래 리스트: ", original)
print("새로운 리스트: ", new)
"""

#6_2 PPT p.5 / 책 p.286 피타고라스 삼각형
"""
#방법1 - 리스트 함축
new_list = [(x,y,z) for x in range(1,30) for y in range(x,30) for z in range(y,30) if x**2 + y**2 == z**2]
print(new_list)

#방법2 - 풀어서
new_list = []
for x in range(1, 30):
    for y in range(x, 30):
        for z in range (y, 30):
            if x**2 + y**2 == z**2:
                new_list.append([x, y, z])
print(new_list)

#방법3 - 반쯤 풀어서
list_a = [(x, y, z) for x in range(1, 30) for y in range(x, 30) for z in range(y, 30)]
new_list = [(x, y, z) for (x, y, z) in list_a if x**2 + y ** 2 == z ** 2]
print(new_list)
"""

#6_3 PPT p.5 / 책 p. 291 체커 보드 패턴 만들기
"""
table = []

#화면 출력
def printList(mylist):
    for row in range(len(mylist)):
        for col in range(len(mylist[0])):
            print(mylist[row][col], end = " ")
        print()
        
#2차원 리스트 초기화
def init(mylist):
    for row in range(len(mylist)):
        for col in range(len(mylist[0])):
            if (row + col) % 2 == 0:
                table[row][col] = 1

#2차원 리스트 생성
for row in range(10):
    table += [[0] * 10]
    
init(table)
printList(table)
"""

#6_3 PPT p.7 / 책 p.293 전치 행렬 계산
"""
#방법1 - 이중 for문
matrix = [[1, 2, 3], [4, 5, 6], [7, 8, 9]]
print("원래 행렬=",matrix)
transposed = []

for i in range(3):
    transposed_row = []
    for row in matrix:
        transposed_row.append(row[i])
    transposed.append(transposed_row)
    
print("전치 행렬=", transposed)

matrix = [[1, 2, 3], [4, 5, 6], [7, 8, 9]]
print("원래 행렬=", matrix)

# 전치 행렬
transposed = [[matrix[x][y] for x in range(3)] for y in range(3)]
print("전치 행렬=", transposed)
"""
#6_3 PPT p.8 / 책 p.295 Tic-Tac-Toc
board= [[' ' for x in range (3)] for y in range(3)]
while True:
    #게임 보드를 그린다.
    for r in range(3):
        print(" " + board[r][0] + "| " + board[r][1] + "| " + board[r][2])
    if (r != 2):
        print("---|---|---")
        
    #사용자로부터 좌표를 입력받는다
    x = int(input("다음 수의 x좌표를 입력하시오: "))
    y = int(input("다음 수의 y좌표를 입력하시오: "))
    
    #사용자가 입력한 좌표를 검사한다
    if board[x][y] != ' ':
        print("잘못된 위치입니다. ")
    else:
        board[x][y] = 'X'
        
    #컴퓨터가 놓을 위치를 결정한다. 첫 번째로 발견하는 비어있는 칸에 놓는다
    done = False
    for i in range(3):
        for j in range(3):
            if board[i][j] == ' ' and not done:
                board[i][j] = 'O';
                done = True
                break;


