import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
// Java와 DB 연결하기 위한 공통 클래스, 타 클래스에서 DB 접속 시 이 클래스 이용해 사용

class DBConnection {
    private static final String URL = "jdbc:mysql://localhost:3306/waitlist_db";
    private static final String USER = "root";
    private static final String PASSWORD = "1234";

    // Java와 DB 연결 위한 드라이버, 있어야만 MySQL 접속 가능
    static {
        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
        } catch (ClassNotFoundException e) {
            throw new ExceptionInInitializerError("MySQL Connector/J 라이브러리가 프로젝트에 추가되어 있지 않습니다.");
        }
    }

    // 객체 생성 막기 위한 private 생성자, getConnection 메서드만 사용하도록 함
    private DBConnection() {
    }

    // DB 연결 객체 반환하는 메서드, WaitingDB, StoreSettingDB 클래스에서 이 메서드 호출해 DB 접속함
    public static Connection getConnection() throws SQLException {
        return DriverManager.getConnection(URL, USER, PASSWORD);
    }
}
