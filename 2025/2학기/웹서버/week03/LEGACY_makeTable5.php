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
        $str .= "<table border=\"1\">\n";

        $str .= "<colgroup>\n";
        $str .= "<col width=\"160\">\n";
        if ($type == "A") {
            for ($i = 0; $i < 5; $i++) $str .= "<col width=\"160\">\n";
        }
        else if ($type == "B") {
            for ($i = 0; $i < 6; $i++) $str .= "<col width=\"160\">\n";
        }
        else if ($type == "C") {
            for ($i = 0; $i < 7; $i++) $str .= "<col width=\"160\">\n";
        }
        $str .= "</colgroup>\n";

        $str .= "<tr>\n";
        if ($type == "A") {
            $str .= "<th>시간</th><th>월</th><th>화</th><th>수</th><th>목</th><th>금</th>";
        }
        else if ($type == "B") {
            $str .= "<th>시간</th><th>월</th><th>화</th><th>수</th><th>목</th><th>금</th><th>토</th>";
        }
        else if ($type == "C") {
            $str .= "<th>시간</th><th>일</th><th>월</th><th>화</th><th>수</th><th>목</th><th>금</th><th>토</th>";
        }
        $str .= "\n</tr>\n";

        for ($j = $ST; $j < $ET; $j++) {
            $str .= "<tr>\n";
            $str .= "<td align=\"center\">{$j}:00 ~ " . ($j + 1) . ":00</td>";

            if ($type == "A") {
                for ($i = 0; $i < 5; $i++) $str .= "<td></td>";
            }
            else if ($type == "B") {
                for ($i = 0; $i < 6; $i++) $str .= "<td></td>";
            }
            else if ($type == "C") {
                for ($i = 0; $i < 7; $i++) $str .= "<td></td>";
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
