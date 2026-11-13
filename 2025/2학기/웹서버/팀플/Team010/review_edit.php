<?php
// review_edit.php : 내가 쓴 리뷰 수정 (DB 전용)

require_once "auth.php";
require_once "db.php";

require_login();  // 로그인 필수

// "어떤 점이 좋아요" 선택지 목록 (review_add.php와 동일하게 유지)
$good_options = [
    "음식이 맛있어요",
    "커피가 맛있어요",
    "친절해요",
    "매장이 넓어요",
    "음료가 맛있어요",
    "혼밥하기 좋아요",
    "가성비가 좋아요",
    "매장이 청결해요",
    "디저트가 맛있어요",
    "재료가 신선해요",
    "대화하기 좋아요",
    "인테리어가 멋져요",
    "양이 많아요",
    "좌석이 편해요",
    "화장실이 깨끗해요",
    "조용한 분위기에요",
    "아이들과 오기 좋아요",
    "사진이 잘 나와요",
    "단체 모임하기 좋아요",
    "음악이 좋아요",
    "뷰가 좋아요",
    "새로 생긴 가게에요",
    "직원들이 전문적이에요",
    "재방문 의사가 있어요",
    "메뉴 설명이 친절해요",
    "대기 시간이 짧아요",
    "예약이 편리해요",
    "포장 서비스가 좋아요",
    "배달 서비스가 좋아요",
    "가격이 합리적이에요",
    "분위기가 아늑해요",
    "청결 관리가 잘 되어있어요",
    "테이블 간격이 넓어요",
    "조명이 분위기 있어요",
    "음식이 깔끔하게 나와요",
    "직원들이 빠르게 대응해요",
    "사장님이 친절해요",
    "지역 맛집이에요",
    "특별한 날 가기 좋아요",
    "건강한 맛이에요",
    "포장이 깔끔해요",
    "집중하기 좋아요",
    "음식이 빨리 나와요",
    "메뉴 구성이 알차요",
];

if (!function_exists('h')) {
    function h($s) {
        return htmlspecialchars($s ?? "", ENT_QUOTES | ENT_SUBSTITUTE, "UTF-8");
    }
}

$err = "";

// 1. 어떤 리뷰를 수정할지: GET 또는 POST로 넘어온 id 사용
//    - 마이페이지에서는 review_edit.php?idx=리뷰ID 로 링크됨
$review_id = 0;
if ($_SERVER["REQUEST_METHOD"] === "POST") {
    $review_id = intval($_POST["review_id"] ?? 0);
} else {
    $review_id = intval($_GET["idx"] ?? ($_GET["id"] ?? 0));
}

if ($review_id <= 0) {
    die("잘못된 접근입니다. <a href='mypage.php'>마이페이지로</a>");
}

$user_id = $_SESSION["user_id"] ?? 0;
if ($user_id <= 0) {
    die("로그인 정보가 없습니다. <a href='login.php'>로그인</a>");
}

// 2. DB에서 리뷰 정보 읽어오기 (본인 리뷰만 수정 가능)
//    → 이제는 shops와 JOIN해서 list_num을 가져올 필요 없음
$sql = "
    SELECT *
    FROM reviews
    WHERE id = ? AND user_id = ?
";
$stmt = mysqli_prepare($conn, $sql);
if (!$stmt) {
    die("리뷰 조회 오류: " . mysqli_error($conn));
}
mysqli_stmt_bind_param($stmt, "ii", $review_id, $user_id);
mysqli_stmt_execute($stmt);
$result = mysqli_stmt_get_result($stmt);
$review_row = mysqli_fetch_assoc($result);
mysqli_stmt_close($stmt);

if (!$review_row) {
    die("존재하지 않거나 수정 권한이 없는 리뷰입니다. <a href='mypage.php'>마이페이지로</a>");
}

// 기존 값들 (reviews 테이블 기준)
// shop_id 는 reviews.shop_id (→ shops.id)
$shop_id        = (int)$review_row["shop_id"];
$shop_name      = $review_row["shop_name"] ?? "";
$old_good       = $review_row["good_point"] ?? "";
$old_star       = isset($review_row["star"]) ? (int)$review_row["star"] : 0;
$old_content    = $review_row["content"] ?? "";
$old_image_path = $review_row["reviewer_image"] ?? "";

// 3. POST로 수정 처리
if ($_SERVER["REQUEST_METHOD"] === "POST") {

    $new_good    = trim($_POST["review_good"] ?? "");
    $new_content = trim($_POST["review_content"] ?? "");
    $new_star    = (int)($_POST["review_star"] ?? 0);

    if ($new_good === "" || $new_content === "") {
        $err = "어떤 점이 좋은지, 내용은 필수입니다.";
    } elseif (!in_array($new_good, $good_options, true)) {
        $err = "유효하지 않은 '어떤 점이 좋아요' 값입니다.";
    } else {
        if ($new_star < 0) $new_star = 0;
        if ($new_star > 5) $new_star = 5;

        // -----------------------------
        // ① 이미지 업로드 처리 (선택)
        // -----------------------------
        $saved_path = $old_image_path;  // 기본은 기존 경로 유지

        if (isset($_FILES["review_image_file"]) &&
            $_FILES["review_image_file"]["error"] !== UPLOAD_ERR_NO_FILE) {

            $file = $_FILES["review_image_file"];

            if ($file["error"] === UPLOAD_ERR_OK) {

                // 확장자 체크
                $ext = strtolower(pathinfo($file["name"], PATHINFO_EXTENSION));
                $allowed_ext = ["jpg","jpeg","png","gif","webp"];

                if (!in_array($ext, $allowed_ext, true)) {
                    $err = "이미지 파일(jpg, png, gif, webp)만 업로드할 수 있습니다.";
                } else {
                    $upload_dir = __DIR__ . "/uploads/reviews";

                    if (!is_dir($upload_dir)) {
                        mkdir($upload_dir, 0777, true);
                    }

                    // 파일명: rev_유저ID_타임스탬프.ext
                    $new_name  = "rev_" . $user_id . "_" . time() . "." . $ext;
                    $dest_path = $upload_dir . "/" . $new_name;

                    if (move_uploaded_file($file["tmp_name"], $dest_path)) {
                        // 기존 이미지 파일 삭제(선택)
                        if (!empty($old_image_path)) {
                            $old_full = __DIR__ . "/" . $old_image_path;
                            if (file_exists($old_full)) {
                                @unlink($old_full);
                            }
                        }
                        $saved_path = "uploads/reviews/" . $new_name;
                    } else {
                        $err = "이미지 업로드 중 오류가 발생했습니다.";
                    }
                }
            } else {
                $err = "이미지 업로드 중 오류가 발생했습니다. (코드: " . $file["error"] . ")";
            }
        }

        // 업로드 에러가 있으면 UPDATE 진행 안 함
        if ($err === "") {

            // ② 리뷰 내용 DB 업데이트
            $upd_sql = "
                UPDATE reviews
                SET good_point = ?, content = ?, star = ?, reviewer_image = ?
                WHERE id = ? AND user_id = ?
            ";
            $upd_stmt = mysqli_prepare($conn, $upd_sql);
            if (!$upd_stmt) {
                $err = "리뷰 수정 준비 중 오류가 발생했습니다: " . mysqli_error($conn);
            } else {
                mysqli_stmt_bind_param(
                    $upd_stmt,
                    "ssissi",
                    $new_good,
                    $new_content,
                    $new_star,
                    $saved_path,
                    $review_id,
                    $user_id
                );

                if (!mysqli_stmt_execute($upd_stmt)) {
                    $err = "리뷰 수정 중 오류가 발생했습니다: " . mysqli_stmt_error($upd_stmt);
                }
                mysqli_stmt_close($upd_stmt);
            }

            // ③ 혼밥하기 좋아요 카운트 조정 (shops.solo_good_cnt)
            if ($err === "") {
                if ($old_good !== "혼밥하기 좋아요" && $new_good === "혼밥하기 좋아요") {
                    // 새로 혼밥으로 바뀜 → +1
                    $sql_cnt = "UPDATE shops
                                SET solo_good_cnt = solo_good_cnt + 1
                                WHERE id = ?";
                    $st_cnt = mysqli_prepare($conn, $sql_cnt);
                    if ($st_cnt) {
                        mysqli_stmt_bind_param($st_cnt, "i", $shop_id);
                        mysqli_stmt_execute($st_cnt);
                        mysqli_stmt_close($st_cnt);
                    }
                } elseif ($old_good === "혼밥하기 좋아요" && $new_good !== "혼밥하기 좋아요") {
                    // 예전엔 혼밥이었는데 지금은 아님 → -1 (음수 방지)
                    $sql_cnt = "UPDATE shops
                                SET solo_good_cnt = CASE
                                    WHEN solo_good_cnt > 0 THEN solo_good_cnt - 1
                                    ELSE 0
                                END
                                WHERE id = ?";
                    $st_cnt = mysqli_prepare($conn, $sql_cnt);
                    if ($st_cnt) {
                        mysqli_stmt_bind_param($st_cnt, "i", $shop_id);
                        mysqli_stmt_execute($st_cnt);
                        mysqli_stmt_close($st_cnt);
                    }
                }
            }

            // 모든 게 정상이면 마이페이지로 이동
            if ($err === "") {
                header("Location: mypage.php");
                exit();
            }

            // 에러가 생겼다면, 폼에 다시 뿌려줄 값들 갱신
            $old_good       = $new_good;
            $old_star       = $new_star;
            $old_content    = $new_content;
            $old_image_path = $saved_path;
        }
    }
}
?>
<!DOCTYPE html>
<html lang="ko">
<head>
    <meta charset="UTF-8">
    <title>리뷰 수정</title>
    <link rel="stylesheet" href="reviewCss.css">
    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Jua&display=swap" rel="stylesheet">
</head>
<body>
    <div class="review-box">
    <h1 class="title">리뷰 수정✏️</h1>

    <?php if ($err !== ""): ?>
        <p style="color:red;"><?php echo h($err); ?></p>
    <?php endif; ?>

    <form method="post" action="review_edit.php" enctype="multipart/form-data">
        <input type="hidden" name="review_id" value="<?php echo $review_id; ?>">

        <p>
            가게 이름:<br>
            <input
                type="text"
                value="<?php echo h($shop_name); ?>"
                readonly
                style="
                    background:#f1f3f5;
                    border:1px solid #ccc;
                    padding:6px;
                    width:280px;
                    font-weight:bold;
                    cursor:not-allowed;
                "
            >
        </p>

        <p>
            어떤 점이 좋아요?<br>
            <select name="review_good">
                <?php foreach ($good_options as $opt): ?>
                    <option value="<?php echo h($opt); ?>"
                        <?php if ($opt === $old_good) echo "selected"; ?>>
                        <?php echo h($opt); ?>
                    </option>
                <?php endforeach; ?>
            </select>
        </p>

        <p>
            별점:
            <div id="star-rating" class="star-rating" data-current-rating="<?php echo h($old_star); ?>">
                <span class="star" data-value="1">★</span>
                <span class="star" data-value="2">★</span>
                <span class="star" data-value="3">★</span>
                <span class="star" data-value="4">★</span>
                <span class="star" data-value="5">★</span>
            </div>
            <input type="hidden" name="review_star" id="review_star_input" value="<?php echo h($old_star); ?>">
        </p>

        <p>
            현재 사진:<br>
            <?php if (!empty($old_image_path)): ?>
                <img src="<?php echo h($old_image_path); ?>" alt="리뷰 이미지"
                     style="max-width:200px; max-height:200px; border:1px solid #ccc;">
            <?php else: ?>
                <span>등록된 사진 없음</span>
            <?php endif; ?>
        </p>

        <p>
            사진 변경 (선택):<br>
            <input type="file" name="review_image_file" accept="image/*">
        </p>

        <p>
            내용:<br>
            <textarea name="review_content" rows="5" cols="50"><?php
                echo h($old_content);
            ?></textarea>
        </p>

        <p>
            <button type="submit">저장</button>
        </p>
    </form>

    <p class="link">
        <a href="mypage.php">마이페이지로</a>
        <?php if ($shop_id > 0): ?>
            | <a href="shop_detail.php?id=<?php echo $shop_id; ?>">가게 상세로</a>
        <?php endif; ?>
    </p>

    </div>
    <script>
    document.addEventListener('DOMContentLoaded', function() {
        const starContainer = document.getElementById('star-rating');
        const stars = starContainer.querySelectorAll('.star');
        const hiddenInput = document.getElementById('review_star_input');

        let currentRating = parseInt(starContainer.dataset.currentRating);
        updateStars(currentRating);

        starContainer.addEventListener('mouseover', function(e) {
            if (e.target.classList.contains('star')) {
                const hoverValue = parseInt(e.target.dataset.value);
                updateStars(hoverValue, true);
            }
        });

        starContainer.addEventListener('mouseout', function() {
            updateStars(currentRating);
        });

        starContainer.addEventListener('click', function(e) {
            if (e.target.classList.contains('star')) {
                const clickValue = parseInt(e.target.dataset.value);
                
                if (currentRating === clickValue) {
                    currentRating = 0;
                } else {
                    currentRating = clickValue;
                }
                
                hiddenInput.value = currentRating;
                updateStars(currentRating);
                starContainer.dataset.currentRating = currentRating;
            }
        });

        function updateStars(rating, isPreview = false) {
            stars.forEach(star => {
                const starValue = parseInt(star.dataset.value);
                if (starValue <= rating) {
                    star.classList.add('selected');
                } else {
                    star.classList.remove('selected');
                }
                star.style.cursor = 'pointer'; 
            });
        }
    });
    </script>
</body>
</html>
