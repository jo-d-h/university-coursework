import java.time.LocalDateTime;

// Member 테이블의 회원정보 담는 클래스
class Member {
    int memberId;   // 회원 고유번호
    String name;    // 이름
    String phone;   // 전화번호
    String loginId; // 로그인 아이디
    String password;    // 비밀번호(SHA-256 암호화)
    boolean isAdmin;    // 관리자 여부(0: 일반, 1: 관리자)
    LocalDateTime createdAt;    //계정 생성일

    // DB에서 조회한 회원 데이터를 Member 객체로 만들 때 사용하는 생성자
    public Member(int memberId, String name, String phone, String loginId, String password, boolean isAdmin, LocalDateTime createdAt) {
        this.memberId = memberId;
        this.name = name;
        this.phone = phone;
        this.loginId = loginId;
        this.password = password;
        this.isAdmin = isAdmin;
        this.createdAt = createdAt;
    }

    @Override
    public String toString() {
        return memberId + " | " + name + "(" + loginId + ") | " + phone + " | 관리자: " + isAdmin;
    }
}
