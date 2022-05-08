<?php
include ("assignment8inclusions.php");
echo "<br>";
    try { // if something goes wrong, an exception is thrown
        $dsn = "mysql:host=courses;dbname=z1910087";
        $pdo = new PDO($dsn, $username, $password);
        $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);


// this is for the S table
           /// this code outputs the seller and what parts they have and the quantity and displays the supplier info as well
$rs = $pdo->prepare("SELECT * FROM SP where S = ?;");
if(!$rs) { echo"Error in query"; die(); } // rs returns false if fail
$rs->execute(array($_POST["S"]));

$rows = $rs->fetchAll(PDO::FETCH_ASSOC);
drawTable($rows);

$rs = $pdo->prepare("SELECT * FROM S where S = ?;");
if(!$rs) { echo"Error in query"; die(); } // rs returns false if fail
$rs->execute(array($_POST["S"]));

$rows = $rs->fetchAll(PDO::FETCH_ASSOC);
drawTable($rows);

}
catch(PDOexception $e) { // handle that exception
    echo "Connection to database failed: " . $e->getMessage();
    }

?>


<html>

<form action="" method="post">
<input type="text" name="S" id="$prepared" placeholder="Type Seller Num & Hit Ok">
<button type="submit" name="ok">OK</button>
<br>
<br>
</form>

<form action="http://students.cs.niu.edu/~z1910087/buy.php" method="POST">      
<input type="submit" value="Buy Parts" name="parts" />
<br><br>


</html>
