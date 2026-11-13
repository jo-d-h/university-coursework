import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

// member 테이블의 DB 작업 담당(회원가입, 조회, 중복확인용)
class MemberDB {
    // 회원 정보를 DB에 저장 후 생성된 회원번호를 반환
    public int insertMember(String name, String phone, String loginId, String password, boolean isAdmin) throws SQLException {
        String sql = "INSERT INTO member (name, phone, login_id, password, is_admin) "
                + "VALUES (?, ?, ?, ?, ?)";

        try (
                Connection conn = DBConnection.getConnection();
                PreparedStatement preparedStatement = conn.prepareStatement(sql, Statement.RETURN_GENERATED_KEYS)
        ) {
            preparedStatement.setString(1, name);
            preparedStatement.setString(2, phone);
            preparedStatement.setString(3, loginId);
            preparedStatement.setString(4, password);
            preparedStatement.setBoolean(5, isAdmin);
            preparedStatement.executeUpdate();

            try (ResultSet rs = preparedStatement.getGeneratedKeys()) {
                if (rs.next()) {
                    return rs.getInt(1);
                }
            }
        }

        throw new SQLException("회원 번호 생성에 실패했습니다.");
    }

    // 로그인 아이디로 회원 정보 조회
    public Member findByLoginId(String loginId) throws SQLException {
        String sql = "SELECT member_id, name, phone, login_id, password, is_admin, created_at "
                + "FROM member "
                + "WHERE login_id = ?";

        try (
                Connection conn = DBConnection.getConnection();
                PreparedStatement preparedStatement = conn.prepareStatement(sql)
        ) {
            preparedStatement.setString(1, loginId);

            try (ResultSet rs = preparedStatement.executeQuery()) {
                return rs.next() ? toMember(rs) : null;
            }
        }
    }

    // 회원 고유번호로 회원 정보 조회
    public Member findByMemberId(int memberId) throws SQLException {
        String sql = "SELECT member_id, name, phone, login_id, password, is_admin, created_at "
                + "FROM member "
                + "WHERE member_id = ?";

        try (
                Connection conn = DBConnection.getConnection();
                PreparedStatement preparedStatement = conn.prepareStatement(sql)
        ) {
            preparedStatement.setInt(1, memberId);

            try (ResultSet rs = preparedStatement.executeQuery()) {
                return rs.next() ? toMember(rs) : null;
            }
        }
    }

    // 이미 사용중인 로그인 아이디인지 확인
    public boolean existsByLoginId(String loginId) throws SQLException {
        String sql = "SELECT COUNT(*) FROM member WHERE login_id = ?";
        return exists(sql, loginId);
    }

    // 이미 사용중인 휴대폰 번호인지 확인
    public boolean existsByPhone(String phone) throws SQLException {
        String sql = "SELECT COUNT(*) FROM member WHERE phone = ?";
        return exists(sql, phone);
    }

    // 중복 확인용 공통 메서드
    private boolean exists(String sql, String value) throws SQLException {
        try (
                Connection conn = DBConnection.getConnection();
                PreparedStatement preparedStatement = conn.prepareStatement(sql)
        ) {
            preparedStatement.setString(1, value);

            try (ResultSet rs = preparedStatement.executeQuery()) {
                return rs.next() && rs.getInt(1) > 0;
            }
        }
    }

    // 결과를 member 객체로 변환
    private Member toMember(ResultSet rs) throws SQLException {
        return new Member(
                rs.getInt("member_id"),
                rs.getString("name"),
                rs.getString("phone"),
                rs.getString("login_id"),
                rs.getString("password"),
                rs.getBoolean("is_admin"),
                rs.getTimestamp("created_at").toLocalDateTime()
        );
    }
}
