<?php
$estado = "NONE";
if (isset($_GET["estado"])) {$estado = $_GET["estado"];}
file_put_contents('debug.txt', date('d.m.Y H:i:s') . ' - ' . $estado);
file_put_contents('log.txt', date('d.m.Y H:i:s') . ' - endpoint - ' . var_export($_POST, true) . "\n", FILE_APPEND);
$task = file_get_contents('task.txt');
echo $task;
?>