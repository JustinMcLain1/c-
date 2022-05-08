
<?php

    //include("z1910087"); //works similar to c++
    echo"<html><head><title>Test</title></head><body></body> 
    </html>
        This is the index document. 
    </body>
    </html>"; //php has very similar syntax to C++
    print("This is more text.");
    echo "This is more text.";
    
    $i = 3; 
    echo "i = ".$i; // the "." before the $ is for concatination
    echo"(".gettype($i).")";
    $i = 3.0;
    echo "i = $i\n"; // this is substitud with single qoutes

    $i = "3";
    echo"(".gettype($i).")"; 

    $i = 4 * 3 * 1.2 / 45;
    echo"i = $i(".gettype($i).")";

    if(gettype($x) == "NULL"){
        echo "There is no spoon, nor an x.";
    };
    else {
        echo "Whoa.";
    }

    $i = "x";

    switch($i) {
        case "x":
            echo "You chose x for i";
            break;
        case "y":
            echo "Whyyyyyy?";
            break;
        default:
            echo "Nothing really matters.";
            break;
    }

    $y = 5; 
    while($y > 0) {
        echo "$y bottles of nonalcoholic beverage on the wall.\n";
        $y--
    }

    $a[0] = 0;
    $a[1] = 1;
    $a[2] = 4;
    $a[3] = 9;
    $a[4] = 16;
    $a[7] = 49; // in c++ you cant do this but in php you can it will display the other arrays for 5 and 6 with empty values
    $a["pizza"] = "Gino's East\n";

    //constructor function for array
    $b = array(1,2,3,4,5,6,7);
    print_r($b);


    for($i=0;$i<8;++$i) {
        echo "a[$i] = ${a[$i]}\n"; // the {} indiciate the index arrays are also associative
    }

    //to print pizza array
    echo $a["pizza"];

    foreach( $a as $x ) { //x is set to the current value of the index of a
        //runs the body for the number of elements inside
        echo "foreach printed $x this time\n";
    }

    foreach( $a $key => $x ) { //thing on left is a key thing on right is a value     
        echo "foreach has an element with a key of $key, and its value is $x this time\n";
    }

    //reccursively print array
    print_r($a) // prints "array ( contents of array with arrows)" 
    // dont use in production code ***************************************************

    echo "GET:\n"; //outputs what si submitted to formw
    print_r($_GET);

    echo "you have specified an id of ${_GET["id]"]}\n";

    echo "<table border=1">;
    for($i=0;$i<$_GET["id"];$i++) {
        echo "<tr>";
        echo"<td width=50>$i</td><td>".$i*$i."</td>";
        echo"</tr>";
    }
    echo "</table>";

    echo "POST:\n";
    print_r($_POST);

?>