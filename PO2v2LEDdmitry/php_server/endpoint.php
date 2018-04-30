<?php
file_put_contents('log.txt', date('d.m.Y H:i:s') . ' - ' . var_export($_POST, true) . "\n", FILE_APPEND);
$task = file_get_contents('task.txt');
echo $task;
if ($task !== 'OK') {
    file_put_contents('task.txt', 'OK');}
if ($task !== 'RESET') {
    file_put_contents('task.txt', 'RESET');}
if ($task !== 'PWR_ON') {
    file_put_contents('task.txt', 'PWR_ON');}
if ($task !== 'PWR_OFF') {
    file_put_contents('task.txt', 'PWR_OFF');}
if ($task !== 'SHUTDOWN') {
    file_put_contents('task.txt', 'SHUTDOWN');}
?>