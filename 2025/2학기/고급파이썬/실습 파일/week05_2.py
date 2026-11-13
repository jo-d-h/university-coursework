#7_1 PPT p.14 / 책 p.323 문자열의 공통 문자
"""
s1 = input("첫 번째 문자열:")
s2 = input("두 번째 문자열:")

list1 = list( set(s1) & set(s2) ) #세트로 만들고 교집합 연산을 한다

print("공통적인 글자:", end=" ")
for i in list1:
    print(i, end=" ")
"""

#7_2 PPT p.15 / 책 p.324 중복되지 않은 단어의 개수 세기
"""
txt = input("입력 텍스트: ")
words = txt.split(" ")
yes = set(words)

print("사용된 단어의 개수= ", len(yes))
print(yes)
"""

#7_2 PPT p.7 / 책 p.334 학생 성적 처리
"""
score_dic = {
    'Kim': [99, 83, 95],
    'Lee': [68, 45, 78],
    'Choi': [25, 56, 69]
}

for name, scores in score_dic.items():
    print(name, "의 평균성적=", sum(scores) / len(scores))
"""

