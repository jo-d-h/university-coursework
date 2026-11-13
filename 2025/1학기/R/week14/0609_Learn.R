#p.366 단일변수 데이터 분석

install.packages("carData")
library(carData)

data("Titanic")

titanic <- Titanicsurvival

room.class <- TitanicSurvival$passengerClass
room.class

room_tbl <- table(room.class)
room_tbl
sum(room_tbl)

barplot(room_tbl, main = "선실별 탑승객"
        , xlab = "선실 등급"
        , ylab = "탑승객 수"
        , col = c('blue', 'yellow', 'green'))

room_tbl / sum(room_tbl)
pie(room_tbl, main = "선실별 탑승객"
    , col = c('blue', 'yellow', 'green'))

#p.369
grad <- state.x77[,'HS Grad']
grad

summary(grad)
var(grad) #분산
sd(grad) # 표준편차
#히스토그램
hist(grad, main = '주별 졸업률'
     , xlab = '졸업률'
     , ylab = '주의 개수'
     , col = 'orange')

#상자그림
boxplot(grad, main = '주별 졸업률'
        , col = 'orange')

#시험! 졸업률이 가장 낮은 주
idx <- which(grad == min(grad))
grad[idx]

#졸업률이 가장 높은 주
idx <- which(grad == max(grad))
grad[idx]

#졸업률이 평균 이하인 주
idx <- which(grad < mean(grad))
grad[idx]

#p.377 Lab(DOOR 과제), 자동차 가격 및 성능 데이터 분석
install.packages("DAAG")
library(DAAG)
str(carprice)
carprice$Price #자동차 가격

range(carprice$Price) #값의 범위
mean(carprice$Price) #값의 평균

hist(carprice$Price, main = '자동차 가격',
     xlab = '가격대',
     ylab = '빈도',
     col = 'green')

tbl <- table(carprice$Type)

barplot(tbl, main = '자동차 타입별 빈도수',
        xlab = '타입',
        ylab = '빈도수',
        col = rainbow(tbl))

idx <- which(carprice$Price == max(carprice$Price))
carprice[idx, c('Type', 'Price')]

#p.379 다중변수 데이터
x <- pressure
x

plot(x)
plot(x$temperature, x$pressure, main = "온도와 기압",
     xlab = "온도(화씨)", ylab = "기압")

#p.381 상관관계
cars
y <- cars
str(y)
plot(y, main = "자동차 속도와 제동거리", 
     xlab = "속도",
     ylab = "제동거리")

cor(y)
cor(y$speed, y$dist)

#시험! 매트릭스 -> 데이터프레임
st <- data.frame(state.x77)
str(state.x77) #매트릭스
str(st) #데이터프레임

plot(st)
