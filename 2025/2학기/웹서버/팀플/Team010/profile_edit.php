<?php
require_once "auth.php";
require_once "db.php";

require_login();

$user_id = $_SESSION["user_id"];
$err = "";
$msg = "";

// 현재 정보 가져오기 (username, email, nickname만)
$sql = "SELECT username, email, nickname FROM users WHERE id = ?";
$stmt = mysqli_prepare($conn, $sql);
mysqli_stmt_bind_param($stmt, "i", $user_id);
mysqli_stmt_execute($stmt);
$result = mysqli_stmt_get_result($stmt);
$user = mysqli_fetch_assoc($result);
mysqli_stmt_close($stmt);

if (!$user) {
    die("사용자 정보를 찾을 수 없습니다.");
}

if ($_SERVER["REQUEST_METHOD"] === "POST") {
    $email    = trim($_POST["email"] ?? "");
    $nickname = trim($_POST["nickname"] ?? "");
    $new_pw   = trim($_POST["new_password"] ?? "");

    if ($email === "" || $nickname === "") {
        $err = "이메일과 닉네임을 모두 입력해주세요.";
    } else {
        // 이메일, 닉네임 수정
        $sql = "UPDATE users SET email = ?, nickname = ? WHERE id = ?";
        $stmt = mysqli_prepare($conn, $sql);
        mysqli_stmt_bind_param($stmt, "ssi", $email, $nickname, $user_id);
        mysqli_stmt_execute($stmt);
        mysqli_stmt_close($stmt);

        // 비밀번호 변경 (입력된 경우만)
        if ($new_pw !== "") {
            $hash = password_hash($new_pw, PASSWORD_DEFAULT);
            $sql = "UPDATE users SET password = ? WHERE id = ?";
            $stmt = mysqli_prepare($conn, $sql);
            mysqli_stmt_bind_param($stmt, "si", $hash, $user_id);
            mysqli_stmt_execute($stmt);
            mysqli_stmt_close($stmt);
        }

        // 세션에 닉네임도 갱신
        $_SESSION["nickname"] = $nickname;

        $msg = "수정이 완료되었습니다.";
    }
}
?>
<!DOCTYPE html>
<html lang="ko">
<head>
    <meta charset="UTF-8">
    <title>개인정보 수정</title>
    <link rel="stylesheet" href="userCss.css">
    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Jua&display=swap" rel="stylesheet">

</head>
<body>
    <div class="edit-UI">
    <h1>개인정보 수정</h1>

    <?php if ($err): ?>
        <p style="color:red;"><?php echo htmlspecialchars($err); ?></p>
    <?php endif; ?>
    <?php if ($msg): ?>
        <p style="color:green;"><?php echo htmlspecialchars($msg); ?></p>
    <?php endif; ?>

    <form method="post" action="profile_edit.php">
        <p>아이디 (변경 불가)<br><span class="user-name"><?php echo htmlspecialchars($user["username"]); ?></span></p>
        <p>이메일<br><input type="email" name="email" value="<?php echo htmlspecialchars($user["email"]); ?>"></p>
        <p>닉네임<br><input type="text" name="nickname" value="<?php echo htmlspecialchars($user["nickname"]); ?>"></p>
        <p>새 비밀번호(변경 시만 입력)<br><input type="password" name="new_password"></p>
        <p><button type="submit">저장</button></p>
    </form>

    <p><a href="mypage.php">마이페이지로</a></p>
</body>
</html>
