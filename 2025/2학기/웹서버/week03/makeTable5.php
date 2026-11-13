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
    function makeTable($type, $ST, $ET) {
        $str = "";
        $width = 6 * 160;
        if ($type == "B") $width += 160;
        if ($type == "C") $width += 2 * 160;

        $str .= "<table width=\"$width\">\n";
        $str .= "<colgroup>\n";

        for ($i = 0; $i < 5; $i++) $str .= "<col width=\"160\">\n";\
        if ($type == "B") $str .= "<col width=\"160\">\n";
        if ($type == "C") {
            $str .= "<col width=\"160\">\n"; // 일요일
            $str .= "<col width=\"160\">\n"; // 토요일
        }
        $str .= "</colgroup>\n";

        $str .= "<tr>\n";
        $str .= "<th>시간</th><th>월</th><th>화</th><th>수</th><th>목</th><th>금</th>";
        if ($type == "B") $str .= "<th>토</th>";
        if ($type == "C") $str .= "<th>일</th><th>토</th>";
        $str .= "\n</tr>\n";
        
        for ($j = $ST; $j < $ET; $j++) {
            $str .= "<tr>\n";
            $str .= "<td align=\"center\">" . $j . ":00 ~ " . ($j + 1) . ":00</td>";
            for ($i = 0; $i < 5; $i++) $str .= "<td></td>";
            if ($type == "B") $str .= "<td></td>";
            if ($type == "C") {
                $str .= "<td></td>"; // 일요일
                $str .= "<td></td>"; // 토요일
            }
            $str .= "\n</tr>\n";
        }
        $str .= "</table>\n";
        return $str;
    }
    ?>

    <?php
    $type = $_GET['type']  ?? "A";
    $ST   = $_GET['start'] ?? 9;
    $ET   = $_GET['end']   ?? 17;  
    echo makeTable($type, $ST, $ET);
    ?>
</body>
</html>
