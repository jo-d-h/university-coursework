import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.LinkedHashMap;
import java.util.Map;

public class StatisticsDB {
    public int getActiveWaitingCount() throws SQLException {
        // 대기중이거나 호출된 팀 수 카운트
        String sql = "SELECT COUNT(*) FROM waiting WHERE status IN ('WAIT', 'CALL')";

        try (
                Connection conn = DBConnection.getConnection();
                PreparedStatement pstmt = conn.prepareStatement(sql);
                ResultSet rs = pstmt.executeQuery()
        ) {
            if (rs.next()) {
                return rs.getInt(1);
            }
            return 0;
        }
    }

    // 평균 대기시간
    public double getAverageWaitingMinutes() throws SQLException {
        String sql = "SELECT AVG(TIMESTAMPDIFF(MINUTE, created_at, entered_at)) FROM waiting WHERE status = 'IN'";

        try (
                Connection conn = DBConnection.getConnection();
                PreparedStatement pstmt = conn.prepareStatement(sql);
                ResultSet rs = pstmt.executeQuery()
        ) {
            if (rs.next()) {
                return rs.getDouble(1);
            }
            return 0.0;
        }
    }

    // 오늘 방문
    public int getTodayTotalWaiting() throws SQLException {
        String sql = "SELECT COUNT(*) FROM waiting WHERE DATE(created_at) = CURDATE()";

        try (
                Connection conn = DBConnection.getConnection();
                PreparedStatement pstmt = conn.prepareStatement(sql);
                ResultSet rs = pstmt.executeQuery()
        ) {
            if (rs.next()) {
                return rs.getInt(1);
            }
            return 0;
        }
    }

    // 노쇼 비율
    public double getNoShowRate() throws SQLException {
        // COUNT(*) 결과 0이면 오류나서 IFNULL로 감싸놨습니다
        String sql = "SELECT IFNULL(COUNT(CASE WHEN status = 'NO_SHOW' THEN 1 END) * 100.0 / COUNT(*), 0.0) FROM waiting";

        try (
                Connection conn = DBConnection.getConnection();
                PreparedStatement pstmt = conn.prepareStatement(sql);
                ResultSet rs = pstmt.executeQuery()
        ) {
            if (rs.next()) {
                return rs.getDouble(1);
            }
            return 0.0;
        }
    }

    // key = 시간, value = 웨이팅 수
    public Map<Integer, Integer> getHourlyWaitingStats() throws SQLException {
        String sql = "SELECT HOUR(created_at), COUNT(*) FROM waiting GROUP BY HOUR(created_at) ORDER BY HOUR(created_at)";
        Map<Integer, Integer> stats = new LinkedHashMap<>();

        try (
                Connection conn = DBConnection.getConnection();
                PreparedStatement pstmt = conn.prepareStatement(sql);
                ResultSet rs = pstmt.executeQuery()
        ) {
            while (rs.next()) {
                int hour = rs.getInt(1);
                int count = rs.getInt(2);
                stats.put(hour, count);
            }
        }
        return stats;
    }

    // 요일별 통계
    public Map<String, Integer> getWeeklyStats() throws SQLException {
        String sql = "SELECT DAYOFWEEK(created_at), DAYNAME(created_at), COUNT(*)\n" +
                "FROM waiting\n" +
                "GROUP BY DAYOFWEEK(created_at), DAYNAME(created_at)\n" +
                "ORDER BY DAYOFWEEK(created_at)";
        Map<String, Integer> stats = new LinkedHashMap<>();

        try (
                Connection conn = DBConnection.getConnection();
                PreparedStatement pstmt = conn.prepareStatement(sql);
                ResultSet rs = pstmt.executeQuery()
        ) {
            while (rs.next()) {
                stats.put(rs.getString(2), rs.getInt(3));
            }
        }
        return stats;
    }

    public int getTotalWaitingCount() throws SQLException {
        String sql = "SELECT COUNT(*) FROM waiting";

        try (
                Connection conn = DBConnection.getConnection();
                PreparedStatement pstmt = conn.prepareStatement(sql);
                ResultSet rs = pstmt.executeQuery()
        ) {
            if (rs.next()) {
                return rs.getInt(1);
            }
            return 0;
        }
    }

    // 입장 완료 수
    public int getEnteredCount() throws SQLException {
        String sql = "SELECT COUNT(*) FROM waiting WHERE status = 'IN'";

        try (
                Connection conn = DBConnection.getConnection();
                PreparedStatement pstmt = conn.prepareStatement(sql);
                ResultSet rs = pstmt.executeQuery()
        ) {
            if (rs.next()) {
                return rs.getInt(1);
            }
            return 0;
        }
    }

    // 취소 수
    public int getCancelCount() throws SQLException {
        String sql = "SELECT COUNT(*) FROM waiting WHERE status = 'CANCEL'";

        try (
                Connection conn = DBConnection.getConnection();
                PreparedStatement pstmt = conn.prepareStatement(sql);
                ResultSet rs = pstmt.executeQuery()
        ) {
            if (rs.next()) {
                return rs.getInt(1);
            }
            return 0;
        }
    }

    // 노쇼 수
    public int getNoShowCount() throws SQLException {
        String sql = "SELECT COUNT(*) FROM waiting WHERE status = 'NO_SHOW'";

        try (
                Connection conn = DBConnection.getConnection();
                PreparedStatement pstmt = conn.prepareStatement(sql);
                ResultSet rs = pstmt.executeQuery()
        ) {
            if (rs.next()) {
                return rs.getInt(1);
            }
            return 0;
        }

    }
}