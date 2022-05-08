<?php

include ("assignment8inclusions.php");
    try {
    $dsn = "mysql:host=courses;dbname=z1910087";
    

	 $S = $_POST['S'];
	 $SNAME = $_POST['SNAME'];
	 $STATUS = $_POST['STATUS'];
	 $CITY = $_POST['CITY'];
     $pdo = new PDO($dsn, $username, $password);
     $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
	 $qry = "INSERT INTO S (S,SNAME,STATUS,CITY)
	 VALUES ('$S','$SNAME','$STATUS','$CITY')";
     $pdo->exec($qry);
	 echo "Entry Successfull Check everything page";
}
catch(PDOexception $e) { // handle that exception
    echo "Connection to database failed: " . $e->getMessage();
    } 
    
?>