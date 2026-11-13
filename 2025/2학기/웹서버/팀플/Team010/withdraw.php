<?php
require_once "auth.php";
require_once "db.php";

require_login();

$user_id = $_SESSION["user_id"];

$sql = "DELETE FROM users WHERE id = ?";
$stmt = mysqli_prepare($conn, $sql);
mysqli_stmt_bind_param($stmt, "i", $user_id);
mysqli_stmt_execute($stmt);
mysqli_stmt_close($stmt);

$_SESSION = array();
session_destroy();

header("Location: index.php");
exit();
