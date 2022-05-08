<?php

include ("assignment8inclusions.php");
    try {
    $dsn = "mysql:host=courses;dbname=z1910087";
    

	 $P = $_POST['P'];
	 $PNAME = $_POST['PNAME'];
	 $COLOR = $_POST['COLOR'];
	 $WEIGHT = $_POST['WEIGHT'];
     $pdo = new PDO($dsn, $username, $password);
     $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
	 $qry = "INSERT INTO P (P,PNAME,COLOR,WEIGHT)
	 VALUES ('$P','$PNAME','$COLOR','$WEIGHT')";
     $pdo->exec($qry);
	 echo "Entry Sucessful Check Everything Page";
}
catch(PDOexception $e) { // handle that exception
    echo "Connection to database failed: " . $e->getMessage();
    } 
    
?>