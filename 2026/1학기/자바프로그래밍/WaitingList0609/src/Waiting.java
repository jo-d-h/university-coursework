import java.time.LocalDateTime;

/*
  입장/취소 때 리스트에서 지우지 말고 'status'만 변경해주세요.
 */

// 대기, 호출, 입장, 취소, 노쇼 상태
enum Status { WAIT, CALL, IN, CANCEL, NO_SHOW }

class Waiting {
    static int seq = 1;

    int waitingNum;          // 번호는 숫자, 누적 증가
    int memberId;            // 회원 고유 번호
    String name;             // 이름
    String phone;            // 연락처
    int totalCount;          // 인원
    Status status;           // 상태
    LocalDateTime createdAt;
    LocalDateTime calledAt;  // 호출 시간

    public Waiting(int memberId, String name, String phone, int totalCount) {
        this.waitingNum = seq++;
        this.memberId = memberId;
        this.name = name;
        this.phone = phone;
        this.totalCount = totalCount;
        this.status = Status.WAIT;
        this.createdAt = LocalDateTime.now();

    }

    // DB에서 조회한 고객 데이터를 Waiting 객체로 만들 때 사용하는 생성자
    public Waiting(int waitingNum, int memberId, String name, String phone, int totalCount, Status status, LocalDateTime createdAt, LocalDateTime calledAt) {
        this.waitingNum = waitingNum;
        this.memberId = memberId;
        this.name = name;
        this.phone = phone;
        this.totalCount = totalCount;
        this.status = status;
        this.createdAt = createdAt;
        this.calledAt = calledAt;
    }

    @Override
    public String toString() {
        return String.format("[%03d번] %s(%s) | %d명 | 상태 : %s | 등록시간 : %s",
                waitingNum, name, phone, totalCount, status, createdAt.toLocalTime().withNano(0));
    }
}
