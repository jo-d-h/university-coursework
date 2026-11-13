#8_2 PPT p.7 / 책 p. 394 백터 객체에 특수 메소드 정의하기

class Vector2D:
    def __init__(self, x, y):
        self.x = x;
        self.y = y;
        
    def __add__(self, other):
        return Vector2D(self.x + other.x, self.y + other.y)
    
    def __sub__(self, other):
        return Vector2D(self.x - other.x, self.y - other.y)
    
    def __eq__(self, other):
        return self.x == other.x and self.y == other.y

    def __str__(self):
        return '(%g, %g)' % (self.x, self.y)
    
u = Vector2D(0, 1)
v = Vector2D(1, 0)
w = Vector2D(1, 1)
a = u + v
print(u, "+", v, "=", a)

#12_2 책 p.562 은행의 이율 계산하기
class Bank():
    def getInterestRate(self):
        return 0.0
    
class BadBank(Bank):
    def getInterestRate(self):
        return 10.0;
    
class NormalBank(Bank):
    def getInterestRate(self):
        return 5.0;
    
class GoodBank(Bank):
    def getInterestRate(self):
        return 3.0;
    
b1 = BadBank()
b2 = NormalBank()
b3 = GoodBank()
print("BadBank의 이자율: " + str(b1.getInterestRate()))
print("NormaLBank의 이자율: " + str(b2.getInterestRate()))
print(("GoodBank의 이자율: " + str(b3.getInterestRate())))

