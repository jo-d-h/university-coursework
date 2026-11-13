<?php
// db.php : 프로젝트 공통 DB 연결 파일
// 사용자: root, 패스워드: (빈 문자열)
// DB명: Team010

$host   = "localhost";
$user   = "root";
$pass   = "";           // XAMPP 기본값 (비밀번호 없음)
$dbname = "Team010";    // db_init.php에서 만든 DB 이름과 동일

$conn = mysqli_connect($host, $user, $pass, $dbname);

if (!$conn) {
    die("DB 연결 실패: " . mysqli_connect_error());
}

// 한글 깨짐 방지
mysqli_set_charset($conn, "utf8mb4");
?>
