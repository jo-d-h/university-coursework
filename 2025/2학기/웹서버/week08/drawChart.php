<?php
// MySQL 연결
$conn = mysqli_connect('localhost', 'root', '', '20223156');
if (!$conn) {
    die('Could not connect: ' . mysqli_connect_error());
}

// week08 테이블 데이터 조회
$sql = "SELECT * FROM week08";
$result = mysqli_query($conn, $sql);
if (!$result) {
    die('Error fetching data: ' . mysqli_error($conn));
}

// 성적 등급 개수
$grades = [
    'A' => 0,
    'B' => 0,
    'C' => 0,
    'D' => 0,
    'F' => 0
];

// 점수 계산 후 등급 판별, 카운트
while ($row = mysqli_fetch_assoc($result)) {
    $total = $row['mid'] + $row['atd'] + $row['fin'] + $row['wor'];

    if ($total >= 90) $grades['A']++;
    else if ($total >= 80) $grades['B']++;
    else if ($total >= 70) $grades['C']++;
    else if ($total >= 60) $grades['D']++;
    else $grades['F']++;
}

mysqli_close($conn);
?>

<!DOCTYPE html>
<html lang="ko">
<head>
    <meta charset="UTF-8">
    <title>성적 분포 파이차트</title>
    <script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
    <script type="text/javascript">
        // Google Charts 로드
        google.charts.load('current', {'packages':['corechart']});
        google.charts.setOnLoadCallback(drawChart);

        function drawChart() {
            // PHP의 $grades 배열을 JS로 전달
            var phpGrades = <?php echo json_encode($grades); ?>;

            // 데이터 테이블 생성
            var data = new google.visualization.DataTable();
            data.addColumn('string', '등급');
            data.addColumn('number', '학생 수');

            for (var grade in phpGrades) {
                data.addRow([grade, phpGrades[grade]]);
            }

            // 옵션
            var options = {
                title: '성적 분포 (A~F)',
                pieHole: 0.4, // 도넛 모양
                width: 900,
                height: 500
            };

            // 차트 그리기
            var chart = new google.visualization.PieChart(document.getElementById('piechart'));
            chart.draw(data, options);
        }
    </script>
</head>
<body>
    <h1>성적 분포 차트</h1>
    <div id="piechart"></div>
</body>
</html>
