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
    $type = isset($_POST['type']) ? $_POST['type'] : "A";
    $ST   = isset($_POST['start']) ? $_POST['start'] : 9;
    $ET   = isset($_POST['end']) ? $_POST['end'] : 17;

    $width = 6*160;
    if ($type == "B") $width += 160;

    echo "<table width=" . $width . ">";
    echo "<colgroup>";
    for ($i=0; $i<5; $i++) echo "<col width=160>";
    if ($type == "B") echo "<col width=160>";
    echo "</colgroup>";

    echo "<tr>";
    echo "<th>시간</th><th>월</th><th>화</th><th>수</th><th>목</th><th>금</th>";
    if ($type == "B") echo "<th>토</th>";
    echo "</tr>";
    for ($j=$ST; $j<$ET; $j++) {
        echo "<tr>";
        echo "<td align=center>" . $j . ":00 ~ " . ($j+1) . ":00</td>";
        for ($i=0; $i<5; $i++) echo "<td></td>";
        if ($type == "B") echo "<td></td>";
        echo "</tr>";
    }
    echo "</table>";
    ?>
</body>
</html>
