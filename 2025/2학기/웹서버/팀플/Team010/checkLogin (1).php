<?php
session_start();
$response = ['loggedIn' => isset($_SESSION['user_id'])];
header('Content-Type: application/json');
echo json_encode($response);
?>