#p.234(조건문)
#if-else문
# !! else는 if 중괄호 끝나고 같은 줄에 사용해야 오류 안 남!

type <- 'A'

if(type == 'B') {
  bonus <- 200
} else
{
  bonus <- 100
} 
bonus

height <- 200

if(height >= 170) {
  print("키가 큼")
} else{
    print("키가 작음")
  }
height


#ifelse() 함수
#팝업창으로 입력을 받으려면 패키지에서 svDialogs 체크 확인해야!

#문제 1
#1. 온도 변수 만들기 / 2. 온도 변수 사용자 값 입력
tmp <- as.numeric(dlgInput("온도를 입력하세요")$res) #숫자형으로 변환

#3. 조건문 사용
if(tmp >= 20) {
  print("따뜻하다") #3-1. 20도 이상이면 따뜻하다 출력
} else {
  print("춥다") #3-2. 아니면 춥다 출력
}

#문제 2
#1. a, b, c 변수 만들기
#2. a, b에 정수 값 입력(바로 넣기)
#3-1. a가 크면 c에 a값 넣기
#3-2. b가 크면 c에 b값 넣기
a <- 10; b <- 20; c <- 0;

#방법 1
if (a > b) {
  c <- a
} else
{
  c <- b
}

#방법 2

c <- ifelse(a > b, a, b)
print(c)

#문제 3
#1. cost 변수 만들기
#2. cost에 값 넣기
#3. cost가 10,000 이상이면 "밥사먹기" 출력
#3-1. 아니면 "굶기" 출력 - ifelse( ) 함수 사용

cost <- as.numeric(dlgInput("지금 가지고 있는 현금")$res) #숫자형으로 변환
x <- ifelse(cost >= 10000, "밥 사먹기", "굶기")
#!ifelse 함수 사용시 print로 출력하면 중복으로 출력됨!

print(x)




