#p.394 결측값
z <- c(1,2,3,NA,5,NA,6,7,8)
z
sum(z) #결측값이 있기에 정상 계산이 안 됨
is.na(z) #NA 여부 확인
sum(is.na(z))
sum(z, na.rm = T) #NA 값 제외하고 합계 계산, NA ReaMove(RM) = True
z1 <- z
z1[is.na(z1)] <- 0 #NA 값인 요소를 0으로 대체(치환) <- 안 좋은 방법
sum(z1)

z2 <- as.vector(na.omit(z)) #NA 값 제거한 z3 벡터 생성
#as.vector <- 괄호 안에 있는 대상을 벡터로 변환
#na.omit()는 괄호 안 자료구조의 데이터 중 na 값을 제거하겠다는 의미

mean(z)
mean(z1) #평균에 오류!
mean(z2) #이게 맞음!

#p.397 매트릭스 + 데이터프레임
x <- iris #iris는 데이터프레임
x[1, 2] <- NA; x[1, 3] <- NA
x[2, 3] <- NA; x[3, 4] <- NA
head(x) #첫 6행 값 미리보기

#열별 결측값 개수 확인
#1) 반복문 사용
for(i in 1:ncol(x)) {
  this.na <- is.na(x[,i])
  cat(colnames(x)[i], '\t', sum(this.na), '\n')
}

#행 별 결측값 개수
rowSums(is.na(x)) #행별 NA 개수
sum(rowSums(is.na(x)) > 0) #NA가 포함된 행의 개수
sum(is.na(x)) #데이터 전체의 NA 개수

#p.402 sort 정렬
v1 <- c(4, 88, 4, 1, 4, 12, 378, 64, 77, 12489)
v1
v2 <- sort(v1) #오름차순 정렬
#sort(v1) 만 하면 v1에 저장 안됨!!
v3 <- sort(v1, decreasing = T) #내림차순 정렬(decreasing 옵션 사용!)

name1 <- c('강남', '홍길동', '곽준빈', '박보검')
name1
sort(name1)

name2 <- c('Lee', 'YEE', '강감찬', '辛', '페이커', '4')
sort(name2) #숫자(형식은 문자) -> 영문 -> 한글 순
#한글과 한자는 한자 음과 비교함!(강감찬 -> 신(한자) -> 페이커)
sort(name2, decreasing = T)

v4 <- c(3, 6, 9, 21, 4, 5, 1)
sort(v4)
order(v4) #정렬 전 값의 위치를 알려줌

#p.404 매트릭스 + 데이터프레임 정렬
head(iris)
order(iris$Sepal.Length) #순서 정렬
sort(iris$Sepal.Length) #얘는 의미 없음..?

iris[order(iris$Species, decreasing = T, iris$Petal.Length), ] #맨 뒤에 ',' 붙여야 함!
#첫 번째 종의 기준으로 내림차순 정렬 1번
#정렬된 동일한 종을 기준으로 Petal.Length 내림차순 정렬 2번

#요약! -> Species는 내림차순으로 정렬하나, Petal.Length에 대해서는 
#오름차순으로 정렬하여 값 오류남


