#p.243 for문
x = c(1:10)

#중요!
#얘는 1 2 3 3 (밑 print는 반복문과 관련 X)
for (i in c(1:3))
{
  print(i)
}
print (i)


#얘처럼 중괄호로 묶어서 쓰면 1 1 2 2 3 3이 출력됨!
for (i in c(1:3)){
  print(i)
  print(i)
}

#실행할 때 sum 먼저 변수 만들고(먼저 실행) 이후 반복문 실행해야 오류 X
#재실행 할 때도 sum을 0으로 초기화 하고 실행해야 함
#p.246
sum <- 0

for (i in 1:100)
{
  sum <- sum + i
}
print(sum)

#p.245
for (i in 1:9) {
  cat('2 * ', i, '=', 2 * i, '\n')
}

#p.246 짝수만 출력
for (i in 1:20)
{
  if (i %% 2 == 0)
  {
    cat(i, " ")
  }
}

#p.247 응용
norow <- nrow(iris) #150개
mylabel <- c() #빈 벡터

for(i in 1:norow) {
  if(iris$Petal.Length[i] <= 1.6) {
    mylabel[i] <- 'L'
  }
  else if (iris$Petal.Length[i] >= 5.1) {
    mylabel[i] <- 'H'
  }
  else {
    mylabel[i] <- 'M'
  }
}
print(mylabel) #출력
newds <- data.frame(iris$Petal.Length, mylabel) #데이터프레임 결합
head(newds) #결합한 데이터프레임 첫 6개 출력(head)

#p.249 while문
sum <- 0
i <- 1

while(i <= 100) {
  sum <- sum + i
  i <- i + 1
}
print(sum)
