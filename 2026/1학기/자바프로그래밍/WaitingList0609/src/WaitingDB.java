import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.List;

// waiting 테이블의 DB 작업 담당(대기 등록, 조회, 상태 변경)
class WaitingDB {
    // 회원 기준으로 대기 정보를 DB에 저장, 생성된 대기번호 반환
    public int insertWaiting(int memberId, String name, String phone, int totalCount) throws SQLException {
        String sql = "INSERT INTO waiting (member_id, name, phone, total_count) "
                + "VALUES (?, ?, ?, ?)";

        try (
                Connection conn = DBConnection.getConnection();
                PreparedStatement preparedStatement = conn.prepareStatement(sql, Statement.RETURN_GENERATED_KEYS)
        ) {
            preparedStatement.setInt(1, memberId);
            preparedStatement.setString(2, name);
            preparedStatement.setString(3, phone);
            preparedStatement.setInt(4, totalCount);
            preparedStatement.executeUpdate();

            try (ResultSet rs = preparedStatement.getGeneratedKeys()) {
                if (rs.next()) {
                    return rs.getInt(1);
                }
            }
        }

        throw new SQLException("대기번호 생성에 실패했습니다.");
    }

    // 현재 대기 중인 목록 조회(WAIT, CALL 상태)
    public List<Waiting> findActiveWaitingList() throws SQLException {
        String sql = "SELECT waiting_num, member_id, name, phone, total_count, status, created_at, called_at "
                + "FROM waiting "
                + "WHERE status IN ('WAIT', 'CALL') "
                + "ORDER BY waiting_num";

        return findWaitingList(sql);
    }

    // 모든 대기 목록 조회
    public List<Waiting> findAllWaitingList() throws SQLException {
        String sql = "SELECT waiting_num, member_id, name, phone, total_count, status, created_at, called_at "
                + "FROM waiting "
                + "ORDER BY waiting_num";

        return findWaitingList(sql);
    }

    // 특정 회원의 대기 목록 조회
    public List<Waiting> findWaitingListByMemberId(int memberId) throws SQLException {
        String sql = "SELECT waiting_num, member_id, name, phone, total_count, status, created_at, called_at "
                + "FROM waiting "
                + "WHERE member_id = ? "
                + "ORDER BY waiting_num";

        try (
                Connection conn = DBConnection.getConnection();
                PreparedStatement preparedStatement = conn.prepareStatement(sql)
        ) {
            preparedStatement.setInt(1, memberId);
            return findWaitingList(preparedStatement);
        }
    }

    // 현재 대기 중인 팀 수 조회(WAIT, CALL 상태)
    public int countActiveWaitingTeams() throws SQLException {
        String sql = "SELECT COUNT(*) "
                + "FROM waiting "
                + "WHERE status IN ('WAIT', 'CALL')";

        try (
                Connection conn = DBConnection.getConnection();
                PreparedStatement preparedStatement = conn.prepareStatement(sql);
                ResultSet rs = preparedStatement.executeQuery()
        ) {
            return rs.next() ? rs.getInt(1) : 0;
        }
    }

    // 같은 회원이 이미 대기 중인지 확인
    public boolean hasActiveWaiting(int memberId) throws SQLException {
        String sql = "SELECT COUNT(*) "
                + "FROM waiting "
                + "WHERE member_id = ? AND status IN ('WAIT', 'CALL')";

        try (
                Connection conn = DBConnection.getConnection();
                PreparedStatement preparedStatement = conn.prepareStatement(sql)
        ) {
            preparedStatement.setInt(1, memberId);

            try (ResultSet rs = preparedStatement.executeQuery()) {
                return rs.next() && rs.getInt(1) > 0;
            }
        }
    }

    // 특정 대기번호보다 앞에 있는 WAIT 대기 팀 수 조회(고객 기능 - 현재 나의 순번)
    public int countActiveWaitingBefore(int waitingNum) throws SQLException {
        // 본인보다 앞에 있는 대기중인 고객만 계산
        String sql = "SELECT COUNT(*) "
                + "FROM waiting "
                + "WHERE status IN ('WAIT', 'CALL') "
                + "AND waiting_num < ?";

        try (
                Connection conn = DBConnection.getConnection();
                PreparedStatement preparedStatement = conn.prepareStatement(sql)
        ) {
            preparedStatement.setInt(1, waitingNum);

            try (ResultSet rs = preparedStatement.executeQuery()) {
                return rs.next() ? rs.getInt(1) : 0;
            }
        }
    }

    // 다음 대기 고객 호출
    public boolean callNextWaiting() throws SQLException {
        String sql = "UPDATE waiting "
                + "SET status = 'CALL', called_at = NOW() "
                + "WHERE status = 'WAIT' "
                + "ORDER BY waiting_num "
                + "LIMIT 1";

        return executeUpdate(sql);
    }

    // 특정 대기 고객 호출
    public boolean callWaiting(int waitingNum) throws SQLException {
        return updateStatus(waitingNum, "CALL", "called_at");
    }

    // 특정 대기 고객 입장 처리
    public boolean enterWaiting(int waitingNum) throws SQLException {
        return updateStatus(waitingNum, "IN", "entered_at");
    }

    // 특정 대기 고객 취소 처리
    public boolean cancelWaiting(int waitingNum) throws SQLException {
        return updateStatus(waitingNum, "CANCEL", "cancelled_at");
    }

    // 특정 회원의 본인 대기만 취소 처리(고객 기능 - 대기 취소)
    public boolean cancelWaitingByMemberId(int waitingNum, int memberId) throws SQLException {
        String sql = "UPDATE waiting "
                + "SET status = 'CANCEL', cancelled_at = NOW() "
                + "WHERE waiting_num = ? " // 대기번호와
                + "AND member_id = ? " // 고객 id가 맞을 때,
                + "AND status IN ('WAIT', 'CALL')"; // 호출, 대기 상태가 모두 맞아야만 취소 가능

        try (
                Connection conn = DBConnection.getConnection();
                PreparedStatement preparedStatement = conn.prepareStatement(sql)
        ) {
            preparedStatement.setInt(1, waitingNum);
            preparedStatement.setInt(2, memberId);
            return preparedStatement.executeUpdate() > 0;
        }
    }

    // 특정 대기 고객 노쇼 처리
    public boolean noShowWaiting(int waitingNum) throws SQLException {
        return updateStatus(waitingNum, "NO_SHOW", "no_show_at");
    }

    // SELECT 결과를 Waiting 리스트로 변환
    private List<Waiting> findWaitingList(String sql) throws SQLException {
        try (
                Connection conn = DBConnection.getConnection();
                PreparedStatement preparedStatement = conn.prepareStatement(sql)
        ) {
            return findWaitingList(preparedStatement);
        }
    }

    // PreparedStatement 실행 결과를 Waiting 리스트로 변환
    private List<Waiting> findWaitingList(PreparedStatement preparedStatement) throws SQLException {
        List<Waiting> waitingList = new ArrayList<>();

        try (ResultSet rs = preparedStatement.executeQuery()) {
            while (rs.next()) {
                java.sql.Timestamp calledAtTs = rs.getTimestamp("called_at");
                LocalDateTime calledAt = (calledAtTs != null) ? calledAtTs.toLocalDateTime() : null;

                Waiting waiting = new Waiting(
                        rs.getInt("waiting_num"),
                        rs.getInt("member_id"),
                        rs.getString("name"),
                        rs.getString("phone"),
                        rs.getInt("total_count"),
                        Status.valueOf(rs.getString("status")),
                        rs.getTimestamp("created_at").toLocalDateTime(),
                        calledAt
                );

                waitingList.add(waiting);
            }
        }

        return waitingList;
    }

    // 대기 상태와 처리 시간을 변경
    private boolean updateStatus(int waitingNum, String status, String timeColumn) throws SQLException {
        String sql = "UPDATE waiting SET status = ?, " + timeColumn + " = NOW() WHERE waiting_num = ?";

        try (
                Connection conn = DBConnection.getConnection();
                PreparedStatement preparedStatement = conn.prepareStatement(sql)
        ) {
            preparedStatement.setString(1, status);
            preparedStatement.setInt(2, waitingNum);
            return preparedStatement.executeUpdate() > 0;
        }
    }

    // UPDATE SQL 실행
    private boolean executeUpdate(String sql) throws SQLException {
        try (
                Connection conn = DBConnection.getConnection();
                PreparedStatement preparedStatement = conn.prepareStatement(sql)
        ) {
            return preparedStatement.executeUpdate() > 0;
        }
    }
}