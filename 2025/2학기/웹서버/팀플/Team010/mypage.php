<?php
// mypage.php (XML 사용하지 않는 DB 전용 버전)

require_once "auth.php";  // require_login() 제공
require_once "db.php";    // $conn 제공

require_login();          // 로그인 안 하면 자동 차단

// 세션 정보
$user_id  = $_SESSION["user_id"];
$username = $_SESSION["username"];
$nickname = $_SESSION["nickname"];

// ----------------------------
// 1. 사용자 정보 불러오기
// ----------------------------
$sql_user = "
    SELECT username, email, nickname, is_admin, created_at
    FROM users
    WHERE id = ?
";
$stmt_user = mysqli_prepare($conn, $sql_user);
mysqli_stmt_bind_param($stmt_user, "i", $user_id);
mysqli_stmt_execute($stmt_user);
$res_user = mysqli_stmt_get_result($stmt_user);
$user = mysqli_fetch_assoc($res_user);
mysqli_stmt_close($stmt_user);

if (!$user) {
    die("회원 정보를 찾을 수 없습니다.");
}

// ----------------------------
// 2. 내 리뷰 목록 불러오기
// ----------------------------
$sql_rev = "
    SELECT
        id AS review_id,
        shop_name,
        content,
        good_point,
        star,
        created_at,
        reviewer_image            
    FROM reviews
    WHERE user_id = ?
    ORDER BY id DESC
";


$stmt_rev = mysqli_prepare($conn, $sql_rev);
mysqli_stmt_bind_param($stmt_rev, "i", $user_id);
mysqli_stmt_execute($stmt_rev);
$res_rev = mysqli_stmt_get_result($stmt_rev);

$my_reviews = [];
while ($row = mysqli_fetch_assoc($res_rev)) {
    $my_reviews[] = $row;
}
mysqli_stmt_close($stmt_rev);

// HTML 안전 출력 헬퍼
function h($s) {
    return htmlspecialchars($s ?? "", ENT_QUOTES | ENT_SUBSTITUTE, "UTF-8");
}
?>
<!DOCTYPE html>
<html lang="ko">
<head>
    <meta charset="UTF-8">
    <title>마이페이지</title>

    <link rel="stylesheet" href="userCss.css">
    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Jua&display=swap" rel="stylesheet">

</head>
<body>
    <div class="mypage-UI">

    <div class="user-title"><p>마이페이지</p></div>

    <p class="welcome"><strong class="user-name"><?php echo h($nickname); ?></strong> 님 환영합니다.</p>

    <div class="content-wrapper">

    <div class="user-info">
    <h2>개인 정보ℹ️</h2>
    <ul>
        <li>아이디: <?php echo h($user["username"]); ?></li>
        <li>이메일: <?php echo h($user["email"]); ?></li>
        <li>닉네임: <?php echo h($user["nickname"]); ?></li>
        <li>가입일: <?php echo h($user["created_at"]); ?></li>
    </ul>

    <p>
        <a href="profile_edit.php">개인정보 수정</a> |
        <a href="withdraw.php" onclick="return confirm('정말 탈퇴하시겠습니까?');">회원 탈퇴</a>
    </p>
    </div>

    <div class = "review-history">
    <h2>내 리뷰 목록⭐</h2>

    <?php if (empty($my_reviews)): ?>
        <p>아직 작성한 리뷰가 없습니다.</p>
    <?php else: ?>
        <ul>
            <?php foreach ($my_reviews as $r): ?>
            <li> <div class="review-item">
                [<?php echo h($r["shop_name"]); ?>]
                <?php if (!empty($r["star"]) && (int)$r["star"] > 0): ?>
                    · ★ <?php echo (int)$r["star"]; ?>/5
                <?php endif; ?>
                <br>

                <?php if (!empty($r["good_point"])): ?>
                    👍 <?php echo h($r["good_point"]); ?><br>
                <?php endif; ?>

                <?php echo nl2br(h($r["content"])); ?>

                <?php if (!empty($r["reviewer_image"])): ?>
                    <br>
                    <img src="<?php echo h($r["reviewer_image"]); ?>"
                        alt="리뷰 이미지"
                        style="max-width:150px; max-height:150px; margin-top:4px; border:1px solid #ccc; border-radius:8px;">
                <?php endif; ?>

                <br>
                • <a href="review_edit.php?idx=<?php echo (int)$r['review_id']; ?>">수정</a>
                • <a href="review_delete.php?idx=<?php echo (int)$r['review_id']; ?>&from=mypage"
                    onclick="return confirm('정말 이 리뷰를 삭제하시겠습니까?');">
                    <normal style = "color : red">삭제</normal>
                </a>
                </div></li>
        <?php endforeach; ?>
        </ul>
    <?php endif; ?>
                </div></div>
    <?php if (is_admin()): ?>
    <div class="admin-menu">
        <h3>관리자 메뉴</h3>
        <p><a href="admin.php">관리자 페이지로 이동</a></p>
    </div>
    <?php endif; ?>

    <p>
        <a href="index.php">메인으로</a> |
        <a href="logout.php">로그아웃</a>
    </p>
    
    </div>

</body>
</html>
