def f_8_4():
    class Rectangle:
        def __init__(self, x, y, w, h):
            self.x = x
            self.y = y
            self.width = w
            self.height = h

        def __str__(self):
            self.str_info = '(' + str(self.x) + ',' + str(self.y) + ',' + str(self.width) + ',' + str(self.height) + ')'
            return self.str_info

        def setX(self, x):
            self.x = x

        def setY(self, y):
            self.y = y

        def setWidth(self, w):
            self.width = w

        def setHeight(self, h):
            self.height = h

        def getX(self):
            return self.x

        def getY(self):
            return self.y

        def getWidth(self):
            return self.width

        def getHeight(self):
            return self.height

        def getArea(self):
            return self.width * self.height

        def overlap(self, r):
            self.right_x = self.x + self.width
            self.right_y = self.y + self.height
            r.right_x = r.x + r.width
            r.right_y = r.y + r.height

            if self.x > r.right_x or r.x > self.right_x or self.y > r.right_y or r.y > self.right_y:
                print('r1과 r2는 서로 겹치지 않습니다.')
                return False
            else:
                print('r1과 r2는 서로 겹칩니다.')
                return True

    r1 = Rectangle(0, 0, 100, 100)
    r2 = Rectangle(10, 10, 100, 100)
    r1.overlap(r2)

class Person:
    def __init__(self, name, mobile=None, office=None, email=None):
        self.name = name
        self.mobile = mobile
        self.office = office
        self.email = email

    def __str__(self):
        s = self.name + '\n'
        s += 'office phone: ' + str(self.office) + '\n'
        s += 'email address: ' + str(self.email)
        return s

    def setName(self, name):
        self.name = name
    def setMobile(self, mobile):
        self.mobile = mobile
    def setOffice(self, office):
        self.office = office
    def setEmail(self, email):
        self.email = email
    def getName(self):
        return self.name
    def getMobile(self):
        return self.mobile
    def getOffice(self):
        return self.office
    def getEmail(self):
        return self.email

class PhoneBook:
    def __init__(self):
        self.contacts = {}
    def add(self, name, mobile=None, office=None, email=None):
        p = Person(name, mobile, office, email)
        self.contacts[name] = p
    def __str__(self):
        s = ''
        for name in sorted(self.contacts):
            s += str(self.contacts[name]) + '\n\n'
        return s

def f_8_6():
    p1 = Person("Kim", office="1234567", email="kim@company.com")
    p2 = Person("Park", office="2345678")
    p2.setEmail("park@company.com")
    
def f_8_7():
    obj = PhoneBook()
    obj.add("Kim", office="1234567", email="kim@company.com")
    obj.add("Park", office="2345678", email="park@company.com")
    print(obj)

f_8_4()
f_8_6()
f_8_7()

