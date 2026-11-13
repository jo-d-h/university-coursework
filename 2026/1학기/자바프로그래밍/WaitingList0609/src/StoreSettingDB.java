import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
// Store_Setting 테이블 설정값 조회하는 클래스, 단일 가게이므로 id = 1 값만 조회함

class StoreSettingDB {
    public String getWaitingStatus() throws SQLException { // 현재 웨이팅 접수 상태 조회
        String sql = "SELECT waiting_status FROM store_setting WHERE id = 1";
        return selectString(sql);
    }

    public int getMaxWaitingCount() throws SQLException { // 최대 대기 수 조회
        String sql = "SELECT max_waiting_count FROM store_setting WHERE id = 1";
        return selectInt(sql);
    }

    public int getNoShowLimitMinutes() throws SQLException { // 노쇼 제한시간 조회
        String sql = "SELECT no_show_limit_minutes FROM store_setting WHERE id = 1";
        return selectInt(sql);
    }

    private String selectString(String sql) throws SQLException { // 문자열(접수 상황) 설정 값 조회
        try ( // DB 연결, SQL 실행, 결과 저장, 반환
                Connection conn = DBConnection.getConnection();
                PreparedStatement preparedStatement = conn.prepareStatement(sql);
                ResultSet rs = preparedStatement.executeQuery()
        ) {
            if (rs.next()) {
                return rs.getString(1);
            }
        }

        throw new SQLException("store_setting 설정 값을 찾을 수 없습니다.");
    }

    private int selectInt(String sql) throws SQLException { //숫자(최대 대기 수, 노쇼 제한시간) 설정 값 조회
        try ( // DB 연결, SQL 실행, 결과 저장, 반환
                Connection conn = DBConnection.getConnection();
                PreparedStatement preparedStatement = conn.prepareStatement(sql);
                ResultSet rs = preparedStatement.executeQuery()
        ) {
            if (rs.next()) {
                return rs.getInt(1);
            }
        }

        throw new SQLException("store_setting 설정 값을 찾을 수 없습니다.");
    }

    public boolean updateWaitingStatus(String newStatus) throws SQLException {
        String sql = "UPDATE store_setting SET waiting_status = ?, updated_at = NOW() WHERE id = 1";
        try (
            Connection conn = DBConnection.getConnection();
            PreparedStatement ps = conn.prepareStatement(sql)
        ) {
            ps.setString(1, newStatus);
            return ps.executeUpdate() > 0;
        }
    }
}
