<html>

<head>
    <title>Justin McLain Z1910087</title>
    <body> 
        <h1> CSCI 466 Assignment 8 </h1> 
    </body>
</head>
<?php 
echo "<br>";
include ("assignment8inclusions.php");

    try { // if something goes wrong, an exception is thrown
        $dsn = "mysql:host=courses;dbname=z1910087";
        $pdo = new PDO($dsn, $username, $password);
        $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
    }
    catch(PDOexception $e) { // handle that exception
        echo "Connection to database failed: " . $e->getMessage();
        }
    

?>