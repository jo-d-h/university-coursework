<meta charset="utf-8">
<link rel="StyleSheet" href="./table.css" type="text/css" />

<?php
    $dom = new DOMDocument();
    $dom->load('scores.xml');
    $rows = $dom->getElementsByTagName("Row");

    echo "<table width=600 border=1>";
    echo "<colgroup>";
    for ($i = 0; $i < 6; $i++) echo "<col width=100>";
    echo "</colgroup>";

    echo "<tr align=center>";
    echo "<th>학번</th><th>국어</th><th>영어</th><th>수학</th><th>과학</th><th>역사</th>";
    echo "</tr>";

    $i = 1;
    while ($rows->item($i)) {
        $row = $rows->item($i);
        $cells = $row->getElementsByTagName("Cell");

        echo "<tr align=center>";
        for ($j = 0; $j < 6; $j++) {
            $cell = $cells->item($j);
            if ($j == 0) {
                echo "<td><a href='./makeExcelGraph.php?sn=" . $cell->nodeValue . "' target=_blank>" . $cell->nodeValue . "</a></td>";
            } else {
                echo "<td>" . $cell->nodeValue . "</td>";
            }
        }
        echo "</tr>";
        $i++;
    }

    echo "</table>";
?>
