<meta charset="utf-8">
<link rel="stylesheet" href="style.css" type="text/css">

<?php
$uid = $_POST['uid'];
$mid = $_POST['mid'];
$atd = $_POST['atd'];
$fin = $_POST['fin'];
$wor = $_POST['wor'];

// 항목별 최대점수, 이름을 배열로 정의
$limits = [
    'mid' => ['name' => '중간고사', 'max' => 30],
    'fin' => ['name' => '기말고사', 'max' => 30],
    'atd' => ['name' => '출석', 'max' => 10],
    'wor' => ['name' => '과제', 'max' => 30]
];

//검사
foreach ($limits as $key => $info) {
    $name = $info['name'];
    $max = $info['max'];
    if ($$key < 0 || $$key > $max) {
        echo "<h2>입력값 오류</h2>";
        echo "<p>{$name}는 0에서 {$max} 사이의 값이어야 합니다.</p>";
        echo "<a href='request.php'>다시 입력하기</a>";
        exit();
    }
}

$conn = mysqli_connect('localhost', 'root', '', '20223156');

if (!$conn) {
    die('Could not connect: ' . mysqli_connect_error());
}

$sql = "INSERT INTO week08 (uid, mid, atd, fin, wor) 
        VALUES ('$uid', $mid, $atd, $fin, $wor)
        ON DUPLICATE KEY UPDATE
        mid=$mid, atd=$atd, fin=$fin, wor=$wor";

if (mysqli_query($conn, $sql)) {
    echo "<h2>성적이 성공적으로 저장되었습니다.</h2>";
    echo "<p>학번: $uid</p>";
    echo "<p>중간고사(30%): $mid</p>";
    echo "<p>출석(10%): $atd</p>";
    echo "<p>기말고사(30%): $fin</p>";
    echo "<p>과제(30%): $wor</p>";
} else {
    echo "Error: " . $sql . "<br>" . mysqli_error($conn);
}

mysqli_close($conn);

echo "<a href='request.php'>다른 성적 입력하기</a>";
?>