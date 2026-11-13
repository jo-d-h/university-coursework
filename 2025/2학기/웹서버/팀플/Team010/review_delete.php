<?php
// review_delete.php : 리뷰 삭제
// - 일반 사용자: 본인 리뷰만 삭제 가능
// - 관리자: 모든 리뷰 삭제 가능
//   (혼밥하기 좋아요 카운트(solo_good_cnt)는 공통으로 감소 처리)

require_once "auth.php";
require_once "db.php";

require_login(); // 로그인 필수

if (!function_exists('h')) {
    function h($s) {
        return htmlspecialchars($s ?? "", ENT_QUOTES | ENT_SUBSTITUTE, "UTF-8");
    }
}

// 현재 로그인 사용자 정보
$user_id  = $_SESSION["user_id"] ?? 0;
$is_admin = is_admin();   // 관리자 여부

if ($user_id <= 0) {
    die("로그인 정보가 없습니다. <a href='login.php'>로그인</a>");
}

// -----------------------------------------
// 1) 삭제할 리뷰 id 확인
// -----------------------------------------
$review_id = 0;

// mypage.php는 idx, admin.php는 id로 보낼 수 있어 둘 다 허용
if (isset($_GET["idx"])) {
    $review_id = (int)$_GET["idx"];
} elseif (isset($_GET["id"])) {
    $review_id = (int)$_GET["id"];
}

if ($review_id <= 0) {
    die("잘못된 접근입니다. <a href='mypage.php'>마이페이지</a>");
}

// -----------------------------------------
// 2) 리뷰 조회
// -----------------------------------------
if ($is_admin) {
    // 관리자 → user_id 체크 없이 조회
    $sql = "
        SELECT r.*, s.id AS shop_id
        FROM reviews r
        LEFT JOIN shops s ON r.shop_id = s.id
        WHERE r.id = ?
    ";
    $stmt = mysqli_prepare($conn, $sql);
    mysqli_stmt_bind_param($stmt, "i", $review_id);
} else {
    // 일반 유저 → 본인 리뷰만 조회 가능
    $sql = "
        SELECT r.*, s.id AS shop_id
        FROM reviews r
        LEFT JOIN shops s ON r.shop_id = s.id
        WHERE r.id = ? AND r.user_id = ?
    ";
    $stmt = mysqli_prepare($conn, $sql);
    mysqli_stmt_bind_param($stmt, "ii", $review_id, $user_id);
}

if (!$stmt) {
    die("리뷰 조회 오류: " . mysqli_error($conn));
}

mysqli_stmt_execute($stmt);
$result = mysqli_stmt_get_result($stmt);
$review = mysqli_fetch_assoc($result);
mysqli_stmt_close($stmt);

if (!$review) {
    die("존재하지 않거나 삭제 권한이 없습니다. <a href='mypage.php'>마이페이지</a>");
}

// 필요한 값 추출
$shop_id    = (int)$review["shop_id"];          // shops.id
$good_point = $review["good_point"];
$image_path = $review["reviewer_image"];        // 이미지 경로

// -----------------------------------------
// 3) 혼밥 카운트 감소 처리
// -----------------------------------------
if ($good_point === "혼밥하기 좋아요") {

    $sql_cnt = "
        UPDATE shops
        SET solo_good_cnt = CASE
            WHEN solo_good_cnt > 0 THEN solo_good_cnt - 1
            ELSE 0
        END
        WHERE id = ?
    ";
    $st_cnt = mysqli_prepare($conn, $sql_cnt);

    if ($st_cnt) {
        mysqli_stmt_bind_param($st_cnt, "i", $shop_id);
        mysqli_stmt_execute($st_cnt);
        mysqli_stmt_close($st_cnt);
    }
}

// -----------------------------------------
// 4) 리뷰 DB 삭제
// -----------------------------------------
if ($is_admin) {
    // 관리자 → 제한 없이 삭제
    $sql_del = "DELETE FROM reviews WHERE id = ?";
    $stmt_del = mysqli_prepare($conn, $sql_del);
    mysqli_stmt_bind_param($stmt_del, "i", $review_id);

} else {
    // 일반 사용자 → 본인 리뷰만 삭제 가능
    $sql_del = "DELETE FROM reviews WHERE id = ? AND user_id = ?";
    $stmt_del = mysqli_prepare($conn, $sql_del);
    mysqli_stmt_bind_param($stmt_del, "ii", $review_id, $user_id);
}

if (!$stmt_del) {
    die("리뷰 삭제 오류: " . mysqli_error($conn));
}

$ok = mysqli_stmt_execute($stmt_del);
mysqli_stmt_close($stmt_del);

if (!$ok) {
    die("리뷰 삭제 실패: " . mysqli_error($conn));
}

// -----------------------------------------
// 5) 파일 삭제 (리뷰 사진)
// -----------------------------------------
if (!empty($image_path)) {
    $full_path = __DIR__ . "/" . $image_path;
    if (file_exists($full_path)) {
        @unlink($full_path);
    }
}

// -----------------------------------------
// 6) 삭제 후 이동
// -----------------------------------------
$from = $_GET['from'] ?? '';

if ($from === 'admin') {
    header("Location: admin.php");
    exit;
}

// 기본은 마이페이지로
header("Location: mypage.php");
exit;
