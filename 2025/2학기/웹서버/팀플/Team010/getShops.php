<?php
header("Content-Type: application/json; charset=UTF-8");

$location = $_GET['location'] ?? null;
$keyword  = $_GET['keyword'] ?? null;
$search   = $_GET['search'] ?? null;

$conn = new mysqli("localhost", "root", "", "Team010");
$conn->set_charset("utf8");

if ($search) {
    // 🔍 검색어 기반 (shop_name 또는 shop_keyword)
    $sql = "SELECT id, list_num, shop_name, shop_menu, shop_keyword, shop_loca, shop_time,
                   shop_callnum, shop_sns, shop_extphoto, combined_avg_star, combined_review_cnt
            FROM shops
            WHERE shop_name LIKE CONCAT('%', ?, '%')
               OR shop_keyword LIKE CONCAT('%', ?, '%')";
    $stmt = $conn->prepare($sql);
    $stmt->bind_param("ss", $search, $search);
    $stmt->execute();
    $result = $stmt->get_result();

} else if ($keyword && $location) {
    // 📍 장소 + 키워드 복합 검색
    $sql = "SELECT id, list_num, shop_name, shop_menu, shop_keyword, shop_loca, shop_time,
                   shop_callnum, shop_sns, shop_extphoto, combined_avg_star, combined_review_cnt
            FROM shops
            WHERE shop_keyword LIKE CONCAT('%', ?, '%')
              AND shop_loca LIKE CONCAT('%', ?, '%')";
    $stmt = $conn->prepare($sql);
    $stmt->bind_param("ss", $keyword, $location);
    $stmt->execute();
    $result = $stmt->get_result();

} else if ($keyword) {
    // 🏷️ 키워드만 있을 경우 (쉼표로 여러 개 처리)
    $keywords = explode(',', $keyword);
    $conditions = [];
    $types = str_repeat('s', count($keywords));
    foreach ($keywords as $kw) {
        $conditions[] = "shop_keyword LIKE CONCAT('%', ?, '%')";
    }
    $sql = "SELECT id, list_num, shop_name, shop_menu, shop_keyword, shop_loca, shop_time,
                   shop_callnum, shop_sns, shop_extphoto, combined_avg_star, combined_review_cnt
            FROM shops
            WHERE " . implode(' OR ', $conditions);
    $stmt = $conn->prepare($sql);
    $stmt->bind_param($types, ...$keywords);
    $stmt->execute();
    $result = $stmt->get_result();

} else if ($location === '기타') {
    // 📍 기타 장소
    $sql = "SELECT id, list_num, shop_name, shop_menu, shop_keyword, shop_loca, shop_time,
                   shop_callnum, shop_sns, shop_extphoto, combined_avg_star, combined_review_cnt
            FROM shops
            WHERE shop_loca NOT LIKE '%수덕전%'
              AND shop_loca NOT LIKE '%지천관%'
              AND shop_loca NOT LIKE '%행복기숙사%'
              AND shop_loca NOT LIKE '%국제관%'";
    $result = $conn->query($sql);

} else if ($location) {
    // 📍 장소만 있을 경우
    $sql = "SELECT id, list_num, shop_name, shop_menu, shop_keyword, shop_loca, shop_time,
                   shop_callnum, shop_sns, shop_extphoto, combined_avg_star, combined_review_cnt
            FROM shops
            WHERE shop_loca LIKE CONCAT('%', ?, '%')";
    $stmt = $conn->prepare($sql);
    $stmt->bind_param("s", $location);
    $stmt->execute();
    $result = $stmt->get_result();

} else {
    // 📋 아무 조건도 없을 경우 전체 출력
    $result = $conn->query("SELECT * FROM shops");
}

$shops = [];
while ($row = $result->fetch_assoc()) {
    $row['combined_avg_star'] = number_format((float)$row['combined_avg_star'], 2, '.', '');
    $shops[] = $row;
}

echo json_encode($shops, JSON_UNESCAPED_UNICODE | JSON_UNESCAPED_SLASHES);
?>