install.packages("carData")
library(carData)
str(UN)

col_na <- function(y) {
  return(sum(is.na(y)))
}

apply(UN, 2, FUN = col_na)

mean(UN$lifeExpF, na.rm = T)

tmp <- UN[,c('pctUrban', 'infantMortality')]
tmp <-  tmp[complete.cases(tmp),]
colMeans(tmp)

tmp <- subset(UN, region == 'Asia')
mean(tmp$fertility, na.rm = T)

