#25.09.10 2주차 실습
#https://wikidocs.net/7022

#021 문자열 인덱싱
letters = 'python'
print(letters[0], letters[2])

#023 문자열 인덱싱
string = "홀짝홀짝홀짝"
print(string[::2])

#025 문자열 치환
phone_number = "010-1111-2222"
phone_number1 = phone_number.replace("-", " ")
print(phone_number1)

#026 문자열 다루기
phone_number = "010-1111-2222"
phone_number1 = phone_number.replace("-", "")
print(phone_number1)

#028 문자열은 immutable
#오류 발생! (문자열은 수정 불가능)

#029 replace 메서드
string = 'abcdfe2a354a32a'
string = string.replace('a','A')
print(string)

#030 replace 매서드
#그래로 유지. 저장을 안해서!

#031 문자열 합치기
#34

#032 문자열 곱하기
#HiHiHi

#033 문자열 곱하기
print("-" * 80)

#034 문자열 곱하기
t1 = 'python'
t2 = 'java'
t3 = t1 + " " + t2 + " "
print(t3 * 4)

#037 문자열 출력
name1 = "김민수" 
age1 = 10
name2 = "이철희"
age2 = 13
print(f"이름: {name1} 나이: {age1}")
print(f"이름: {name2} 나이: {age2}")

#038 컴마 제거하기
상장주식수 = "5,969,782,550"
컴마제거 = 상장주식수.replace(",", "")
타입변환 = int(컴마제거)
print(타입변환)

#039 문자열 슬라이싱
분기 = "2020/03(E) (IFRS연결)"
print(분기[:7])

#041 upper 메서드
ticker = "btc_krw"
upper_ticker = ticker.upper()
print(upper_ticker)

#042 lower 메서드
ticker = "BTC_KRW"
lower_ticker = ticker.lower()
print(lower_ticker)



