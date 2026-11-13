<?php
// review_add.php : 가게 상세에서 들어와 DB에 리뷰 저장

require_once "auth.php";
require_once "db.php";

require_login();  // 로그인 필수

// "어떤 점이 좋아요" 선택지 목록
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

// 1. 어떤 가게에 대한 리뷰인지: id(shops.id) 기준으로 받음
if ($_SERVER["REQUEST_METHOD"] === "POST") {
    // 폼에서 hidden으로 넘어온 값
    $shop_id = intval($_POST["shop_id"] ?? 0);
} else {
    // GET으로 들어올 때: id = shops.id
    $shop_id = intval($_GET["id"] ?? 0);
}

if ($shop_id <= 0) {
    die("잘못된 접근입니다. <a href='index.php'>메인으로</a>");
}

// 2. id 로 shops에서 실제 PK(id)와 이름 가져오기
$sql = "SELECT id, shop_name FROM shops WHERE id = ?";
$stmt = mysqli_prepare($conn, $sql);
if (!$stmt) {
    die("가게 조회 오류: " . mysqli_error($conn));
}
mysqli_stmt_bind_param($stmt, "i", $shop_id);
mysqli_stmt_execute($stmt);
$result   = mysqli_stmt_get_result($stmt);
$shop_row = mysqli_fetch_assoc($result);
mysqli_stmt_close($stmt);

if (!$shop_row) {
    die("존재하지 않는 가게입니다. <a href='index.php'>메인으로</a>");
}

$shop_db_id = (int)$shop_row["id"];    // shops 테이블 PK
$shop_name  = $shop_row["shop_name"];  // 가게 이름 (표시용)

// 3. 폼 전송 처리 (리뷰 저장)
if ($_SERVER["REQUEST_METHOD"] === "POST") {
    $content = trim($_POST["review_content"] ?? "");
    $good    = trim($_POST["review_good"] ?? "");
    $star    = (int)($_POST["review_star"] ?? 0);

    if ($content === "" || $good === "") {
        $err = "어떤 점이 좋은지, 내용은 필수입니다.";
    } elseif (!in_array($good, $good_options, true)) {
        $err = "유효하지 않은 '어떤 점이 좋아요' 값입니다.";
    } else {
        if ($star < 0) $star = 0;
        if ($star > 5) $star = 5;

        $user_id = $_SESSION["user_id"] ?? 0;
        if ($user_id <= 0) {
            $err = "로그인 정보가 없습니다. 다시 로그인 해 주세요.";
        } else {

            // -----------------------------
            // ① 이미지 업로드 처리
            // -----------------------------
            $saved_path = "";  // DB에 넣을 reviewer_image 값 (상대경로)

            if (isset($_FILES["review_image_file"]) &&
                $_FILES["review_image_file"]["error"] !== UPLOAD_ERR_NO_FILE) {

                $file = $_FILES["review_image_file"];

                if ($file["error"] === UPLOAD_ERR_OK) {

                    // 확장자만 간단히 체크 (jpg/png/gif/webp 정도 허용)
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
                            // DB에는 웹에서 접근 가능한 상대 경로 저장
                            $saved_path = "uploads/reviews/" . $new_name;
                        } else {
                            $err = "이미지 업로드 중 오류가 발생했습니다.";
                        }
                    }
                } else {
                    $err = "이미지 업로드 중 오류가 발생했습니다. (코드: " . $file["error"] . ")";
                }
            }

            // 업로드에서 에러가 없을 때만 INSERT 실행
            if ($err === "") {

                // 세션에서 닉네임 가져와서 reviewer_nickname에도 저장
                $reviewer_nickname = $_SESSION["nickname"] ?? $_SESSION["username"] ?? "익명";

                // 우리 사이트 작성 리뷰 → source_id = 2 (상수로 고정)
                $sql = "INSERT INTO reviews
                            (shop_id, shop_name, user_id, source_id,
                             reviewer_nickname, reviewer_image,
                             content, good_point, star, created_at)
                        VALUES (?, ?, ?, 2, ?, ?, ?, ?, ?, NOW())";

                $stmt = mysqli_prepare($conn, $sql);
                if (!$stmt) {
                    $err = "리뷰 저장 준비 중 오류가 발생했습니다: " . mysqli_error($conn);
                } else {
                    mysqli_stmt_bind_param(
                        $stmt,
                        "isissssi",    // i s i s s s s i  (총 8개)
                        $shop_db_id,        // i - shop_id (shops.id)
                        $shop_name,         // s - shop_name
                        $user_id,           // i - user_id
                        $reviewer_nickname, // s - reviewer_nickname
                        $saved_path,        // s - reviewer_image
                        $content,           // s - content
                        $good,              // s - good_point
                        $star               // i - star
                    );

                    if (!mysqli_stmt_execute($stmt)) {
                        $err = "리뷰 저장 중 오류가 발생했습니다: " . mysqli_stmt_error($stmt);
                    } else {
                        mysqli_stmt_close($stmt);

                        // ★ 혼밥하기 좋아요 선택 시 카운트 +1
                        if ($good === '혼밥하기 좋아요') {
                            $upd_sql = "UPDATE shops
                                        SET solo_good_cnt = solo_good_cnt + 1
                                        WHERE id = ?";
                            $upd_stmt = mysqli_prepare($conn, $upd_sql);
                            if ($upd_stmt) {
                                mysqli_stmt_bind_param($upd_stmt, "i", $shop_db_id);
                                mysqli_stmt_execute($upd_stmt);
                                mysqli_stmt_close($upd_stmt);
                            }
                        }

                        // 저장 완료 후 가게 상세로 이동
                        header("Location: shop_detail.php?id=" . $shop_id);
                        exit();
                    }
                    // 실패했을 때는 위에서 $err 설정되고, 폼 아래에 에러 메세지 출력됨
                }
            }
        }
    }
}
?>
<!DOCTYPE html>
<html lang="ko">
<head>
    <meta charset="UTF-8">
    <title>리뷰 작성</title>
    <link rel="stylesheet" href="reviewCss.css">
    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Jua&display=swap" rel="stylesheet">
</head>
<body>
    <div class="review-box">
    <p class="title">리뷰 작성⭐</p>

    <?php if ($err !== ""): ?>
        <p style="color:red;"><?php echo h($err); ?></p>
    <?php endif; ?>

    <form method="post" action="review_add.php" enctype="multipart/form-data">
        <!-- 이 가게가 어떤 가게인지: shops.id -->
        <input type="hidden" name="shop_id" value="<?php echo $shop_id; ?>">

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
                <?php
                $selected_good = $_POST["review_good"] ?? "";
                foreach ($good_options as $opt):
                ?>
                    <option value="<?php echo h($opt); ?>"
                        <?php if ($opt === $selected_good) echo "selected"; ?>>
                        <?php echo h($opt); ?>
                    </option>
                <?php endforeach; ?>
            </select>
        </p>

        <p>
            별점:
            <?php $selected_star = $_POST["review_star"] ?? "0"; ?>
            <div id="star-rating" class="star-rating" data-current-rating="<?php echo h($selected_star); ?>">
                <span class="star" data-value="1">★</span>
                <span class="star" data-value="2">★</span>
                <span class="star" data-value="3">★</span>
                <span class="star" data-value="4">★</span>
                <span class="star" data-value="5">★</span>
            </div>
            <input type="hidden" name="review_star" id="review_star_input" value="<?php echo h($selected_star); ?>">
        </p>

        <p>
            사진 업로드 (선택):<br>
            <input type="file" name="review_image_file" accept="image/*">
        </p>

        <p>
            내용:<br>
            <textarea name="review_content" rows="5" cols="50"><?php
                echo h($_POST["review_content"] ?? "");
            ?></textarea>
        </p>

        <p>
            <button type="submit">저장</button>
        </p>
    </form>

    <p class="link">
        <a href="shop_detail.php?id=<?php echo $shop_id; ?>">가게 상세로 돌아가기 </a>|
        <a href="mypage.php">마이페이지로</a>
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
