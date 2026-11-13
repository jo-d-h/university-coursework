<?php
    $conn = mysqli_connect('localhost', 'root', '');
    if (!$conn) 
        die('Could not connect: ' . mysqli_connect_error());

    if (!mysqli_select_db($conn, '20223156'))
        die('Could not connect: ' . mysqli_connect_error());

    // week08 테이블 생성
    $sql = "CREATE TABLE IF NOT EXISTS week08 (
        uid VARCHAR(20) NOT NULL, /*학번*/
        mid INT NOT NULL, /*중간고사 30퍼*/
        atd INT NOT NULL, /*출석 10퍼*/
        fin INT NOT NULL, /*기말고사 30퍼*/
        wor INT NOT NULL, /*과제 30퍼*/
        PRIMARY KEY (uid)
    )";

    if(!mysqli_query($conn, $sql)) 
        echo "Error creating table `week08`: " . mysqli_error($conn) . "<br/>";
    else 
        echo "Table `week08` is created<br/>";

    mysqli_close($conn);
?>
