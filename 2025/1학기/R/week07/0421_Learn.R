iris[,c(1:4)] #1~4열

x <- iris
x[,c(1:2)] #1~2열
x[1:5, ] #1~5행

iris[1:5, c(1, 3)] #1~5행, 1, 3열

#p.170
kcal <- c(514, 533, 566)
na <- c(917,853, 888)
fat <- c(11, 13, 10)
menu <- c('새우', '불고기', '치킨')
burger <- data.frame(kcal, na, fat, menu)
burger
rownames(burger) <- c('M', 'L', 'B')

burger['M', 'na']
burger['M', ]
burger[, 'kcal']
burger[c('M', 'B'), 'menu']

#p.172 매트릭스 / 데이터프레임 다루기
x <- iris
dim(iris) #행/열 개수
nrow(iris) #행 개수
ncol(iris) #열 개수
colnames(iris) #행 이름
rownames(iris) #열 이름
head(iris) #맨 앞 6개
tail(iris) #맨 뒤 6개

#p.174
str(iris) #iris의 데이터 정보 보기
#150행, 5열 / 각 행의 이름과 내용 보기

iris[,5] #5열 데이터 보기 (5열 : 종)

levels(iris[,5]) #5열 종류 보기(내용 말고)

table(iris[,5]) #iris 5열의 개수 세기

#p.176
colSums(iris[,-5]) #5열(종) 제외 합계(-5열은 종, 문자형이라 제외)
colMeans(iris[,-5]) #행별 평균(동일)
rowSums(iris[,-5]) #열별 합계(굳이?)
rowMeans(iris[,-5]) #열별 평균

#p.177
z <- matrix(1:20, nrow = 4, ncol = 5)
z
t(z)

#p.178
IR.1 <- subset(iris, Species== 'Setosa') #subset(추출대상, 종이 세토사인거만)
IR.1
IR.2 <- subset(iris, Sepal.Length > 5.0 & Sepal.Width > 4.0)
IR.2
IR.2[, c(2, 4)] #IR2에서 2, 4열 값만!