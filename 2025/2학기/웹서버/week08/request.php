<meta charset="utf-8">
<link rel="stylesheet" href="style.css" type="text/css">

<h2>성적 입력 페이지</h2>

<form action="reply.php" method="post">

    <div>
        <label>학번:</label>
        <input type="text" name="uid" required>
    </div>
    <br>

    <div>
        <label>중간고사(30%):</label>
        <input type="number" name="mid" required>
    </div>
    <br>

    <div>
        <label>출석(10%):</label>
        <input type="number" name="atd" required>
    </div>
    <br>

    <div>
        <label>기말고사(30%):</label>
        <input type="number" name="fin" required>
    </div>
    <br>

    <div>
        <label>과제(30%):</label>
        <input type="number" name="wor" required>
    </div>
    <br>

    <input type="submit" value="성적 저장">
</form>
