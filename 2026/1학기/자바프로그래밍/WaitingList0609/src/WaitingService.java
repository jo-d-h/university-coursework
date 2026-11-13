import java.sql.SQLException;
import java.time.LocalDateTime;
import java.time.temporal.ChronoUnit;
import java.util.LinkedList;
import java.util.List;

class WaitingService {
    public final List<Waiting> waitingList = new LinkedList<>();
    private final WaitingDB waitingDB = new WaitingDB();
    private final StoreSettingDB storeSettingDB = new StoreSettingDB();
    private String storeStatus = "OPEN";

    static class OperationResult {
        private final boolean success;
        private final String message;
        private final Waiting waiting;

        OperationResult(boolean success, String message, Waiting waiting) {
            this.success = success;
            this.message = message;
            this.waiting = waiting;
        }

        public boolean isSuccess() {
            return success;
        }

        public String getMessage() {
            return message;
        }

        public Waiting getWaiting() {
            return waiting;
        }
    }

    public List<Waiting> getActiveWaitingList() throws SQLException {
        return waitingDB.findActiveWaitingList();
    }

    public List<Waiting> getAllWaitingList() throws SQLException {
        return waitingDB.findAllWaitingList();
    }

    // 로그인 된 회원의 대기 목록 조회
    public List<Waiting> getWaitingListByMemberId(int memberId) throws SQLException {
        return waitingDB.findWaitingListByMemberId(memberId);
    }

    // 로그인한 회원의 현재 대기 순번 조회
    //반환값: 양수 = 대기 순번, 0 = 대기 없음, -1 = 호출됨
    public int getWaitingPosition(int memberId) throws SQLException {
        List<Waiting> myWaitingList = waitingDB.findWaitingListByMemberId(memberId); // 현재 로그인 된 회원의 대기 목록을 조회

        for (Waiting waiting : myWaitingList) {
            if (waiting.status == Status.WAIT) { // 상태가 WAIT 이면
                return waitingDB.countActiveWaitingBefore(waiting.waitingNum) + 1; // 앞 팀 수 + 1 반환
            }
            if (waiting.status == Status.CALL) { //호출된 상태
                return -1;
            }
        }

        return 0; // 대기 팀이 없으면 0 반환
    }

    // 회원 기준 대기 등록 처리
    public OperationResult registerWaiting(int memberId, String name, String phone, int totalCount) {
        // 입력 값 유효 검증
        if (memberId <= 0) {
            return fail("회원 정보가 올바르지 않습니다.");
        }

        if (isBlank(name) || isBlank(phone)) {
            return fail("이름과 전화번호를 입력해주세요.");
        }

        if (totalCount <= 0) {
            return fail("인원 수는 1명 이상이어야 합니다.");
        }

        // 매장 상태, 중복 대기, 최대 대기 수 확인
        try {
            String waitingStatus = getStoreStatus();
            if (!waitingStatus.equals("OPEN")) {
                return fail("현재 대기 접수가 불가능한 상태입니다.");
            }

            if (waitingDB.hasActiveWaiting(memberId)) {
                return fail("이미 대기 중인 내역이 있습니다.");
            }

            int maxWaitingCount = storeSettingDB.getMaxWaitingCount();
            if (waitingDB.countActiveWaitingTeams() >= maxWaitingCount) {
                return fail("현재 대기 가능 팀 수가 초과되었습니다.");
            }

            // 대기 등록 후 생성된 대기번호로 등록 결과 조회
            int waitingNum = waitingDB.insertWaiting(memberId, name, phone, totalCount);
            Waiting waiting = findByNumber(waitingDB.findAllWaitingList(), waitingNum);

            // 등록은 성공했지만 조회 결과가 없을 경우 대비
            if (waiting == null) {
                return success("대기 등록 완료. 대기번호: " + String.format("%03d", waitingNum), null);
            }

            return success("대기 등록 완료 " + formatWaiting(waiting, "WAIT"), waiting);
        } catch (SQLException e) {
            return fail("대기 등록 중 DB 오류가 발생했습니다.\n" + e.getMessage());
        }
    }

    public List<Waiting> getCallWaitingList() throws SQLException {
        List<Waiting> callList = new LinkedList<>();

        for (Waiting waiting : waitingDB.findActiveWaitingList()) {
            if (waiting.status == Status.CALL) {
                callList.add(waiting);
            }
        }

        return callList;
    }

    public long getPassedMinutes(LocalDateTime calledAt) {
        if (calledAt == null) {
            return 0;
        }
        return ChronoUnit.MINUTES.between(calledAt, LocalDateTime.now());
    }

    public OperationResult callNextWaiting() {
        try {
            Waiting target = null;

            for (Waiting waiting : waitingDB.findActiveWaitingList()) {
                if (waiting.status == Status.WAIT) {
                    target = waiting;
                    break;
                }
            }

            if (target == null) {
                return fail("호출할 대기 고객이 없습니다.");
            }

            if (waitingDB.callWaiting(target.waitingNum)) {
                return success("고객 호출 완료 " + formatWaiting(target, "CALL"), target);
            }

            return fail("고객 호출에 실패했습니다.");
        } catch (SQLException e) {
            return fail("고객 호출 중 DB 오류가 발생했습니다.\n" + e.getMessage());
        }
    }

    public OperationResult enterWaiting(int waitingNum) {
        try {
            Waiting target = findByNumber(waitingDB.findAllWaitingList(), waitingNum);

            if (target == null) {
                return fail("해당 대기번호를 찾을 수 없습니다.");
            }

            if (target.status != Status.CALL) {
                return fail("호출 상태인 고객만 입장 처리할 수 있습니다.");
            }

            if (waitingDB.enterWaiting(waitingNum)) {
                return success("입장 처리 완료 " + formatWaiting(target, "IN"), target);
            }

            return fail("입장 처리에 실패했습니다.");
        } catch (SQLException e) {
            return fail("입장 처리 중 DB 오류가 발생했습니다.\n" + e.getMessage());
        }
    }

    public OperationResult cancelWaitingFromDB(int waitingNum) {
        try {
            Waiting target = findByNumber(waitingDB.findAllWaitingList(), waitingNum);

            if (target == null) {
                return fail("해당 대기번호를 찾을 수 없습니다.");
            }

            if (target.status == Status.IN) {
                return fail("이미 입장 처리된 고객은 취소할 수 없습니다.");
            }

            if (target.status == Status.CANCEL) {
                return fail("이미 취소 처리된 대기입니다.");
            }

            if (target.status == Status.NO_SHOW) {
                return fail("노쇼 처리된 고객은 취소 처리할 수 없습니다.");
            }

            if (waitingDB.cancelWaiting(waitingNum)) { // 로그인 된 회원의 본인 대기 취소
                return success("대기 취소 처리 완료 " + formatWaiting(target, "CANCEL"), target);
            }

            return fail("대기 취소 처리에 실패했습니다.");
        } catch (SQLException e) {
            return fail("대기 취소 처리 중 DB 오류가 발생했습니다.\n" + e.getMessage());
        }
    }

    // 로그인한 회원의 본인 대기 취소
    public OperationResult cancelMyWaiting(int memberId, int waitingNum) {
        if (memberId <= 0) {
            return fail("회원 정보가 올바르지 않습니다.");
        }

        if (waitingNum <= 0) {
            return fail("대기번호가 올바르지 않습니다.");
        }

        try {
            Waiting target = findByNumber(waitingDB.findWaitingListByMemberId(memberId), waitingNum);

            if (target == null) {
                return fail("본인의 대기 내역을 찾을 수 없습니다.");
            }

            if (target.status == Status.IN) {
                return fail("이미 입장 처리된 대기는 취소할 수 없습니다.");
            }

            if (target.status == Status.CANCEL) {
                return fail("이미 취소 처리된 대기입니다.");
            }

            if (target.status == Status.NO_SHOW) {
                return fail("노쇼 처리된 대기는 취소할 수 없습니다.");
            }

            if (waitingDB.cancelWaitingByMemberId(waitingNum, memberId)) {
                return success("대기 취소 완료 " + formatWaiting(target, "CANCEL"), target);
            }

            return fail("대기 취소에 실패했습니다.");
        } catch (SQLException e) {
            return fail("대기 취소 중 DB 오류가 발생했습니다.\n" + e.getMessage());
        }
    }

    public OperationResult noShowWaitingFromDB(int waitingNum) {
        try {
            Waiting target = findByNumber(waitingDB.findAllWaitingList(), waitingNum);

            if (target == null) {
                return fail("해당 대기번호를 찾을 수 없습니다.");
            }

            if (target.status != Status.CALL) {
                return fail("호출 상태인 고객만 노쇼 처리할 수 있습니다.");
            }

            if (target.calledAt == null) {
                return fail("호출 시간 정보가 없습니다.");
            }

            long minutesPassed = getPassedMinutes(target.calledAt);
            int limitMinutes = storeSettingDB.getNoShowLimitMinutes();
            if (minutesPassed < limitMinutes) {
                return fail("아직 제한 시간이 지나지 않았습니다. (호출 후 " + minutesPassed + "분 경과)");
            }

            if (waitingDB.noShowWaiting(waitingNum)) {
                return success("노쇼 처리 완료 " + formatWaiting(target, "NO_SHOW"), target);
            }

            return fail("노쇼 처리에 실패했습니다.");
        } catch (SQLException e) {
            return fail("노쇼 처리 중 DB 오류가 발생했습니다.\n" + e.getMessage());
        }
    }

    public OperationResult changeStoreStatusForDisplay(String newStatus) {
        try {
            validateStoreStatus(newStatus);

            String currentStatus = getStoreStatus();
            if (currentStatus.equals(newStatus)) {
                return fail("이미 " + newStatus + " 상태입니다.");
            }

            if (storeSettingDB.updateWaitingStatus(newStatus)) {
                storeStatus = newStatus;
                return success("웨이팅 상태가 " + newStatus + "로 변경되었습니다.", null);
            }

            return fail("웨이팅 상태 변경에 실패했습니다.");
        } catch (IllegalArgumentException e) {
            return fail(e.getMessage());
        } catch (Exception e) {
            return fail("상태 변경 중 DB 오류가 발생했습니다.\n" + e.getMessage());
        }
    }

    public String getStoreStatus() {
        try {
            storeStatus = storeSettingDB.getWaitingStatus();
        } catch (SQLException e) {
            return storeStatus;
        }
        return storeStatus;
    }

    private OperationResult success(String message, Waiting waiting) {
        return new OperationResult(true, "-> " + message, waiting);
    }

    private OperationResult fail(String message) {
        return new OperationResult(false, "-> " + message, null);
    }

    private String formatWaiting(Waiting waiting, String status) {
        return "[" + String.format("%03d", waiting.waitingNum) + "번 "
                + waiting.name + "(" + waiting.phone + ") | "
                + waiting.totalCount + "명 | 상태: " + status + " | 등록시간: "
                + waiting.createdAt.toLocalTime().withNano(0) + "]";
    }

    private Waiting findByNumber(List<Waiting> list, int waitingNum) {
        for (Waiting waiting : list) {
            if (waiting.waitingNum == waitingNum) {
                return waiting;
            }
        }
        return null;
    }

    // null 또는 공백만 있는 문자열인지 확인
    private boolean isBlank(String value) {
        return value == null || value.trim().isEmpty();
    }

    private void validateStoreStatus(String newStatus) {
        if (!newStatus.equals("OPEN") && !newStatus.equals("PAUSED") && !newStatus.equals("CLOSED")) {
            throw new IllegalArgumentException("OPEN / PAUSED / CLOSED 중 입력해주세요.");
        }
    }
}


