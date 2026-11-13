str(longley)
head(longley)

df <- longley[,c('GNP', 'Unemployed', 'Armed.Forces', 'Population', 'Employed')]
df

plot(df)

cor(df)
