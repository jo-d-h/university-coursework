import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.sql.SQLException;

// 회원가입, 로그인에 필요한 회원 기능 로직 담당
class MemberService {
    private final MemberDB memberDB = new MemberDB();

    // 회원 처리 결과를 UI로 전달하는 내부 클래스
    static class MemberResult {
        private final boolean success;  // 처리 성공 여부
        private final String message;   // 결과 메세지
        private final Member member;    // 로그인, 회원가입 성공 시 회원 정보

        MemberResult(boolean success, String message, Member member) {
            this.success = success;
            this.message = message;
            this.member = member;
        }

        public boolean isSuccess() {
            return success;
        }

        public String getMessage() {
            return message;
        }

        public Member getMember() {
            return member;
        }
    }

    // 회원가입 처리: 중복 확인, 비밀번호 암호화해서 저장
    public MemberResult signUp(String name, String phone, String loginId, String password, boolean isAdmin) {
        if (isBlank(name) || isBlank(phone) || isBlank(loginId) || isBlank(password)) {
            return fail("이름, 전화번호, 아이디, 비밀번호를 모두 입력해주세요.");
        }

        try {
            // 로그인용 아이디 중복 확인
            if (memberDB.existsByLoginId(loginId)) {
                return fail("이미 사용 중인 아이디입니다.");
            }

            // 전화번호 중복 확인
            if (memberDB.existsByPhone(phone)) {
                return fail("이미 가입된 전화번호입니다.");
            }

            // 비밀번호 저장 시 SHA-256으로 암호화 해서 저장함
            String encryptedPassword = encryptPassword(password);
            int memberId = memberDB.insertMember(name, phone, loginId, encryptedPassword, isAdmin);
            Member member = memberDB.findByMemberId(memberId); // 저장된 회원 정보를 다시 조회해 반환

            return success("회원가입이 완료되었습니다.", member);
        } catch (SQLException e) {
            return fail("회원가입 중 DB 오류가 발생했습니다: " + e.getMessage());
        }
    }

    // 로그인 처리: 입력 비밀번호를 암호화 후 DB의 암호화 비밀번호와 비교
    public MemberResult login(String loginId, String password) {
        if (isBlank(loginId) || isBlank(password)) {
            return fail("아이디와 비밀번호를 모두 입력해주세요.");
        }

        try {
            // 로그인 아이디로 회원 조회
            Member member = memberDB.findByLoginId(loginId);

            if (member == null) {
                return fail("존재하지 않는 아이디입니다.");
            }

            // 입력한 비밀번호를 암호화 후 저장된 비밀번호와 비교
            String encryptedPassword = encryptPassword(password);
            if (!member.password.equals(encryptedPassword)) {
                return fail("비밀번호가 일치하지 않습니다.");
            }

            return success("로그인 성공", member);
        } catch (SQLException e) {
            return fail("로그인 중 DB 오류가 발생했습니다: " + e.getMessage());
        }
    }

    // SHA-256으로 비밀번호 암호화
    private String encryptPassword(String password) {
        try {
            MessageDigest digest = MessageDigest.getInstance("SHA-256");
            byte[] hash = digest.digest(password.getBytes(StandardCharsets.UTF_8));
            StringBuilder sb = new StringBuilder();

            // byte 배열을 16진수 문자열로 변환
            for (byte b : hash) {
                sb.append(String.format("%02x", b));
            }

            return sb.toString();
        } catch (NoSuchAlgorithmException e) {
            throw new IllegalStateException("SHA-256 암호화 알고리즘을 사용할 수 없습니다.", e);
        }
    }

    // null 또는 공백만 있는 문자열인지 확인
    private boolean isBlank(String value) {
        return value == null || value.trim().isEmpty();
    }

    // 성공 결과 생성
    private MemberResult success(String message, Member member) {
        return new MemberResult(true, message, member);
    }

    // 실패 결과 생성
    private MemberResult fail(String message) {
        return new MemberResult(false, message, null);
    }
}
