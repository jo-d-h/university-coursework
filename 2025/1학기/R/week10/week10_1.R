install.packages('svDialogs')
ui <- dlgInput('Input income')$res #팝업을 통해 입력받고, ui에 저장
ui

income <- as.numeric(ui) #문자열을 숫자로
income

tax <- income * 0.05 #세금
tax

cat('세금 :', tax)

res <- sqrt(200)
res
res2 <- sqrt(10000)
res2
print(res) #하나의 값 출력, 자동 줄바꿈 O
cat(res, "\n", res2) #여러개의 값 출력, 자동 줄바꿈 X

getwd() #현재 작업 폴더 알아내기

air2 <- read.csv("air.csv", header = T)
air <- read.csv('airquality.csv', header = T)
air <- airquality
write.csv(air, "air.csv", row.names = F) #csv 파일로 저장

print(air)
print(air2)


iris
i <- iris
write.csv(i, "iris.csv", row.names = F) #csv 파일로 저장

install.packages("xlsx")

write.xlsx(air, 'air.xlsx', row.names = F)

air3 <- read.xlsx('air.xlsx', header = T, sheetIndex = 1)
air3

#!시험!
print('작업시작')
a <- 10; b <- 20
sink("res2.txt", append = T) #출력은 res.txt 파일에서 실행됨!
cat("a + b = ", a+b, "\n") #얘는 res.txt에서
print('작업종료')
sink() #출력 정지
cat('hello world \n') #얘는 바로 출력
sink('result.txt', append = T)
cat('a * b = ', a*b, '\n') #얘는 res.txt에서
sink()
print('작업종료') #얘는 바로 출력
