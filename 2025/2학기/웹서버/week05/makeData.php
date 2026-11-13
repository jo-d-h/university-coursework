<meta charset = "UTF-8">
<link rel="stylesheet" href=./table.css type="text/css" />

<?php
    $subject = array("국어", "영어", "수학", "과학", "역사");
    $data = array();
    for($i=20240001; $i<20240010; $i++) {
        $data[$i] = array();
        foreach ($subject as $s) {
            $data[$i][$s] = rand(51, 100);
        }
    }

    $jsonData = json_encode($data);
    $myfile = fopen("data.txt", "w") or die("Unable to open file!");
    fwrite($myfile, $jsonData);
    fclose($myfile);

    echo "<p>■ var_dump(\$data)</p>";
    var_dump($data);
    echo "<hr/>";
    echo "<p>■ &lt;pre&gt; &lt;?php print_r(\$data); ?&gt; &lt;/pre&gt;</p>";
    echo "<pre>";
    print_r($data);
    echo "</pre>";
    echo "<p>■ echo \$jsonData</p>";
    echo $jsonData;
    echo "<hr/>";
?>