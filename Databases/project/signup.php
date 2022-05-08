
<?php
 include("design.php");

 /**************************************
*	CSCI 466 - Group Project
*   "Dogs"
*	Fareed Ali Agha (Z1911794)
*	Eric Baumbach (Z1912129)
*	Justin McLain (Z1910087)
*	Gavin St. George (Z1909350)
*	Dale Westberg (Z1910234)
**************************************/

//The first queue is a free for all, first come, 
//first served queue that can be entered without charge.
//this is where you enter the song and singerID

//The other queue is an accelerated, priority queue, where the 
//user can pay money to potentially have his song played earlier

    try {
    $dsn = "mysql:host=courses;dbname=z1910087";
     
    //select from table
    $rs = $pdo->prepare("SELECT * FROM Singer,Song,File;");
    if(!$rs) { echo"Error in query"; die(); } // rs returns false if fail
    $rs->execute();
    $rows = $rs->fetchAll(PDO::FETCH_ASSOC);
    
    //variables defined
     $SeqNo = $_POST['SeqNo'];   
	 $Singer = $_POST['SingerID'];
     $fileID = $_POST['fileID'];
     
     //insert into queue
     $pdo = new PDO($dsn, $username, $password);
     $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
	 $qry = "INSERT INTO SingMainQueue (SeqNo,singerID,fileID)
	 VALUES ('$SeqNo','$Singer','$fileID')";
     $pdo->exec($qry);

    

}
catch(PDOexception $e) { // handle that exception
    echo "Connection to database failed: " . $e->getMessage();
    } 
    

?>
