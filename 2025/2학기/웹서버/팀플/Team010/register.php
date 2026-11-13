<?php
session_start();
require_once "db.php";

$err = "";
$msg = "";

if ($_SERVER["REQUEST_METHOD"] === "POST") {
    $username  = trim($_POST["username"] ?? "");
    $email     = trim($_POST["email"] ?? "");
    $nickname  = trim($_POST["nickname"] ?? "");
    $password  = trim($_POST["password"] ?? "");
    $password2 = trim($_POST["password2"] ?? "");

    if ($username === "" || $email === "" || $nickname === "" || $password === "" || $password2 === "") {
        $err = "모든 항목을 입력해주세요.";
    } elseif ($password !== $password2) {
        $err = "비밀번호 확인이 일치하지 않습니다.";
    } else {
        // 중복 체크
        $sql = "SELECT id FROM users WHERE username = ? OR email = ?";
        $stmt = mysqli_prepare($conn, $sql);
        mysqli_stmt_bind_param($stmt, "ss", $username, $email);
        mysqli_stmt_execute($stmt);
        mysqli_stmt_store_result($stmt);

        if (mysqli_stmt_num_rows($stmt) > 0) {
            $err = "이미 사용 중인 아이디 또는 이메일입니다.";
        } else {
            $hash = password_hash($password, PASSWORD_DEFAULT);

            $sql = "INSERT INTO users (username, email, nickname, password)
                    VALUES (?, ?, ?, ?)";
            $stmt2 = mysqli_prepare($conn, $sql);
            mysqli_stmt_bind_param($stmt2, "ssss", $username, $email, $nickname, $hash);

            if (mysqli_stmt_execute($stmt2)) {
                $msg = "회원가입이 완료되었습니다. 로그인해주세요.";
            } else {
                $err = "회원가입 중 오류가 발생했습니다.";
            }
            mysqli_stmt_close($stmt2);
        }
        mysqli_stmt_close($stmt);
    }
}
?>
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>회원가입</title>
    <link rel="stylesheet" href="userCss.css">

    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Jua&display=swap" rel="stylesheet">
</head>
<body>


<div class="login-UI">

<div class="title">
        <p>회원가입</p>
</div>

    <?php if ($err): ?><p style="color:red; font-size : 20px;"><?= htmlspecialchars($err) ?></p><?php endif; ?>
    <?php if ($msg): ?><p style="color:green; font-size : 20px;"><?= htmlspecialchars($msg) ?></p><?php endif; ?>


<form method="post">
    <p>아이디: <input type="text" name="username"></p>
    <p>이메일: <input type="email" name="email"></p>
    <p>닉네임: <input type="text" name="nickname"></p>
    <p>비밀번호: <input type="password" name="password"></p>
    <p>비밀번호 확인: <input type="password" name="password2"></p>
    <button type="submit">등록</button>
</form>
</br>
<a href="login.php">로그인하기</a>
</br>
</div>

</body>
</html>
