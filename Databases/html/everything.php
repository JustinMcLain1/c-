<?php
include ("assignment8inclusions.php");
try { // if something goes wrong, an exception is thrown
    $dsn = "mysql:host=courses;dbname=z1910087";
    $pdo = new PDO($dsn, $username, $password);
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

    $rs = $pdo->prepare("SELECT * FROM P;");
    if(!$rs) { echo"Error in query"; die(); } // rs returns false if fail
    $rs->execute();

    
        $rows = $rs->fetchAll(PDO::FETCH_ASSOC);
        drawTable($rows);

$rs = $pdo->prepare("SELECT * FROM SP;");
if(!$rs) { echo"Error in query"; die(); } // rs returns false if fail
$rs->execute();

$rows = $rs->fetchAll(PDO::FETCH_ASSOC);
drawTable($rows);


$rs = $pdo->prepare("SELECT * FROM S;");
if(!$rs) { echo"Error in query"; die(); } // rs returns false if fail
$rs->execute();

$rows = $rs->fetchAll(PDO::FETCH_ASSOC);
drawTable($rows);

}
catch(PDOexception $e) { // handle that exception
    echo "Connection to database failed: " . $e->getMessage();
    }


    
?>
