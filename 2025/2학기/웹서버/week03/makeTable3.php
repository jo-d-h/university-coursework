<!DOCTYPE html>
<html lang="en">

    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <link rel="StyleSheet" href="./table.css" type="text/css" />
        <title>Document</title>
    </head>

    <body>
        <?php
        // 테이블의 한칸은 160px로 한다.
        $type = isset($_GET['type']) ? $_GET['type'] : "A"; // 기본값 "A"
        $ST = isset($_GET['start']) ? intval($_GET['start']) : 9; // 기본값 9
        $ET = isset($_GET['end']) ? intval($_GET['end']) : 17; // 기본값 17

        $width = 6 * 160;
        if ($type == "B") $width += 160;

        echo "<table width=\"$width\">";
        echo "<colgroup>";
        for ($i = 0; $i < 6; $i++) echo "<col width=\"160\">";
        if ($type == "B") echo "<col width=\"160\">";
        echo "</colgroup>";

        echo "<tr>";
        echo "<th>시간</th><th>월</th><th>화</th><th>수</th><th>목</th><th>금</th>";
        if ($type == "B") echo "<th>토</th>";
        echo "</tr>";

        for ($j = $ST; $j < $ET; $j++) {
            echo "<tr>";
            echo "<td align=\"center\">" . sprintf("%02d:00 ~ %02d:00", $j, $j + 1) . "</td>";
            for ($i = 0; $i < 5; $i++) echo "<td></td>";
            if ($type == "B") echo "<td></td>";
            echo "</tr>";
        }
        echo "</table>";
        ?>
    </body>
</html>
