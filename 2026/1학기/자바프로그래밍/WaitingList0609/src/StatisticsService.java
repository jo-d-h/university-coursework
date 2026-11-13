import java.sql.SQLException;
import java.util.Map;
import java.time.LocalTime;

public class StatisticsService {

    private final StatisticsDB db = new StatisticsDB();
    private final StoreSettingDB storeSettingDB = new StoreSettingDB();

    static class StatisticsSummary {
        final int totalWaitingCount;
        final int enteredCount;
        final int cancelCount;
        final int noShowCount;
        final int activeWaitingCount;
        final int maxWaitingCount;
        final int busyRatio;
        final String busyLevel;

        StatisticsSummary(int totalWaitingCount, int enteredCount, int cancelCount, int noShowCount,
                          int activeWaitingCount, int maxWaitingCount, int busyRatio, String busyLevel) {
            this.totalWaitingCount = totalWaitingCount;
            this.enteredCount = enteredCount;
            this.cancelCount = cancelCount;
            this.noShowCount = noShowCount;
            this.activeWaitingCount = activeWaitingCount;
            this.maxWaitingCount = maxWaitingCount;
            this.busyRatio = busyRatio;
            this.busyLevel = busyLevel;
        }
    }

    public StatisticsSummary getStatisticsSummary() throws SQLException {
        int activeWaitingCount = db.getActiveWaitingCount();
        int totalWaitingCount = db.getTotalWaitingCount();

        // DB store_setting 테이블에서 최대 대기 팀 수 조회
        int maxWaitingCount = storeSettingDB.getMaxWaitingCount();

        // Math.round는 반올림 함수
        int busyRatio = (int) Math.round((double) activeWaitingCount / maxWaitingCount * 100);

        //비율이 100% 안넘도록
        if (busyRatio > 100) busyRatio = 100;

        return new StatisticsSummary(
                totalWaitingCount,
                getEnteredCount(),
                getCancelCount(),
                getNoShowCount(),
                activeWaitingCount,
                maxWaitingCount,
                busyRatio,
                getBusyLevelByRatio(busyRatio)
        );
    }

    private String getBusyLevelByRatio(int ratio) {
        if (ratio < 30) return "LOW";
        else if (ratio <= 70) return "NORMAL";
        return "HIGH";
    }

    public String getBusyLevel() throws SQLException {
        int count = db.getActiveWaitingCount();
        int maxWaitingCount = storeSettingDB.getMaxWaitingCount();
        // 최대 대기 수 기준: 33% 이하 여유, 67% 이하 보통, 초과 혼잡
        if (count <= maxWaitingCount * 0.33) return "여유";
        else if (count <= maxWaitingCount * 0.67) return "보통";
        else return "혼잡";
    }

    public String predictBusyLevelByHour() throws SQLException {
        Map<Integer, Integer> hourly = db.getHourlyWaitingStats();
        int currentHour = LocalTime.now().getHour();
        int currentHourCount = hourly.getOrDefault(currentHour, 0);
        int maxCount = 0;
        for (int count : hourly.values()) { if (count > maxCount) maxCount = count; }
        if (maxCount == 0) return "여유 예상";
        double ratio = (double) currentHourCount / maxCount;
        if (ratio <= 0.3) return "여유 예상";
        else if (ratio <= 0.7) return "보통 예상";
        else return "혼잡 예상";
    }

    public String predictByAverage() throws SQLException {
        int current = db.getActiveWaitingCount();
        int maxWaitingCount = storeSettingDB.getMaxWaitingCount();
        if (current >= maxWaitingCount / 2) return "평소보다 혼잡";
        else if (current <= 2) return "평소보다 여유";
        else return "평균 수준";
    }

    public int predictPeakHour() throws SQLException {
        Map<Integer, Integer> stats = db.getHourlyWaitingStats();
        int peakHour = 0; int max = 0;
        for (int hour : stats.keySet()) {
            if (stats.get(hour) > max) { max = stats.get(hour); peakHour = hour; }
        }
        return peakHour;
    }

    public int getTotalWaitingCount() throws SQLException { return db.getTotalWaitingCount(); }
    public int getEnteredCount() throws SQLException { return db.getEnteredCount(); }
    public int getCancelCount() throws SQLException { return db.getCancelCount(); }
    public int getNoShowCount() throws SQLException { return db.getNoShowCount(); }
}