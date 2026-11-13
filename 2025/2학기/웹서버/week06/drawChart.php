<?php
// MySQL 연결
$conn = mysqli_connect('localhost', 'root', '', '20223156');
if (!$conn) {
    die('Could not connect: ' . mysqli_connect_error());
}

// week06 테이블 데이터 조회
$sql = "SELECT * FROM week06";
$result = mysqli_query($conn, $sql);
if (!$result) {
    die('Error fetching data: ' . mysqli_error($conn));
}

// PHP 배열로 변환
$data = [];
while ($row = mysqli_fetch_assoc($result)) {
    $data[] = [
        'uid' => $row['uid'],
        'kor' => (int)$row['kor'],
        'eng' => (int)$row['eng'],
        'math' => (int)$row['math'],
        'sci' => (int)$row['sci'],
        'hist' => (int)$row['hist']
    ];
}

mysqli_close($conn);
?>

<!DOCTYPE html>
<html lang="ko">
<head>
    <meta charset="UTF-8">
    <title>학생 성적 테이블</title>
    <script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
    <script type="text/javascript">
        // Google Charts 로드
        google.charts.load('current', {'packages':['table']});
        google.charts.setOnLoadCallback(drawTable);

        function drawTable() {
            var dataTable = new google.visualization.DataTable();

            // 컬럼 정의
            dataTable.addColumn('string', '학번');
            dataTable.addColumn('number', '국어');
            dataTable.addColumn('number', '영어');
            dataTable.addColumn('number', '수학');
            dataTable.addColumn('number', '과학');
            dataTable.addColumn('number', '역사');

            // PHP 배열을 JSON으로 변환 후 JS에서 사용
            var phpData = <?php echo json_encode($data); ?>;

            // 행 추가
            phpData.forEach(function(row) {
                dataTable.addRow([row.uid, row.kor, row.eng, row.math, row.sci, row.hist]);
            });

            // 테이블 그리기
            var table = new google.visualization.Table(document.getElementById('table_div'));
            table.draw(dataTable, {showRowNumber: true, width: '100%', height: '100%'});
        }
    </script>
</head>
<body>
    <h1>학생 성적 테이블</h1>
    <div id="table_div"></div>
</body>
</html>
