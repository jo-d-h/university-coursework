<?php
    $conn = mysqli_connect('localhost', 'root', '');
    if (!$conn) {
        die('Could not connect: ' . mysqli_connect_error());
    }
    if(!mysqli_select_db($conn, '20223156')) {
        die('Can\'t use database: ' . mysqli_error($conn));
    }

    $sql = "SELECT * FROM week08";
    $result = mysqli_query($conn, $sql);
    while ($row = mysqli_fetch_array($result)) {
        echo $row['uid'] . " ";
        echo $row['mid'] . " ";
        echo $row['atd'] . " ";
        echo $row['fin'] . " ";
        echo $row['wor'] . "<br/>";
    }

    mysqli_close($conn);
?>