<?php
// admin.php (DB 전용 관리자 페이지)
// - 회원 목록 관리 (강제 탈퇴 포함)
// - 리뷰 목록 관리 (출처/별점/좋았던 점/내용 확인)
// XML(review.xml, shoplist.xml)은 사용하지 않음.

require_once "auth.php";
require_once "db.php";

require_admin();   // 관리자가 아니면 index.php로 리다이렉트

// HTML 출력용 헬퍼
function h($s) {
    return htmlspecialchars($s ?? "", ENT_QUOTES | ENT_SUBSTITUTE, "UTF-8");
}

/* =====================================================
   ①-2. 관리자 권한 부여 / 해제
      (?make_admin=ID) 또는 (?remove_admin=ID)
   ===================================================== */
if (isset($_GET["make_admin"]) || isset($_GET["remove_admin"])) {
    $target_id = isset($_GET["make_admin"])
        ? (int)$_GET["make_admin"]
        : (int)$_GET["remove_admin"];

    $do_grant = isset($_GET["make_admin"]);  // true면 부여, false면 해제

    // 1) 사용자 정보 확인
    $sql = "SELECT id, username, is_admin FROM users WHERE id = ?";
    $stmt = mysqli_prepare($conn, $sql);
    if ($stmt === false) {
        die("사용자 조회 쿼리 준비 실패: " . mysqli_error($conn));
    }
    mysqli_stmt_bind_param($stmt, "i", $target_id);
    mysqli_stmt_execute($stmt);
    $res  = mysqli_stmt_get_result($stmt);
    $info = mysqli_fetch_assoc($res);
    mysqli_stmt_close($stmt);

    if (!$info) {
        die("해당 사용자를 찾을 수 없습니다.");
    }

    if ($do_grant) {
        // 관리자 권한 부여
        if ((int)$info["is_admin"] === 1) {
            header("Location: admin.php");
            exit();
        }

        $stmt_up = mysqli_prepare($conn,
            "UPDATE users SET is_admin = 1 WHERE id = ?"
        );
        if ($stmt_up) {
            mysqli_stmt_bind_param($stmt_up, "i", $target_id);
            mysqli_stmt_execute($stmt_up);
            mysqli_stmt_close($stmt_up);
        }
    } else {
        // 관리자 해제
        if ((int)$info["is_admin"] === 0) {
            header("Location: admin.php");
            exit();
        }

        // 최소 1명은 관리자로 남겨두기
        $cnt_res = mysqli_query(
            $conn,
            "SELECT COUNT(*) AS c FROM users WHERE is_admin = 1"
        );
        $cnt_row = $cnt_res ? mysqli_fetch_assoc($cnt_res) : null;
        $admin_count = $cnt_row ? (int)$cnt_row["c"] : 0;

        if ($admin_count <= 1) {
            die("마지막 관리자는 관리자 해제할 수 없습니다.");
        }

        $stmt_up = mysqli_prepare($conn,
            "UPDATE users SET is_admin = 0 WHERE id = ?"
        );
        if ($stmt_up) {
            mysqli_stmt_bind_param($stmt_up, "i", $target_id);
            mysqli_stmt_execute($stmt_up);
            mysqli_stmt_close($stmt_up);
        }
    }

    // 처리 후 목록으로 리다이렉트
    header("Location: admin.php");
    exit();
}

/* =====================================================
   ①-3. 특정 회원 강제 탈퇴 처리 (?delete_user=ID)
        - users에서 삭제 + 그 사람이 쓴 reviews도 삭제
   ===================================================== */
if (isset($_GET["delete_user"])) {
    $user_id = (int)$_GET["delete_user"];

    // 사용자 정보 확인 (관리자인지 확인)
    $sql = "SELECT id, username, is_admin FROM users WHERE id = ?";
    $stmt = mysqli_prepare($conn, $sql);
    if ($stmt === false) {
        die("사용자 조회 쿼리 준비 실패: " . mysqli_error($conn));
    }
    mysqli_stmt_bind_param($stmt, "i", $user_id);
    mysqli_stmt_execute($stmt);
    $res = mysqli_stmt_get_result($stmt);
    $info = mysqli_fetch_assoc($res);
    mysqli_stmt_close($stmt);

    if (!$info) {
        die("해당 사용자를 찾을 수 없습니다.");
    }

    if ((int)$info["is_admin"] === 1) {
        die("관리자는 삭제할 수 없습니다.");
    }

    // 이 사용자가 쓴 리뷰 먼저 삭제
    $stmt_del_rev = mysqli_prepare($conn, "DELETE FROM reviews WHERE user_id = ?");
    if ($stmt_del_rev) {
        mysqli_stmt_bind_param($stmt_del_rev, "i", $user_id);
        mysqli_stmt_execute($stmt_del_rev);
        mysqli_stmt_close($stmt_del_rev);
    }

    // 사용자 삭제
    $stmt_del_user = mysqli_prepare($conn, "DELETE FROM users WHERE id = ?");
    if ($stmt_del_user) {
        mysqli_stmt_bind_param($stmt_del_user, "i", $user_id);
        mysqli_stmt_execute($stmt_del_user);
        mysqli_stmt_close($stmt_del_user);
    }

    // GET 파라미터 제거 위해 리다이렉트
    header("Location: admin.php");
    exit();
}

/* =====================================================
   ①-4. 리뷰 필터용 GET 파라미터 + 가게 목록
   ===================================================== */

// 필터 값 (가게명, 출처)
$filter_shop   = trim($_GET['shop'] ?? '');
$filter_source = $_GET['source'] ?? 'all';
if (!in_array($filter_source, ['all', 'naver', 'site'], true)) {
    $filter_source = 'all';
}

// 드롭다운에 쓸 가게 목록 (shops 테이블에서 가져오기)
$shop_options = [];
$sql_shop_options = "SELECT DISTINCT shop_name FROM shops ORDER BY shop_name ASC";
$res_shop_options = mysqli_query($conn, $sql_shop_options);
if ($res_shop_options) {
    while ($row = mysqli_fetch_assoc($res_shop_options)) {
        $name = $row['shop_name'] ?? '';
        if ($name !== '') {
            $shop_options[] = $name;
        }
    }
    mysqli_free_result($res_shop_options);
}

/* =====================================================
   ② 전체 회원 목록 불러오기
   ===================================================== */
$sql_users = "
    SELECT id, username, email, nickname, is_admin, created_at
    FROM users
    ORDER BY id ASC
";
$users_result = mysqli_query($conn, $sql_users);
if ($users_result === false) {
    die("회원 목록 조회 실패: " . mysqli_error($conn));
}

/* =====================================================
   ③ 전체 리뷰 목록 불러오기 (DB에서 직접)
      - reviews + review_sources JOIN + 필터
   ===================================================== */

$conditions = [];

// 1) 가게 이름 필터 (드롭다운에서 고른 이름과 정확히 일치)
if ($filter_shop !== '') {
    $shop_escaped = mysqli_real_escape_string($conn, $filter_shop);
    $conditions[] = "r.shop_name = '{$shop_escaped}'";
}

// 2) 출처 필터 (네이버 / 밥도둑)
if ($filter_source === 'naver') {
    // review_sources.code = 'naver'
    $conditions[] = "s.code = 'naver'";
} elseif ($filter_source === 'site') {
    // 우리 사이트 리뷰는 code = 'local'
    $conditions[] = "s.code = 'local'";
}

$sql_reviews = "
    SELECT
        r.id,
        r.shop_name,
        r.reviewer_nickname,
        r.content,
        r.good_point,
        r.star,
        r.created_at,
        r.user_id,
        r.reviewer_image,
        s.name_ko AS source_name,
        s.code    AS source_code,
        u.nickname AS user_nickname
    FROM reviews AS r
    LEFT JOIN review_sources AS s
        ON r.source_id = s.id
    LEFT JOIN users AS u
        ON r.user_id = u.id
";

if (!empty($conditions)) {
    $sql_reviews .= " WHERE " . implode(" AND ", $conditions);
}

$sql_reviews .= " ORDER BY r.id DESC";

$reviews_result = mysqli_query($conn, $sql_reviews);
if ($reviews_result === false) {
    die("리뷰 목록 조회 실패: " . mysqli_error($conn));
}

$all_reviews = [];
while ($row = mysqli_fetch_assoc($reviews_result)) {
    $all_reviews[] = $row;
}
mysqli_free_result($reviews_result);

?>
<!DOCTYPE html>
<html lang="ko">
<head>
    <meta charset="UTF-8">
    <title>관리자 페이지 (DB 전용)</title>

    <link rel="stylesheet" href="adminCSS.css">

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

<div class="adbox">
<p class="title">관리자 페이지</p>
<p>관리자 전용 페이지입니다. 회원 관리 및 리뷰 관리를 할 수 있습니다.</p>

<hr>

<!-- =========================
     1. 회원 목록
     ========================= -->

<div class="list-box">
<p class="list">회원 목록</p>

<table>
    <tr>
        <th>ID</th>
        <th>아이디</th>
        <th>닉네임</th>
        <th>이메일</th>
        <th>가입일</th>
        <th>권한</th>
        <th>관리자 설정</th>
        <th>강제 탈퇴</th>
    </tr>
    <?php while ($row = mysqli_fetch_assoc($users_result)): ?>
        <tr>
            <td><?php echo (int)$row["id"]; ?></td>
            <td><?php echo h($row["username"]); ?></td>
            <td><?php echo h($row["nickname"]); ?></td>
            <td><?php echo h($row["email"]); ?></td>
            <td><?php echo h($row["created_at"]); ?></td>
            <td>
                <?php if ((int)$row["is_admin"] === 1): ?>
                    <span class="badge-admin">관리자</span>
                <?php else: ?>
                    <span class="badge-user">일반</span>
                <?php endif; ?>
            </td>

            <!-- 권한 변경 버튼 -->
            <td>
                <?php if ((int)$row["is_admin"] === 0): ?>
                    <a class="btn-link"
                       href="admin.php?make_admin=<?php echo (int)$row['id']; ?>"
                       onclick="return confirm('이 사용자를 관리자 권한으로 승격하시겠습니까?');">
                        관리자 부여
                    </a>
                <?php else: ?>
                    <a class="btn-link"
                       href="admin.php?remove_admin=<?php echo (int)$row['id']; ?>"
                       onclick="return confirm('이 사용자의 관리자 권한을 해제하시겠습니까?');">
                        관리자 해제
                    </a>
                <?php endif; ?>
            </td>

            <!-- 강제 탈퇴 -->
            <td>
                <?php if ((int)$row["is_admin"] === 0): ?>
                    <a class="btn-danger"
                       href="admin.php?delete_user=<?php echo (int)$row['id']; ?>"
                       onclick="return confirm('정말 이 사용자를 강제 탈퇴시키겠습니까?');">
                        탈퇴
                    </a>
                <?php else: ?>
                    (관리자)
                <?php endif; ?>
            </td>

        </tr>
    <?php endwhile; ?>
</table>

                </div>
<hr>

<!-- =========================
     DB 백업 기능
     ========================= -->
<div class="list-box">
  <p class="list">DB 관리</p>
  <p>현재 DB를 백업하거나 SQL 파일을 DB에 업데이트할 수 있습니다.</p>
  <a href="backup.php" class="btn-link"
     onclick="return confirm('DB 백업/업데이트 페이지로 이동하시겠습니까?');">
     DB 백업하기
  </a>
</div>

<hr>

<!-- =========================
     2. 리뷰 목록
     ========================= -->

<div class = "list-box">

<p class="list">리뷰 목록</p>

<!-- 리뷰 필터 폼 -->
<form method="get"
      style="margin-bottom:12px; padding:8px; background:#f8f9fa; border-radius:8px;">
    <div style="display:flex; flex-wrap:wrap; gap:8px; align-items:center;">
        <div>
            <label>
                가게명:
                <select name="shop" style="padding:4px 6px;">
                    <option value="">전체 가게</option>
                    <?php foreach ($shop_options as $name): ?>
                        <option value="<?php echo h($name); ?>"
                            <?php if ($filter_shop === $name) echo 'selected'; ?>>
                            <?php echo h($name); ?>
                        </option>
                    <?php endforeach; ?>
                </select>
            </label>
        </div>
        <div>
            <label>
                작성 사이트:
                <select name="source" style="padding:4px 6px;">
                    <option value="all"   <?php if ($filter_source === 'all')   echo 'selected'; ?>>전체</option>
                    <option value="naver" <?php if ($filter_source === 'naver') echo 'selected'; ?>>네이버</option>
                    <option value="site"  <?php if ($filter_source === 'site')  echo 'selected'; ?>>밥도둑</option>
                </select>
            </label>
        </div>
        <div>
            <button type="submit" class="btn-link">필터 적용</button>
            <a href="admin.php" class="btn-link">초기화</a>
        </div>
    </div>
</form>

<?php if (empty($all_reviews)): ?>
    <p>등록된 리뷰가 없습니다.</p>
<?php else: ?>
    <ul>
        <?php foreach ($all_reviews as $r): ?>
            <div class="review-box">
                <!-- 출처(네이버 / 우리 사이트 등) 뱃지 -->
                <?php
                    $src_name = $r["source_name"] ?: "출처 정보 없음";
                ?>
                <span class="badge-src"><?php echo h($src_name); ?></span>

                <!-- 가게 이름 -->
                <normal><?php echo h($r["shop_name"]); ?></normal>

                <!-- ★ 별점 (0점은 표시 X) -->
                <?php
                    $star = (int)$r["star"];
                    if ($star > 0 && $star <= 5):
                        $filled = str_repeat("★", $star);
                        $empty  = str_repeat("☆", 5 - $star);
                ?>
                    <span class="star-text">
                        <?php echo $filled . $empty; ?> (<?php echo $star; ?>/5점)
                    </span>
                <?php endif; ?>

                <!-- 좋았던 점 -->
                <?php if (!empty($r["good_point"])): ?>
                    · 👍 <?php echo h($r["good_point"]); ?>
                <?php endif; ?>

                <br>

                <!-- 내용 -->
                <?php echo nl2br(h($r["content"])); ?>

                <!-- 리뷰 이미지 (우리 사이트 리뷰일 때 있을 수 있음) -->
                <?php if (!empty($r["reviewer_image"])): ?>
                    <br>
                    <img src="<?php echo h($r["reviewer_image"]); ?>"
                         alt="리뷰 이미지"
                         style="max-width:150px; max-height:150px; margin-top:4px; border:1px solid #ccc; border-radius:8px;">
                <?php endif; ?>

                <br>

                <!-- 작성자 / 작성시간 -->
                <small>
                    <?php
                        $writer = $r["reviewer_nickname"];   // 네이버 리뷰일 때
                        if (empty($writer)) {
                            $writer = $r["user_nickname"];  // 우리 사이트 리뷰라면 users.nickname
                        }
                    ?>
                    작성자: <?php echo h($writer); ?>
                </small>

                <!-- 리뷰 삭제 (기존 review_delete.php 재사용) -->
                · <a class="btn-delete"
                     href="review_delete.php?idx=<?php echo (int)$r['id']; ?>&from=admin"
                     onclick="return confirm('이 리뷰를 삭제하시겠습니까?');">
                    삭제
                </a>
                    </div>
        <?php endforeach; ?>
    </ul>
<?php endif; ?>

                    </div>
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
