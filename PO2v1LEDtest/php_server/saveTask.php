<?php

file_put_contents('log.txt', date('d.m.Y H:i:s') . ' - saveTask - ' . var_export($_POST, true) . "\n", FILE_APPEND);

//$task = file_get_contents('task.txt');
$task = "OK";
if(isset($_POST['task'])) {	$task = $_POST['task'];}

switch($task){
	case "OK": file_put_contents('task.txt', 'OK'); break;
	case "RESET": file_put_contents('task.txt', 'RESET'); break;
	case "PWR_ON": file_put_contents('task.txt', 'PWR_ON'); break;
	case "PWR_OFF": file_put_contents('task.txt', 'PWR_OFF'); break;
	case "SHUTDOWN": file_put_contents('task.txt', 'SHUTDOWN'); break;	
	default: file_put_contents('task.txt', 'OK');
}
header('Location: interface.php?task='.$task);

?>