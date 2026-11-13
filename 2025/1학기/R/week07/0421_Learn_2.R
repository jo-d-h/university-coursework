#p.180 매트릭스/데이터프레임에 산술연산
a <- matrix(1:20, 4, 5)
b <- matrix(21:40, 4, 5)

a
b

2 * a #실행만 O, 저장은 X(a값은 변하지 않음)
a <- 2 * a #이렇게 하면 저장 O
a + b #두개 크기가 동일해야만 연산 가능
a <- a * 3 #저장 O
b <- b * 3

c <- a + b #다른 저장방법 : 또 다른 변수 만들어서 저장!

#p.182(자료구조)
class(iris) #iris의 자료구조
class(state.x77) #state.x77의 자료구조

x <- state.x77 #(미국 77개 주의 자료수치)

is.matrix(iris) #iris가 matrix인지 T/F
is.matrix(state.x77)

is.data.frame(iris)
is.data.frame(state.x77)

#p.183 (자료구조 변환)
is.matrix(state.x77)
st <- data.frame(state.x77)
head(st)
tail(st)
class(st)

is.data.frame(iris[,1:4]) #5열은 문자열(팩터)이므로 제외!(1~4열은 수치)
iris.m <- as.matrix(iris[,1:4])
head(iris.m)
tail(iris.m)
class(iris.m)

#p.185(데이터프레임의 열 추출 방법)
iris[,"Species"]
iris[,5]
iris["Species"]
iris[5]
iris$Species





















