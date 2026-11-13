<?php
    $conn = mysqli_connect('localhost', 'root', '');

    if (!$conn) {
        die('Could not connect: ' . mysqli_connect_error());
    }

    if(!mysqli_select_db($conn, '20223156')) {
        die('Could not connect: ' . mysqli_connect_error());
    }

    $sql = "insert into week06 (uid, kor, eng, math, sci, hist) values ('20223156', 85, 86, 87, 88, 89)";
    echo $sql . "<br/>";
    if(!mysqli_query($conn, $sql)) {
        echo "Error inserting data: " . mysqli_error($conn);
    } else {
        echo "data is inserted" . "\n";
    }

    $sci = 83;
    $hist = 84;
    $sql = "insert into week06 (uid, kor, eng, math, sci, hist) values ('20240002', 80, 81, 82, " . $sci . ", " . $hist . ");";
    echo $sql . "<br/>";

    if(!mysqli_query($conn, $sql)) {
        echo "Error inserting data: " . mysqli_error($conn);
    } else {
        echo "data is inserted" . "\n";
    }

    mysqli_close($conn);
?>