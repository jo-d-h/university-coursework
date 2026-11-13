<?php
header("Content-Type: application/json; charset=UTF-8");

$conn = new mysqli("localhost", "root", "", "Team010");
$conn->set_charset("utf8mb4");

// 맛집랭킹
$bestSql = "SELECT id, shop_name, shop_loca, shop_keyword, shop_extphoto, combined_avg_star, shop_menu
            FROM shops
            ORDER BY combined_avg_star DESC, shop_name COLLATE utf8mb4_unicode_ci ASC";
$bestResult = $conn->query($bestSql);
$bestShops = [];
while ($row = $bestResult->fetch_assoc()) {
    // ⭐️ 이 부분이 핵심입니다: PHP에서 평점을 소수점 2자리 문자열로 고정합니다.
    $row['combined_avg_star'] = number_format((float)$row['combined_avg_star'], 2, '.', '');
    $bestShops[] = $row;
}

// 혼밥랭킹
$soloSql = "SELECT id, shop_name, shop_loca, shop_keyword, shop_extphoto, solo_good_cnt, shop_menu
            FROM shops
            ORDER BY solo_good_cnt DESC, shop_name COLLATE utf8mb4_unicode_ci ASC";
$soloResult = $conn->query($soloSql);
$soloShops = [];
while ($row = $soloResult->fetch_assoc()) {
    $soloShops[] = $row;
}

echo json_encode([
  "best" => $bestShops,
  "solo" => $soloShops
], JSON_UNESCAPED_UNICODE | JSON_UNESCAPED_SLASHES);
?>