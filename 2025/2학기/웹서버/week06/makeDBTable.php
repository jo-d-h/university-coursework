<?php
    $conn = mysqli_connect('localhost', 'root', '');
    if (!$conn) 
        die('Could not connect: ' . mysqli_connect_error());

    if (!mysqli_select_db($conn, '20223156'))
        die('Could not connect: ' . mysqli_connect_error());

    // week06 테이블 생성
    $sql = "CREATE TABLE IF NOT EXISTS week06 (
        uid VARCHAR(20) NOT NULL,
        kor INT NOT NULL,
        eng INT NOT NULL,
        math INT NOT NULL,
        sci INT NOT NULL,
        hist INT NOT NULL,
        PRIMARY KEY (uid)
    )";

    if(!mysqli_query($conn, $sql)) 
        echo "Error creating table `week06`: " . mysqli_error($conn) . "<br/>";
    else 
        echo "Table `week06` is created<br/>";

    mysqli_close($conn);
?>
