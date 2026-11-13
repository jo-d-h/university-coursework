#기존 평균 구하기
score.1 <- 68
score.2 <- 95
score.3 <- 83
score.4 <- 76
score.5 <- 90

total <- score.1 + score.2 + score.3 + score.4 + score.5

avg <- total / 5
avg

#벡터 / 1차원 함수
score <- c(68, 95, 83, 76, 90) #combine(조합? 결합?) 함수 / 벡터 변수 만듬
mean(score) #score의 평균

v1 <- 50:90
v2 <- 1:100

v1
v2

#c 함수
v3 <- c(1, 2, 5, 1:100)
v3

v4 <- c(1:10, 30:50)
v4

#seq 함수
v5 <- seq(1, 21, 3) #시작값, 종료값, 간격!
v5

v6 <- seq(0.1, 1.0, 0.2)
v6

#0.1 ~ 1.0 사이에서 0.2씩 증가 / 이후 1 ~ 30 사이에서 7씩 증가
#표현은 실수형으로!!
v7 <- c(seq(0.1, 1.0, 0.2), seq(1, 30, 7))
v7

#rep 함수
v8 <- rep(1, times = 100) #1을 100번 반복
v8

v9 <- rep(1, 5) #rep 함수 times 생략 가능
v9

v10 <- rep(1:5, 3) #1 ~ 5를 3번 반복
v10

v11 <- rep(c(1, 3, 5, 7, 9), 10) #1,3,5,7,9를 10번 반복
v11

v12 <- rep(c(1,3,5,7,9), each = 3) #1,3,5,7,9를 하나하나씩 3번씩 반복
v12 #111 333 555 777 999

v13 <- rep(c('a', 'b', 'c'), each = 3) #문자형도 가능
v13


#p.92 확인문제
#1)
v14 <- rep(c('a', 'b', 'c'), 3)
v14
#2)
v15 <- rep(c('a', 'b', 'c'), each = 3)
v15


#벡터에 이름 붙이기
absent <-  c(10, 5, 8, 0, 1)
names(absent) <- c('Mon', 'Tue', 'Wed', 'Thu', 'Fri')
absent #absent의 값 출력(이름 붙인)
names(absent) 
