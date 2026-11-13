<?php
// backup.php : DB 백업/업데이트 페이지

// 한국 시간 기준으로 표시
date_default_timezone_set('Asia/Seoul');

// DB 연결 정보
$host   = "localhost";
$user   = "root";
$pass   = "";
$dbname = "Team010";

$conn = new mysqli($host, $user, $pass, $dbname);
if ($conn->connect_error) {
    die("DB 연결 실패: " . $conn->connect_error);
}

// =======================
// 1. DB 설치 시간 가져오기
//    (db_install.php 또는 이 페이지에서 db_meta에 기록한 값)
// =======================
$install_time = null;

$result = $conn->query("SHOW TABLES LIKE 'db_meta'");
if ($result && $result->num_rows > 0) {
    $result->free();

    $result2 = $conn->query("SELECT installed_at FROM db_meta WHERE id = 1");
    if ($result2 && $row = $result2->fetch_assoc()) {
        $install_time = $row['installed_at'];   // Y-m-d H:i:s
    }
    if ($result2) {
        $result2->free();
    }
} elseif ($result) {
    $result->free();
}

// =======================
// 2. Team010.sql 파일 시간
// =======================
$sql_file  = __DIR__ . DIRECTORY_SEPARATOR . "Team010.sql";  // 절대경로
$file_time = "파일 없음";

if (file_exists($sql_file)) {
    clearstatcache(true, $sql_file);  // 캐시 초기화
    $file_time = date("Y-m-d H:i:s", filemtime($sql_file));
}

// =======================
// 3. 버튼 동작 처리 (POST)
// =======================
$message = "";

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // XAMPP의 mysqldump / mysql 경로
    $mysqldumpExe = 'C:\\xampp\\mysql\\bin\\mysqldump.exe';
    $mysqlExe     = 'C:\\xampp\\mysql\\bin\\mysql.exe';

    if (isset($_POST['backup'])) {
        // ---------- DB → Team010.sql 백업 ----------
        if ($pass === "") {
            $backup_cmd = "\"{$mysqldumpExe}\" -u {$user} {$dbname} > \"{$sql_file}\"";
        } else {
            $backup_cmd = "\"{$mysqldumpExe}\" -u {$user} -p{$pass} {$dbname} > \"{$sql_file}\"";
        }

        $output      = [];
        $result_code = 0;
        exec($backup_cmd, $output, $result_code);

        if ($result_code === 0) {
            clearstatcache(true, $sql_file);
            if (file_exists($sql_file)) {
                $file_time = date("Y-m-d H:i:s", filemtime($sql_file));
            }
            $message = "DB 백업이 완료되었습니다.";
        } else {
            $message  = "백업 중 오류 발생!\n";
            $message .= "명령어: {$backup_cmd}\n";
            $message .= "출력: " . implode("\n", $output);
        }

    } elseif (isset($_POST['update'])) {
        // ---------- Team010.sql → DB 업데이트 ----------
        if (!file_exists($sql_file)) {
            $message = "Team010.sql 파일이 존재하지 않습니다. 먼저 DB 백업을 수행하세요.";
        } else {
            if ($pass === "") {
                $update_cmd = "\"{$mysqlExe}\" -u {$user} {$dbname} < \"{$sql_file}\"";
            } else {
                $update_cmd = "\"{$mysqlExe}\" -u {$user} -p{$pass} {$dbname} < \"{$sql_file}\"";
            }

            $output      = [];
            $result_code = 0;
            exec($update_cmd, $output, $result_code);

            if ($result_code === 0) {
                // 업데이트가 정상 완료됐으므로 "DB 설치(적용) 시간" 갱신
                $conn->query("
                    CREATE TABLE IF NOT EXISTS db_meta (
                        id INT PRIMARY KEY,
                        installed_at DATETIME NOT NULL
                    ) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4
                ");

                $conn->query("
                    INSERT INTO db_meta (id, installed_at)
                    VALUES (1, NOW())
                    ON DUPLICATE KEY UPDATE installed_at = VALUES(installed_at)
                ");

                $install_time = date("Y-m-d H:i:s");  // 화면에서도 바로 반영
                $message = "DB 업데이트가 완료되었습니다.";
            } else {
                $message  = "업데이트 중 오류 발생!\n";
                $message .= "명령어: {$update_cmd}\n";
                $message .= "출력: " . implode("\n", $output);
            }
        }
    }
}
?>
<!DOCTYPE html>
<html lang="ko">
<head>
    <meta charset="UTF-8">
    <title>DB 백업/업데이트</title>

    <link rel="stylesheet" href="userCss.css">

    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Jua&display=swap" rel="stylesheet">
</head>
<body>

    <div class="login-UI">

    <p class = "title">DB 백업/업데이트 페이지</p>

    <p class="user-info">
        <normal class = "back-font">DB 설치 시간:</normal>
        <normal style = "color : green; font-size : 20px;"><?= $install_time ?: "알 수 없음" ?></normal>
    </p>
    <p class="user-info">
        <normal class = "back-font">Team010.sql 파일 시간:</normal>
        <normal style = "color : green; font-size : 20px;"><?= $file_time ?></normal>
    </p>

    <form method="post">
        <button type="submit" name="backup" class = "backup-button">DB 백업하기</button>
        <button type="submit" name="update" class = "backup-button"
                onclick="return confirm('Team010.sql의 내용을 현재 DB에 적용하시겠습니까?');">
            DB 업데이트하기
        </button>
    </form>

    <?php if ($message): ?>
        <p style="white-space:pre-line; color:#333; font-size : 20px">
            <?= htmlspecialchars($message, ENT_QUOTES, 'UTF-8') ?>
        </p>

    <p>
        <a href="admin.php" class="btn-link"
           style="display:inline-block; padding:8px 12px; background:#ccc; border-radius:5px; text-decoration:none;">
            관리자 페이지로 돌아가기
        </a>
    </p>

</div>

    
    <?php endif; ?>
</body>
</html>
