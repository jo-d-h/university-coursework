#p.282 막대그래프 작성 기초

f <- c('WINTER', 'SUMMER', 'SPRING', 'FALL', 'FALL', 'SUMMER', 'SPRING', 'SPRING')
f

fg <- table(f) #도수분포
fg
barplot(fg, main='favorite season') #!막대그래프
barplot(fg, main='favorite season', col='slategray3') #막대 색 지정

colors() #이름으로 쓸 수 있는 색상 확인가능

#막대별로 색깔 다르게 지정
barplot(fg, main='favorite season', col= c('red', 'blue', 'orange', 'yellow'))

#rainbow, heat.colors, terrain.colors, cm.colors 등등 사용가능
barplot(fg, main='favorite season', col = rainbow(10))

#x축, y축에 설명
barplot(fg, main='favorite season',
        col = rainbow(10), xlab = '계절', ylab = '빈도')

#그래프를 수평출력
barplot(fg, main='favorite season',
        col = rainbow(10), horiz=TRUE)

#종합1
barplot(fg, main='favorite season',
        col = rainbow(10), xlab = '계절', ylab = '빈도', 
        horiz=TRUE, names = c('F', 'S', 'SS', 'W'))

#이름 수직배치(las)
barplot(fg, main='favorite season',
        col = rainbow(10), xlab = '계절', ylab = '빈도', las = 2)


#p.293
age.A <- c(13709, 10974, 7979, 5000, 4250)
age.B <- c(17540, 29701, 36209, 33947, 24487)
age.C <- c(991, 2195, 5366, 12980, 19007)

ds <- rbind(age.A, age.B, age.C)
colnames(ds) <- c('1970', '1990', '2010', '2030', '2050')

#중첩그래프
barplot(ds, main='인구 추정', col = c('green', 'blue', 'yellow'))

#각각 막대로 분리(beside)
barplot(ds, main='인구 추정', col = c('green', 'blue', 'yellow'), beside = T)

#범례(legend)
par (mfrow = c(1,1), mar = c(5, 5, 5, 7)) #한번 작성하면 계속 유지!

barplot(ds, main='인구 추정', col = c('green', 'blue', 'yellow'), beside = T,
        legend.text = T, args.legend = list(x = 'topright', bty = 'n', inset = c(-0.25, 0)))

#막대그래프와 히스토그램 차이
#그룹이 명시적으로 구분되지 않으면 히스토그램!

#p.303 히스토그램
dist <- cars[,2] #자동차 제동거리
dist
barplot(dist) #제 동거리를 막대그래프로!
hist(dist) #히스토그램은 구간별(0~10, 10~20, ...)로 나눠서 수치 표시!

hist(dist, #data
     main = 'Histogram for 제동거리', #제목
     xlab = '제동거리', #x축 레이블
     ylab = '빈도수', #y축 레이블
     border = 'blue',#막대 테두리
     col = 'green', #막대 색
     las = 2, #x축 글씨 방향
     breaks = 5) #막대 개수 조절

#p.310 다중 그래프
par(mfrow=c(2, 2), mar=c(3, 3, 4, 2)) #화면 분할(2 X 2)

hist(iris$Sepal.Length,
     main = 'Sepal.Length',
     col = 'orange')

barplot(table(mtcars$cyl),
        main='mtcars',
        col=c('red', ' green', 'blue'))

barplot(table(mtcars$gear),
        main = 'mtcars',
        col = rainbow(3),
        horiz = T)

pie(table(mtcars$cyl),
    main = 'mtcars',
    col = topo.colors(3),
    radius = 2)

par(mfrow=c(1, 1), mar = c(5, 4, 4, 2) + .1) #화면 분할취소
