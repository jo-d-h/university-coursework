#p.135 팩터
#팩터는 한자리 자료형만 저장 가능능
bt <- c('A', 'B', 'B', 'O', 'AB', 'A')
bt.new <- factor(bt) #문자를 기준으로 요소를 구분함(factor)
bt.new #같은 요소는 묶어서 표출(A, AB, B, O 총 4개 레벨로 구성)

bt[5] #AB
bt.new[5] #AB
levels(bt.new)
levels(bt) #팩터가 아니기에 null

bt.new[7] <- 'B' #가능!(4개 레벨 안에 B가 속하기에)
bt.new[8] <- 'C' #오류!(4개 레벨 안에 속하지 X, 안됨)
bt.new #따라서 8번지에는 NA 생성
bt[7] <-'B' #벡터에는 가능하나, 팩터에선 안됨
bt[8] <- 'C'
bt

성별 <-  c('F', 'M', 'F', 'F', 'M')
성별.new <- factor(성별)
성별.new
성별.new[7] <- 'F'
성별.new[8] <- 'M'
성별.new[9] <- '여성'
성별.new #앞과 동일하게 NA 생성

#p.138 리스트
#자료형이 다른 값들을 한곳에 저장하고 다룰 수 있게 함(여러 형태의 자료형 저장)
#(팩터에선 자료형 하나만 됨, 다른 값 넣으려면 리스트 사용)

#이름, 나이, 학생?이외?, 취미
#문자, 숫자, 논리      , 문자열(벡터)

hobby <- c('볼링', '모바일게임', '여행')
person <- list(name = 'Hong', age = 25, stu = TRUE, hobbys = hobby)
person[1] #배열과 비슷한듯?
person[4]

person2 <- list(name = 'Cho', age = 23, stu = TRUE, hobbys = hobby)
person2

