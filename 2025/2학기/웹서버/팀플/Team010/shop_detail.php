<?php
date_default_timezone_set('Asia/Seoul');
require_once "db.php";
require_once "auth.php";

function h($s) {
    return htmlspecialchars($s ?? "", ENT_QUOTES | ENT_SUBSTITUTE, "UTF-8");
}

/* ------------------------------
   1. GET 파라미터 체크
------------------------------ */
$shopListNum = isset($_GET["id"]) ? intval($_GET["id"]) : 0;
if ($shopListNum <= 0) {
    die("잘못된 접근입니다. <a href='index.php'>메인으로</a>");
}

/* ------------------------------
   2. 가게 정보 읽기
------------------------------ */
$sql = "SELECT * FROM shops WHERE id = ?";
$stmt = mysqli_prepare($conn, $sql);
mysqli_stmt_bind_param($stmt, "i", $shopListNum);
mysqli_stmt_execute($stmt);
$res  = mysqli_stmt_get_result($stmt);
$shop = mysqli_fetch_assoc($res);
mysqli_stmt_close($stmt);



if (!$shop) {
    die("가게 정보를 찾을 수 없습니다. <a href='index.php'>메인으로</a>");
}

/* ------------------------------
   3. 값 꺼내기
------------------------------ */
$shop_db_id   = (int)$shop["id"];
$shop_name    = $shop["shop_name"] ?? "";
$shop_menu    = $shop["shop_menu"] ?? "";
$shop_loca    = $shop["shop_loca"] ?? "";
$shop_time    = $shop["shop_time"] ?? "";
$shop_callnum = $shop["shop_callnum"] ?? "";
$shop_sns     = $shop["shop_sns"] ?? "";
$shop_alon    = $shop["shop_alon"] ?? "";
$extphoto     = $shop["shop_extphoto"] ?? "";
$intphoto     = $shop["shop_intphoto"] ?? "";
$menuphoto    = $shop["shop_menuphoto"] ?? "";

// 네이버 평점/리뷰 수 (shops 테이블에 저장된 값)
$naver_rat = isset($shop["shop_n_rat"]) ? (float)$shop["shop_n_rat"] : 0.0;
$naver_rev = isset($shop["shop_n_rev"]) ? (int)$shop["shop_n_rev"] : 0;

/* ------------------------------
   4. 영업 상태 계산
------------------------------ */
function get_open_status_for_today(string $shop_time_text): string
{
    $shop_time_text = trim($shop_time_text);
    if ($shop_time_text === '') return '영업시간 정보 없음';

    $dayMap = ['월'=>1,'화'=>2,'수'=>3,'목'=>4,'금'=>5,'토'=>6,'일'=>7];
    $todayN     = (int)date('N');            // 1~7 (월~일)
    $nowMinutes = (int)date('H') * 60 + (int)date('i');

    foreach (explode(',', $shop_time_text) as $seg) {
        $seg = trim($seg);
        if ($seg === '') continue;

        $parts = preg_split('/\s+/', $seg);
        if (count($parts) < 2) continue;

        $dayPart  = array_shift($parts);
        $timePart = preg_replace('/\s+/', '', implode(' ', $parts));

        // 요일 처리
        $inRange = false;
        if (strpos($dayPart, '-') !== false) {
            [$d1, $d2] = array_map('trim', explode('-', $dayPart));
            $dayMapLocal = ['월'=>1,'화'=>2,'수'=>3,'목'=>4,'금'=>5,'토'=>6,'일'=>7];
            $dayStart = $dayMapLocal[$d1] ?? null;
            $dayEnd   = $dayMapLocal[$d2] ?? null;
            if ($dayStart === null || $dayEnd === null) continue;

            if ($dayStart <= $dayEnd) {
                $inRange = ($todayN >= $dayStart && $todayN <= $dayEnd);
            } else {
                // 토-월 같은 순환 범위
                $inRange = ($todayN >= $dayStart || $todayN <= $dayEnd);
            }
        } else {
            $d = $dayMap[$dayPart] ?? null;
            if ($d === null) continue;
            $inRange = ($d === $todayN);
        }

        if (!$inRange) continue;

        // 시간 범위
        if (strpos($timePart, '~') === false) continue;
        [$t1, $t2] = explode('~', $timePart);
        $t1 = trim($t1);
        $t2 = trim($t2);
        if (strlen($t1) !== 5 || strlen($t2) !== 5) continue;

        [$h1, $m1] = array_map('intval', explode(':', $t1));
        [$h2, $m2] = array_map('intval', explode(':', $t2));

        $startMin = $h1 * 60 + $m1;
        $endMin   = $h2 * 60 + $m2;

        if ($nowMinutes < $startMin) {
            return '영업 전';
        } elseif ($nowMinutes > $endMin) {
            return '영업 종료';
        } else {
            return '영업 중';
        }
    }

    return '오늘 휴무';
}

$open_status = get_open_status_for_today($shop_time);

/* ------------------------------
   5. 리뷰 불러오기 (네이버 / 밥도둑 분리)
------------------------------ */

// 5-1) 밥도둑(우리 사이트) 리뷰: reviews + users
$sql_ours = "
    SELECT r.*, u.nickname AS writer
    FROM reviews AS r
    LEFT JOIN users AS u ON r.user_id = u.id
    WHERE (r.shop_id = ? OR r.shop_name = ?)
      AND r.source_id <> 1
    ORDER BY r.id DESC
";
$stmt_ours = mysqli_prepare($conn, $sql_ours);
mysqli_stmt_bind_param($stmt_ours, "is", $shop_db_id, $shop_name);
mysqli_stmt_execute($stmt_ours);
$res_ours = mysqli_stmt_get_result($stmt_ours);

// 5-2) 네이버 리뷰: naver_reviews
$sql_naver = "
    SELECT *
    FROM naver_reviews
    WHERE (shop_id = ? OR shop_name = ?)
    ORDER BY id DESC
";
$stmt_naver = mysqli_prepare($conn, $sql_naver);
mysqli_stmt_bind_param($stmt_naver, "is", $shop_db_id, $shop_name);
mysqli_stmt_execute($stmt_naver);
$res_naver = mysqli_stmt_get_result($stmt_naver);

$reviews        = [];   // 화면에 뿌릴 전체 리뷰(네이버+밥도둑)
$good_counts    = [];   // '어떤 점이 좋아요' 집계
$site_star_sum  = 0;    // 밥도둑 별점 합
$site_star_cnt  = 0;    // 밥도둑 별점 리뷰 개수

// 밥도둑 리뷰 루프
while ($row = mysqli_fetch_assoc($res_ours)) {
    $star = isset($row["star"]) ? (int)$row["star"] : 0;

    if ($star > 0) {
        $site_star_sum += $star;
        $site_star_cnt++;
    }

    if (!empty($row["good_point"])) {
        $g = $row["good_point"];
        if (!isset($good_counts[$g])) $good_counts[$g] = 0;
        $good_counts[$g]++;
    }

    $reviews[] = $row;
}
mysqli_stmt_close($stmt_ours);

// 네이버 리뷰 루프
while ($row = mysqli_fetch_assoc($res_naver)) {
    // 화면용 필드 맞추기
    $row["source_id"] = 1;
    // review_add.php에서 reviewer_nickname을 쓰는 구조와 맞추기 위해
    if (!isset($row["writer"]) || $row["writer"] === null) {
        $row["writer"] = $row["reviewer_nickname"] ?? "네이버 사용자";
    }

    if (!empty($row["good_point"])) {
        $g = $row["good_point"];
        if (!isset($good_counts[$g])) $good_counts[$g] = 0;
        $good_counts[$g]++;
    }

    $reviews[] = $row;
}
mysqli_stmt_close($stmt_naver);

/* ------------------------------
   6. 평점/리뷰 수 계산
   - 네이버 : shops.shop_n_rat / shop_n_rev
   - 밥도둑 : 실제 reviews(우리 리뷰) 기반
   - 종합   : 둘을 합산
------------------------------ */
$naver_has_rating = ($naver_rat > 0 && $naver_rev > 0);

// 밥도둑 평균 (DB 저장용 + 화면용)
$site_avg_for_db = ($site_star_cnt > 0)
    ? ($site_star_sum / $site_star_cnt)
    : 0.0;

// shops 테이블에 밥도둑 평점/리뷰 수 반영
$upd_sql = "
    UPDATE shops
    SET site_avg_star = ?, site_review_cnt = ?
    WHERE id = ?
";
$upd_stmt = mysqli_prepare($conn, $upd_sql);
if ($upd_stmt) {
    mysqli_stmt_bind_param($upd_stmt, "dii", $site_avg_for_db, $site_star_cnt, $shop_db_id);
    mysqli_stmt_execute($upd_stmt);
    mysqli_stmt_close($upd_stmt);
}

// 종합 평점 계산
$combined_avg   = null;
$combined_count = 0;

if ($naver_has_rating && $site_star_cnt > 0) {
    // 네이버 + 밥도둑
    $combined_sum   = $naver_rat * $naver_rev + $site_star_sum;
    $combined_count = $naver_rev + $site_star_cnt;
    $combined_avg   = $combined_sum / $combined_count;
} elseif ($naver_has_rating && $site_star_cnt === 0) {
    // 네이버만
    $combined_avg   = $naver_rat;
    $combined_count = $naver_rev;
} elseif (!$naver_has_rating && $site_star_cnt > 0) {
    // 밥도둑만
    $combined_avg   = $site_star_sum / $site_star_cnt;
    $combined_count = $site_star_cnt;
} else {
    // 둘 다 없음
    $combined_avg   = null;
    $combined_count = 0;
}

// 종합 리뷰 수 / 평점도 shops 테이블에 반영
$combined_cnt_for_db = $combined_count;
$combined_avg_for_db = (!is_null($combined_avg) && $combined_count > 0)
    ? $combined_avg
    : 0.0;

$upd_sql2 = "
    UPDATE shops
    SET combined_review_cnt = ?, combined_avg_star = ?
    WHERE id = ?
";
$upd_stmt2 = mysqli_prepare($conn, $upd_sql2);
if ($upd_stmt2) {
    mysqli_stmt_bind_param(
        $upd_stmt2,
        "idi",                // int, double, int
        $combined_cnt_for_db,
        $combined_avg_for_db,
        $shop_db_id
    );
    mysqli_stmt_execute($upd_stmt2);
    mysqli_stmt_close($upd_stmt2);
}

/* ------------------------------
   7. 리뷰 필터 (전체 / 네이버 / 밥도둑)
------------------------------ */
$review_filter = $_GET["src"] ?? "all";
if ($review_filter !== "naver" && $review_filter !== "site") {
    $review_filter = "all";
}

/* ------------------------------
   8. 혼밥 추천 플래그
------------------------------ */
$alon_recommend = ((int)$shop_alon === 1);
?>
<!DOCTYPE html>
<html lang="ko">
<head>
    <meta charset="UTF-8">
    <title><?php echo h($shop_name); ?></title>

    <link rel="stylesheet" href="shop-detailCss.css">
    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Jua&display=swap" rel="stylesheet">

</head>
<body>

<div class="side-menu" id="side-menu">
        <a href="#" id="back-link">⬅️ 이전 페이지로</a>
        </br>
        <a href="index.php">홈🏠</a>
        </br>
        <a href="ranking.html">랭킹👑</a>
        </br>
        <button class="close-btn" id="close-btn">&times;</button>
    </div>

  <table class="header-table">
        <tr>
            <td class="header-burgerBox">
            <div class="burger-btn" id="burger-btn">
                &#9776; </div>
            </td>
            <td class="header-logoBox">
                <img src="deulogo.png" alt="동의대로고" class="header-logo">
            </td>

            <td class="header-title" onclick="location.href='index.php'">
                <p>밥도둑</p>
            </td>

            <td class="user-menu" id="user-menu">
                 <img src="사용자.png" alt="사용자" class="user-logo"> 
            </td>
        </tr>
    </table>


<!-- ① 상단 사진: 외부 / 내부 -->
 <div class="divbox">
<div class="photos">
    <div class="photo-box">
        <?php if (!empty($extphoto)): ?>
            <img src="<?php echo h($extphoto); ?>" alt="외부 사진">
        <?php endif; ?>
    </div>
    <div class="photo-box">
        <?php if (!empty($intphoto)): ?>
            <img src="<?php echo h($intphoto); ?>" alt="내부 사진">
        <?php endif; ?>
    </div>
</div>

<!-- ② 가게 기본 정보 -->

<div class="shop-info">

<div class="shop-name"><?php echo h($shop_name); ?></div>

<?php if (!empty($shop_menu)): ?>
    <div class="shop-menu"><?php echo h($shop_menu); ?></div>
<?php endif; ?>



<div><span class="label">위치:</span> <?php echo h($shop_loca); ?></div>
<?php if (isset($shop_callnum) && trim($shop_callnum) !== '' && strcasecmp(trim($shop_callnum), 'none') !== 0): ?>
<div>
    <span class="label">전화:</span> <?php echo h($shop_callnum); ?>
</div>
<?php endif; ?>

<?php if ($shop_sns !== '' && strcasecmp($shop_sns, 'none') !== 0): ?>
<div>
    <span class="label">공식 SNS:</span>
    <a href="<?php echo h($shop_sns); ?>" target="_blank"
       style="color:#1e88e5; text-decoration:underline;">
        방문하기
    </a>
</div>
<?php endif; ?>

<div>
    <span class="label">영업시간:</span>
    <?php echo nl2br(h($shop_time)); ?>

    <?php
    $st = $open_status;
    if ($st === "영업 중") {
        echo '<span class="status-open">'.h($st).'</span>';
    } else {
        echo '<span class="status-closed">'.h($st).'</span>';
    }
    ?>
</div>

<div>
    <span class="label">혼밥 추천:</span>
    <?php echo $alon_recommend ? '⭕' : '❌'; ?>
</div>

<?php if (!empty($menuphoto)): ?>
    <div>
        <button class="menu-btn" onclick="openMenu()">메뉴 보기</button>
    </div>
<?php endif; ?>

</div>

<!-- ③ 리뷰 영역 -->
<div class="review-section">

    <div class="test">

    <h2>
    리뷰
    </h2>

    <div class="review-btn">
        <a href="review_add.php?id=<?php echo $shopListNum; ?>&shop_name=<?php echo urlencode($shop_name); ?>">
            리뷰 작성하기
        </a>
    </div>

    </div>
    <!-- 리뷰 필터 -->
    <div class="review-filter">
        <a href="shop_detail.php?id=<?php echo $shopListNum; ?>&src=all"
        class="filter-btn <?php if ($review_filter === 'all') echo 'active'; ?>">
            전체
        </a>
        <a href="shop_detail.php?id=<?php echo $shopListNum; ?>&src=naver"
        class="filter-btn <?php if ($review_filter === 'naver') echo 'active'; ?>">
            네이버
        </a>
        <a href="shop_detail.php?id=<?php echo $shopListNum; ?>&src=site"
        class="filter-btn <?php if ($review_filter === 'site') echo 'active'; ?>">
            밥도둑
        </a>
    </div>

        <!-- 평점 요약 박스 -->
    <div class="rating-box">
        <div><normal>평점 요약</normal></div>

        <!-- 네이버 -->
        <div>
            <span class="label"; style="color : green";>네이버 :</span>
            <?php
            if ($naver_has_rating) {
                // 네이버 평점/리뷰 수가 있을 때
                echo "★ " . number_format($naver_rat, 2) . " (리뷰 {$naver_rev}개)";
            } else {
                // 평점은 없지만, 리뷰 수는 그대로 표시
                echo "평점 정보 없음 (리뷰 {$naver_rev}개)";
            }
            ?>
        </div>

        <!-- 밥도둑 -->
        <div>
            <span class="label" style="color : blue">밥도둑 :</span>
            <?php
            if ($site_star_cnt > 0) {
                echo "★ " . number_format($site_avg_for_db, 2) . " (리뷰 {$site_star_cnt}개)";
            } else {
                echo "평점 정보 없음 (리뷰 {$site_star_cnt}개)";
            }
            ?>
        </div>

        <!-- 종합 -->
        <div>
            <span class="label" style="color : orange">종합 :</span>
            <?php
            if (!is_null($combined_avg) && $combined_count > 0) {
                echo "★ " . number_format($combined_avg, 2) . " (리뷰 {$combined_count}개)";
            } else {
                echo "평점 정보 없음 (리뷰 {$combined_count}개)";
            }
            ?>
        </div>
    </div>

    <div class="review-box">

    <?php if (!empty($good_counts)): ?>
        <div class="good-tags-title">사람들이 이런 점을 좋아했어요</div>
        <div class="good-tags">
            <?php foreach ($good_counts as $g => $c): ?>
                <span class="good-tag">
                    <?php echo h($g); ?>
                    <small><?php echo $c; ?>명</small>
                </span>
            <?php endforeach; ?>
        </div>
    <?php endif; ?>

    <?php if (empty($reviews)): ?>
        <p>아직 리뷰가 없습니다.</p>
    <?php else: ?>

        <!-- 네이버/밥도둑 섹션 나눠서 표시 -->

        <?php if ($review_filter === 'all' || $review_filter === 'naver'): ?>
        <div class="review-section-title">네이버 리뷰</div>
        <?php
        $has_naver = false;
        foreach ($reviews as $r) {
            $source_id = isset($r["source_id"]) ? (int)$r["source_id"] : 0;
            if ($source_id !== 1) continue; // 네이버만
            $has_naver = true;

            $writer = $r["writer"] ?: "네이버 사용자";
            $star   = isset($r["star"]) ? (int)$r["star"] : 0;
        ?>
            <div class="review-item">
                <div>
                    <normal><?php echo h($writer); ?></normal>
                    <?php if (!empty($r["created_at"])): ?>
                        <time>[<?php echo h($r["created_at"]); ?>]</time>
                    <?php endif; ?>
                    <normal style="color : green">· 네이버</normal>
                    <?php if ($star > 0): ?>
                        <normal style="color : orange">· ★</normal><?php echo $star; ?>/5
                    <?php endif; ?>
                </div>

                <?php if (!empty($r["good_point"])): ?>
                    <div>👍 <?php echo h($r["good_point"]); ?></div>
                <?php endif; ?>

                <div><?php echo nl2br(h($r["content"] ?? "")); ?></div>

                <?php if (!empty($r["reviewer_image"])): ?>
                    <img class="review-image"
                         src="<?php echo h($r["reviewer_image"]); ?>"
                         alt="리뷰 이미지">
                <?php endif; ?>
            </div>
        <?php } ?>
        <?php if (!$has_naver): ?>
            <p>네이버 리뷰가 없습니다.</p>
        <?php endif; ?>
        <?php endif; ?>

        <?php if ($review_filter === 'all' || $review_filter === 'site'): ?>
        <div class="review-section-title">밥도둑 리뷰</div>
        <?php
        $has_ours = false;
        foreach ($reviews as $r) {
            $source_id = isset($r["source_id"]) ? (int)$r["source_id"] : 0;
            if ($source_id === 1) continue; // 우리 리뷰만
            $has_ours = true;

            $writer = $r["writer"] ?: "익명";
            $star   = isset($r["star"]) ? (int)$r["star"] : 0;
        ?>
            <div class="review-item">
                <div>
                    <strong><?php echo h($writer); ?></strong>
                    <?php if (!empty($r["created_at"])): ?>
                        <time>[<?php echo h($r["created_at"]); ?>]</time>
                    <?php endif; ?>
                    <normal style="color : blue">· 밥도둑</normal>
                    <?php if ($star > 0): ?>
                        <normal style="color : orange">· ★</normal> <?php echo $star; ?>/5
                    <?php endif; ?>
                </div>

                <?php if (!empty($r["good_point"])): ?>
                    <div>👍 <?php echo h($r["good_point"]); ?></div>
                <?php endif; ?>

                <div><?php echo nl2br(h($r["content"] ?? "")); ?></div>

                <?php if (!empty($r["reviewer_image"])): ?>
                    <img class="review-image"
                         src="<?php echo h($r["reviewer_image"]); ?>"
                         alt="리뷰 이미지">
                <?php endif; ?>
            </div>
        <?php } ?>
        <?php if (!$has_ours): ?>
            <p>아직 밥도둑에 등록된 리뷰가 없습니다.</p>
        <?php endif; ?>
        <?php endif; ?>

    <?php endif; ?>
</div>

<?php if (!empty($menuphoto)): ?>

</div>

<!-- 네이버 스타일 메뉴 팝업 -->
<div id="menu-popup">
    <div class="popup-content">
        <div class="close" onclick="closePopup()">×</div>
        <img src="<?php echo h($menuphoto); ?>" alt="메뉴 이미지">
    </div>
</div>
</div>
<script>
function openMenu() {
    var popup = document.getElementById('menu-popup');
    if (popup) popup.style.display = 'flex';
}
function closePopup() {
    var popup = document.getElementById('menu-popup');
    if (popup) popup.style.display = 'none';
}
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
<?php endif; ?>

  <div class="copyright-section">
        <p>팀원: 20223123 이승환, 20213025 이현승, 20223156 조동현</p>
        <p>프로젝트: 밥도둑</p>
    </div>

</body>
</html>
