<meta charset="utf-8">
<link rel="StyleSheet" href="./table.css" type="text/css" />
<?php include("./data.php"); ?>

<?php
function makeTimeTableStudent($data, $grade) {
    $time = array('', '(09:00 ~ 09:50)', '(10:00 ~ 10:50)', '(11:00 ~ 11:50)',
                  '(12:00 ~ 12:50)', '(13:00 ~ 13:50)', '(14:00 ~ 14:50)',
                  '(15:00 ~ 15:50)', '(16:00 ~ 16:50)', '(17:00 ~ 17:50)');
    $day = array("월", "화", "수", "목", "금");
    $str = "<table width='" . (180*6) . "px'>";

    //요일
    $str .= "<tr style='background-color:lightblue;'>";
    $str .= "<th width='100'>교시</th>";
    foreach ($day as $d) {
        $str .= "<th width='180'>$d</th>";
    }
    $str .= "</tr>";

    //연강 체크 초기화
    $skip = array_fill(1, 5, 0);

    //교시
    for ($i = 1; $i <= 9; $i++) {
        $str .= "<tr>";
        $str .= "<td bgcolor='lightyellow' align='center'>$i 교시<br/>" . $time[$i] . "</td>";

        for ($j = 1; $j <= 5; $j++) {
            if ($skip[$j] > 0) {
                $skip[$j]--;
                continue;
            }

            $found = false;

            if (isset($data[$j][$i])) {
                foreach ($data[$j][$i] as $subj) {
                    if ($grade === 0 || ($subj[0] !== "" && $subj[0] == $grade) || $subj[0] === "") {
                        $rowspan = isset($subj[3]) ? $subj[3] : 1;
                        $str .= "<td rowspan='$rowspan' align='center'>";
                        $str .= $subj[1] . "<br/>";
                        if (isset($subj[2])) $str .= $subj[2] . " 교수<br/>";
                        if (isset($subj[4])) $str .= "강의실: " . $subj[4] . "<br/>";
                        $str .= "</td>";
                        $skip[$j] = $rowspan - 1;
                        $found = true;
                        break;
                    }
                }
            }

            if (!$found) $str .= "<td></td>";
        }

        $str .= "</tr>";
    }

    $str .= "</table>";
    return $str;
}

$sn = 20223156;

if (isset($_GET['sn']) && isset($personal[$_GET['sn']])) {
    $sn = $_GET['sn'];
    echo makeTimeTableStudent($personal[$sn], 0);
} else if (isset($_GET['grade'])) {
    $grade = intval($_GET['grade']);
    echo makeTimeTableStudent($class, $grade);
}

?>
