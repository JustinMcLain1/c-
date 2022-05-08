<?php

include ("assignment8inclusions.php");
    try {
    $dsn = "mysql:host=courses;dbname=z1910087";
        

        
	 $S = $_POST['S'];
	 $P = $_POST['P'];
	 $QTY1 = $_POST['QTY1'];
     $pdo = new PDO($dsn, $username, $password);
     $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
     $QTY = 'QTY';
     
     //$QTY = "SELECT COUNT(QTY) FROM SP WHERE S = '$S' and p = '$P'";

     $qry = "UPDATE SP SET QTY = $QTY - $QTY1 WHERE S = '$S' AND P= '$P' ";
     $pdo->prepare($qry);
     $pdo->exec($qry);
	 echo "Entry Sucessful Check Everything Page";
}
catch(PDOexception $e) { // handle that exception
    echo "Connection to database failed: " . $e->getMessage();
    } 
    
?>