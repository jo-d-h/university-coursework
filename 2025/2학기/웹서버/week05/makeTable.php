<meta charset="UTF-8">
<link rel="stylesheet" href=./table.css type="text/css" />

<?php
    $myfile = fopen("data.txt", "r") or die("Unable to open file!");
    $jsonData = fread($myfile, filesize("data.txt"));
    fclose($myfile);
    $data = json_decode($jsonData, true);
?>

<?php
    echo "<table width=600>";
    echo "<colgroup>";
    for ($i=0; $i<6; $i++) echo "<col width=100>";
    echo "<colgroup>";
    echo "<tr><th>학번</th><th>국어</th><th>영어</th><th>수학</th><th>과학</th><th>역사</th><th>";
    foreach ($data as $k => $v) {
        echo "<tr align=center>";
        echo "<td><a href='./makeGraph.php?sn=" . $k . "' target=_blank>" . $k . "</a></td>";
        foreach ($v as $vd) echo "<td>" . $vd . "</td>";
        echo "</tr>";
    }
      
    /*
    echo $jsonData;
    echo "<hr/>";
    var_dump($data);
    echo "<hr/>";
    echo "<pre>";
    print_r($data);
    echo "</pre>";
    */
?>