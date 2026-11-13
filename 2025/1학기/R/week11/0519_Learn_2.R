#p.251 apply 함수
#5열은 문자형이라 제외, 숫자형만 수행 가능!
apply(iris[,1:4], 1, mean) #1(행 방향) - 150개의 평균
apply(iris[,1:4], 2, mean) #2(열 방향) - 4개의 평균

#p.253 LAB 자격증 합격 여부 판단하기 I
sub1 <- c(14, 16, 12, 20, 8, 6, 12, 18, 16, 10)
sub2 <- c(18, 14, 14, 16, 10, 12, 10, 20, 14, 14)
sub3 <- c(44, 38, 30, 48, 42, 50, 36, 52, 54, 32)
score <- data.frame(sub1, sub2, sub3)
total <- apply(score, 1, sum)
scoreset <- cbind(score, total)
result <- c()

for(i in 1:nrow(scoreset)) {
  if (scoreset[i, 1] < 20 * 0.4 | scoreset[i, 2] < 20 * 0.4 | scoreset[i, 3] < 60 * 0.4) {
    result[i] <- '불합격'
  }
  else if (scoreset[i, 4] >= 60) {
    result[i] <- '합격'
  }
  else {
    result[i] <-  '불합격'
  }
  
  cat(i, '번째 응시생은', result[i], '입니다\n')
}

#p.256 사용자 정의 함수
mymax <- function(x, y) {
  num.max <- x
  if (y > x) {
    num.max <- y
  }
  return (num.max)
}

mymax(10, 15)
a <- mymax(20, 15)
b <- mymax(31, 45)
print (a + b)
print(mymax(10, 100))

#활용) 삼각형 넓이 구하기
area <- function(b, h) {
  cat("밑변 :", b, "\n높이 :", h, "\n넓이 :", b * h / 2, "\n")
}

area(5, 20)

#활용) 출력 함수 간단히
p <- function(x) {
  print(x)
}

p("안녕하세요")

#활용) 합계 합수 간단히
sum <- 0
for (i in 1:100) {
  sum <- sum + i
}

p(sum)

#활용) 벡터의 합 함수 - 함수 이름은 겹처도 상관 X
vectorsum <- function(x) {
  sum <- 0
  
  for (i in 1:x) {
    sum <- sum + i
  }
  
  p(sum)
}

vectorsum(1000)


