#p.328 원그래프
# 데이터 정의
f <- c('winter', 'summer', 'spring', 'summer',
       'summer', 'summer', 'fall', 'fall',
       'summer', 'spring', 'spring')

# 빈도 테이블 생성
f_c <- table(f)
f_c

# 기본 pie 차트 (explode 없이)
pie(f_c, 
    main = "선호계절", 
    radius = 1)

# plotrix 설치 및 로드
install.packages('plotrix')  # 이미 설치되어 있다면 생략
library(plotrix)

# 3D pie 차트 (explode 사용 가능)
pie3D(f_c, 
      main = "선호계절", 
      labels = names(f_c),
      labelcex = 1.0,
      explode = 0.1,
      radius = 1.5,
      col = c('brown', 'green', 'red','yellow'))

# 막대 그래프
barplot(f_c, 
        main = "선호계절 막대그래프", 
        col = c('yellow', 'red', 'cyan', 'magenta'))

# 히스토그램은 table형에 적합하지 않음 → 수치화해서 써야 함
hist(as.numeric(f_c), 
     main = "빈도 히스토그램", 
     col = "skyblue")

#p.331 선그래프
month = 1:12 #데이터 입력
late = c(5,8,7,9,4,6,12,13,8,6,6,4) #데이터 입력
plot(month, #x 데이터
     late, #y 데이터
     main = '지각생 통계', #제목 
     type = 'l', #그래프 종류(l = 선, b = 점 + 선, s = ?, o = 점선 + 점)
     lty = 1, #선 종류(실선~점선)
     lwd = 1, #선 굵기
     xlab = 'Month', #x 레이블
     ylab = 'Late cnt') #y 레이블

#p.334
month = 1:12
late1 = c(5,8,7,9,4,6,12,13,8,6,6,4)
late2 = c(4,6,5,8,7,8,10,11,6,5,7,3)
plot(month, #x 데이터
     late1, #y 데이터
     main = 'Late students',
     type = 'b', #그래프 종류(l = 선, b = 점 + 선, s = ?, o = 점선 + 점)
     lty = 1, #선 종류(실선~점선)
     col = 'red',#선 색상
     xlab = 'Month', #x 레이블
     ylab = 'Late cnt') #y 레이블

lines(month, #x데이터
      late2, #y데이터
      type = 'b', #선 종류
      col = 'blue') #선 색상


#p.338 상자그림
dist <- cars[,2] #차 제동거리(데이터프레임 -> 숫자형 벡터로 변환)
dist
str(dist)

boxplot(dist, main = '자동차 제동거리')

#p.340
boxplot.stats(dist)

#p.341 그룹이 있는 데이터의 상자그림
boxplot(Petal.Length~Species, #데이터, 그룹정보
        data=iris, #데이터가 저장된 자료 구조
        main='품종별 꽃잎의 길이', #제목
        col = c('green', 'yellow', 'blue')) #상자 색

boxplot(Petal.Width~Species, #데이터, 그룹정보
        data=iris, #데이터가 저장된 자료 구조
        main='품종별 꽃잎의 넓이', #제목
        col = c('green', 'yellow', 'blue')) #상자 색

boxplot(Sepal.Width~Species, #데이터, 그룹정보
        data=iris, #데이터가 저장된 자료 구조
        main='품종별 꽃받임의 넓이', #제목
        col = c('green', 'yellow', 'blue')) #상자 색

#이러면 데이터 속성 안 넣어도 됨
boxplot(iris$Sepal.Length~iris$Species, #데이터, 그룹정보
        main='품종별 꽃받임의 넓이', #제목
        col = c('green', 'yellow', 'blue')) #상자 색

#p.344 산점도
plot(iris$Sepal.Length, iris$Sepal.Width) #150개의 좌표 나옴.(행)

plot(iris$Sepal.Length, iris$Petal.Length)






