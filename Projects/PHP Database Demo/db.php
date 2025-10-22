<?php
    $connectionDB = mysqli_connect(
        "localhost",
        "comp_view",
        "teressa",
        "comp_conf"
    );
	
	$computer_id = 3;
	$disk_id = 0;
	
    if (mysqli_connect_errno()) {
        echo "Unable to connect to MySQL: " . mysqli_connect_error();
    }
?>