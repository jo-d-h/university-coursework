
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
        $type = "A"; // A: 월~금, B: 월~토
        $ST = 9; // 시작 시간
        $ET = 17; // 끝 시간

        $width = 6 * 160;
        if ($type == "B") $width += 160;

        echo "<table width=" . $width . ">" . "\n";
        echo "<colgroup>";
        for ($i = 0; $i < 5; $i++) echo "<col width=160>";
        if ($type == "B") echo "<col width=160>";
        echo "</colgroup>" . "\n";

        echo "<tr>";
        echo "<th>시간</th><th>월</th><th>화</th><th>수</th><th>목</th><th>금</th>";
        if ($type == "B") echo "<th>토</th>";
        echo "</tr>" . "\n";
        for ($j = $ST; $j < $ET; $j++) {
            echo "<tr>";
            echo "<td align=center>" . $j . ":00 ~ " . ($j + 1) . ":00</td>";
            // sprintf를 이용하면 줄을 맞출 수 있음.
            for ($i = 0; $i < 5; $i++) echo "<td></td>";
            if ($type == "B") echo "<td></td>";
            echo "</tr>" . "\n";
        }
        echo "</table>";
        ?>
    </body>
</html>
