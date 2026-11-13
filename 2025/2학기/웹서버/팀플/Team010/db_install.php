<?php
// db_install.php - DB 및 초기 데이터 설치용 스크립트

$host = "localhost";
$user = "root";
$pass = "";
$dbname = "Team010";  // db.php와 동일하게 유지

// 1. MySQL 서버 연결 (DB 선택 X)
$conn = mysqli_connect($host, $user, $pass);
if (!$conn) {
    die("MySQL 서버 연결 실패: " . mysqli_connect_error());
}

// 2. 데이터베이스 생성
$sql = "CREATE DATABASE IF NOT EXISTS `$dbname` CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci";
if (!mysqli_query($conn, $sql)) {
    die("데이터베이스 생성 실패: " . mysqli_error($conn));
}

// 3. 방금 만든 DB 선택
if (!mysqli_select_db($conn, $dbname)) {
    die("데이터베이스 선택 실패: " . mysqli_error($conn));
}

// 3.1 이미 설치 여부 확인 (테이블 존재 여부 검사)
$escapedDb = mysqli_real_escape_string($conn, $dbname);
$checkSql = "SELECT COUNT(*) AS cnt FROM information_schema.tables WHERE table_schema = '$escapedDb'";
$checkRes = mysqli_query($conn, $checkSql);
if ($checkRes) {
    $row = mysqli_fetch_assoc($checkRes);
    if (!empty($row['cnt']) && intval($row['cnt']) > 0) {
        mysqli_free_result($checkRes);
        mysqli_close($conn);
        echo "
        <div style='font-family: Arial; padding:20px;'>
            <h2>ℹ️ DB가 이미 설치되어 있습니다</h2>
            <p>데이터베이스와 테이블이 이미 존재합니다. 다시 설치할 필요가 없습니다.</p>
            </ul>

            <p style='margin-top:20px;'>
                <a href='index.php'>메인 페이지로 이동</a>
            </p>
        </div>
        ";
        exit;
    }
    mysqli_free_result($checkRes);
}

// 4. SQL 파일 경로 (팀에서 공유하는 Team010.sql)
$sqlFile = __DIR__ . "/Team010.sql";   // 실제 파일명과 맞춰줌 (대문자)

if (!file_exists($sqlFile)) {
    die(
        "SQL 파일을 찾을 수 없습니다: " .
        htmlspecialchars($sqlFile, ENT_QUOTES | ENT_SUBSTITUTE, "UTF-8")
    );
}

$sqlContents = file_get_contents($sqlFile);
if ($sqlContents === false) {
    die("SQL 파일을 읽을 수 없습니다.");
}

// 5. 여러 쿼리 한 번에 실행
if (!mysqli_multi_query($conn, $sqlContents)) {
    die("SQL 실행 중 오류: " . mysqli_error($conn));
}

// multi_query 사용 시 남아있는 결과 비우기
do {
    if ($result = mysqli_store_result($conn)) {
        mysqli_free_result($result);
    }
} while (mysqli_more_results($conn) && mysqli_next_result($conn));

// 설치 시간 기록용 테이블 생성
$metaSql = "
    CREATE TABLE IF NOT EXISTS db_meta (
        id INT PRIMARY KEY,
        installed_at DATETIME NOT NULL
    ) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4
";
mysqli_query($conn, $metaSql);

// 현재 시각을 설치 시간으로 저장 (있으면 갱신)
$nowSql = "
    INSERT INTO db_meta (id, installed_at)
    VALUES (1, NOW())
    ON DUPLICATE KEY UPDATE installed_at = VALUES(installed_at)
";
mysqli_query($conn, $nowSql);

mysqli_close($conn);

echo "
<div style='font-family: Arial; padding:20px;'>
    <h2>✅ 설치가 완료되었습니다!</h2>
    <p>DB 및 테이블, 초기 데이터가 정상적으로 설치되었습니다.</p>

    <p style='margin-top:20px;'>
        <a href='index.php'>
            메인 페이지로 이동
        </a>
    </p>
</div>
";
?>
