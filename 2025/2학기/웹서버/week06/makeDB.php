<?php
    $conn = mysqli_connect('localhost', 'root', ''); // 마지막 매개변수는 비밀번호
    if (!$conn) die('Could not connect: ' . mysqli_connect_error());

    $sql = "CREATE DATABASE IF NOT EXISTS `20223156`";

    if(!mysqli_query($conn, $sql)) {
        echo "Error creating database: " . mysqli_error($conn);
    } 
    else {
        echo "Database `20223156` is created" . "\n";
    }

    mysqli_close($conn);
?>