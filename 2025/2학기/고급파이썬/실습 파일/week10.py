#10_1 PPT p.8 / 책 p.465 매출 파일 처리
"""
#실행 전 우측상단 경로입력창에서 주소 바꿔야 함
#C:\Programming\고급파이썬\실습 파일
infilename = input("입력 파일 이름: ");
outfilename = input("출력 파일 이름: ");

infile = open(infilename, "r")
outfile = open(outfilename, "w")

sum = 0
count = 0

line = infile.readline()

while line != "" :
    s = int(line)
    sum += s
    count += 1
    line = infile.readline()
    
outfile.write("총매출 = "+ str(sum)+"\n")
outfile.write("평균 일매출 = "+ str(sum/count))

infile.close()
outfile.close()
"""

#10_1 PPT p.15 / 책 p.473 CSV 파일 처리

import csv

f = open(r"C:\Programming\고급파이썬\실습 파일\weather.csv")
data = csv.reader(f)
header = next(data) #첫줄(헤더) 제거

temp = 1000
for row in data: #최소값 찾기
    if temp > float(row[3]): #최저 기온은 idx=3
        temp = float(row[3])
    
print('가장 추웠던 날은', temp, '입니다')
f.close()

