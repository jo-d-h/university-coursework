#09/24 실습 - 6장 Lab 풀어보기

#6_1 PPT p.15 / 책 p.265 성적 처리 프로그램
"""
STUDENTS = 5
scores = []
count = 0

for i in range(STUDENTS):
    score = int(input("성적을 입력하시오: "))
    scores.append(score)   
    
    if (scores[i] >= 80):
        count += 1
    
print("성적 평균= ", sum(scores) / len(scores))
print("최대점수= ", max(scores))
print("최소점수= ", min(scores))
print("80점 이상= ", count)
"""

#6_1 PPT p.16 / 책 p.266 리스트에서 2번째로 큰 수 찾기
"""
lists = [-7, 2, 3, 8, 6, 6, 75, 38, 3, 2]

#방법1 - 정렬 후 뒤에서 2번째 요소 출력
lists.sort()
print("두 번째로 큰 수= ", lists[-2])

#방법2 - 최댓값 지우고 다음 큰 수 출력
lists.remove(max(lists))
print("두 번째로 큰 수= ", max(lists))
"""

#6_1 PPT p.16 / 책 p.267 콘테스트 평가 - 패스
"""
scores = [10.00, 9.0, 8.3, 7.1, 3.0, 9.0]
print("제거전", scores)
scores.remove(max(scores))
scores.remove(min(scores))
print("제거후", scores)
"""

#6_1 PPT p.17 / 책 p.268 리스트로 스택 흉내내기
"""
stack = []

for i in range(3):
    f = input("과일을 입력하시오: ")
    stack.append(f)
    
for i in range(3):
    print(stack.pop())
"""

#6_1 PPT p.17 / 책 p.269 친구 관리 프로그램
"""
menu = 0
friend = []

while menu != 9:
    print("--------------------")
    print("1. 친구 리스트 출력")
    print("2. 친구 추가")
    print("3. 친구 삭제")
    print("4. 이름 변경")
    print("9. 종료")
    menu = int(input("메뉴를 선택하시오: "))
    
    
    if menu == 1:
        print(friend)
        
    elif menu == 2:
        name = input("이름을 입력하시오: ")
        friend.append(name)
        
    elif menu == 3:
        del_name = input("삭제할 친구의 이름을 입력하시오: ")
        
        if del_name in friend:
            friend.remove(del_name)
        else:
            print("해당 이름은 친구 리스트에 존재하지 않음")
            
    elif menu == 4:
        cha_old_name = input("변경하고 싶은 이름을 입력하시오: ")
        
        if cha_old_name in friend:
            index = friend.index(cha_old_name)
            cha_new_name = input("새로운 이름을 입력하시오: ")
            friend[index] = cha_new_name
            
        else:
            print("해당 이름은 친구 리스트에 존재하지 않음")
"""

#6_1 PPT p.24 / 책 p.278 리스트 슬라이싱
"""
numbers = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
reversed = numbers[::-2]
print(reversed)
numbers[1: ] = []
print(numbers)
"""

#6_1 PPT p.25 / 책 p.281 리스트 변경 함수
"""
wage = [200, 250, 300, 200, 500]

def modify(values, factor):
    for i in range(len(values)):
        values[i] = values[i] * factor
        
print("인상전", wage)
modify(wage, 1.3)
print("인상후", wage)
"""