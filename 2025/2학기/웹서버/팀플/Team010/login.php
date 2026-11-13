<?php
// 에러 화면에 바로 보이게 (디버깅용, 개발할 때만 켜두기)
error_reporting(E_ALL);
ini_set('display_errors', 1);

session_start();
require_once "db.php";

$err = "";

if ($_SERVER["REQUEST_METHOD"] === "POST") {
    $username = trim($_POST["username"] ?? "");
    $password = trim($_POST["password"] ?? "");

    if ($username === "" || $password === "") {
        $err = "아이디와 비밀번호를 모두 입력해주세요.";
    } else {
        // users 테이블 구조: id, username, email, nickname, password, is_admin, created_at
        $sql = "SELECT id, username, nickname, password, is_admin
                FROM users WHERE username = ?";
        $stmt = mysqli_prepare($conn, $sql);
        if ($stmt === false) {
            die("SQL 오류: " . htmlspecialchars(mysqli_error($conn)));
        }

        mysqli_stmt_bind_param($stmt, "s", $username);
        mysqli_stmt_execute($stmt);
        $result = mysqli_stmt_get_result($stmt);

        if ($row = mysqli_fetch_assoc($result)) {
            if (password_verify($password, $row["password"])) {
                // 로그인 성공 → 세션 저장
                $_SESSION["user_id"]  = $row["id"];
                $_SESSION["username"] = $row["username"];  // 아이디
                $_SESSION["nickname"] = $row["nickname"];  // 닉네임
                $_SESSION["is_admin"] = $row["is_admin"];  // 관리자 여부

                echo "<script>
                        localStorage.setItem('loggedIn', 'true');
                        window.location.href = 'mypage.php';
                    </script>";
                exit();
            } else {
                $err = "아이디 또는 비밀번호가 잘못되었습니다.";
            }
        } else {
            $err = "아이디 또는 비밀번호가 잘못되었습니다.";
        }
        mysqli_stmt_close($stmt);
    }
}
?>
<!DOCTYPE html>
<html lang="ko">
<head>
    <meta charset="UTF-8">
    <title>로그인 화면</title>
    <link rel="stylesheet" href="userCss.css">

    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Jua&display=swap" rel="stylesheet">

</head>
<body>

    <div class="login-UI">


    <div class="title">
        <p>로그인</p>
    </div>


    <?php if ($err): ?>
        <p style="color:red;"><?php echo htmlspecialchars($err); ?></p>
    <?php endif; ?>

    <form method="post" action="login.php">
        <p><input type="text" name="username" placeholder="ID"></p>
        <p><input type="password" name="password" placeholder="PASSWORD"></p>
        <p><button type="submit">로그인</button></p>
    </form>
    
    <p><a href="register.php">회원가입</a> | <a href="index.php">메인으로</a></p>


    </div>

    <script>
        document.getElementById('user-menu').addEventListener('click', () => {
            fetch('checkLogin.php')
                .then(res => res.json())
                .then(data => {
                    if (data.loggedIn) {
                        window.location.href = 'mypage.php';
                    } else {
                        window.location.href = 'login.php';
                    }
                })
                .catch(err => {
                    console.error('로그인 상태 확인 실패:', err);
                    window.location.href = 'login.php'; // 오류 시 기본적으로 로그인 페이지로
                });
        });

      // 햄버거 메뉴 토글 기능
      document.getElementById('burger-btn').addEventListener('click', function() {
          document.getElementById('side-menu').classList.add('open');
      });
      
      document.getElementById('close-btn').addEventListener('click', function() {
          document.getElementById('side-menu').classList.remove('open');
      });

      //이전 페이지로 돌아가기 기능
      document.getElementById('back-link').addEventListener('click', function(e) {
          e.preventDefault();
          window.history.back();
    
          document.getElementById('side-menu').classList.remove('open');
      });
      
      // 메뉴 외부 클릭 시 닫기 기능
      document.addEventListener('click', function(event) {
          const menu = document.getElementById('side-menu');
          const burger = document.getElementById('burger-btn');
          
          // 메뉴가 열려있고, 클릭된 요소가 메뉴나 햄버거 버튼 자체가 아닐 때
          if (menu.classList.contains('open') && event.target !== menu && event.target !== burger && !menu.contains(event.target)) {
              menu.classList.remove('open');
          }
      });
      </script>

</body>
</html>
