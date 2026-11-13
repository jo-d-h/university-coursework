<meta charset="UTF-8">
<link rel="stylesheet" href="./table.css" type="text/css" />

<?php
    $sn = $_GET['sn'];


    $dom = new DOMDocument();
    $dom->load('scores.xml');
    $rows = $dom->getElementsByTagName("Row");

    $subject = array("국어", "영어", "수학", "과학", "역사");

    $data = array();
    $n = $rows->length - 1;
    
    for ($i = 1; $i < $rows->length; $i++) {
        $row = $rows->item($i);
        $cells = $row->getElementsByTagName("Cell");
        $id = $cells->item(0)->nodeValue;
        $data[$id] = array();
        for ($j = 0; $j < count($subject); $j++) {
            $data[$id][$subject[$j]] = (int)$cells->item($j+1)->nodeValue;
        }
    }

    $avg = array();
    $max = array();
    $min = array();
    foreach ($subject as $s) { 
        $avg[$s] = 0; 
        $max[$s] = 0; 
        $min[$s] = 100; 
    }

    foreach ($data as $d) {
        foreach ($d as $k => $v) {
            $avg[$k] += $v;
            if ($v > $max[$k]) $max[$k] = $v;
            if ($v < $min[$k]) $min[$k] = $v;
        }
    }
    foreach ($subject as $s) $avg[$s] = (int)($avg[$s] / $n);
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
            echo ",['" . $k . "', " . $max[$k] . ", " . $avg[$k] . ", " . $v . ", " . $min[$k] . "]";
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
