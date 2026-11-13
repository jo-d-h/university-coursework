<meta charset="UTF-8">
<link rel="stylesheet" href=./table.css type="text/css" />

<?php
    $sn = $_GET['sn'];
    $myfile = fopen("data.txt", "r") or die("Unable to open file!");
    $jsonData = fread($myfile, filesize("data.txt"));
    fclose($myfile);
    $data = json_decode($jsonData, true);
    $subject = array("국어", "영어", "수학", "과학", "역사");
?>

<?php
    $avg = array();
    $max = array();
    $min = array();
    $n = count($data);
    foreach ($subject as $s) $avg[$s] = 0;
    foreach ($subject as $s) $max[$s] = 51;
    foreach ($subject as $s) $min[$s] = 100;
    foreach ($data as $d) {
    foreach ($d as $k => $v) {
            $avg[$k] += $v;
            if ($v>$max[$k]) $max[$k] = $v;
            if ($v<$min[$k]) $min[$k] = $v;
        }
    }
    foreach ($subject as $s) $avg[$s] = (int) ($avg[$s] / $n);
?>

<script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
    <script type="text/javascript">
      google.charts.load('current', {'packages':['bar']});
      google.charts.setOnLoadCallback(drawChart);

    function drawChart() {
        var data = google.visualization.arrayToDataTable([
        ['과목', '최대', '평균', '점수', '최소']
        <?php
            foreach ($data[$sn] as $k => $v) {
                echo ",['" . $k . "', ";
                echo $max[$k] . ", ";
                echo $avg[$k] . ", ";
                echo $v . ", ";
                echo $min[$k] . "]" . "\n";
            }
        ?>

        ]);
        var options = {
            chart: {
                title: '성적평가',
                subtitle: '학생 <?=$sn?> 의 성적',
            }
        };
        var chart = new google.charts.Bar(document.getElementById('columnchart_material'));
        chart.draw(data, google.charts.Bar.convertOptions(options));
    }
</script>
<div id="columnchart_material" style="width: 800px; height: 500px;"></div>