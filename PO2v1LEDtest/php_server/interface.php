<?php
//echo nl2br("\n\nRESPOSTA\n");
/*file_put_contents('log.txt', date('d.m.Y H:i:s') . ' - ' . var_export($_POST, true) . "\n", FILE_APPEND);
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
*/

echo
"<form action='saveTask.php#' method='post'>
<input type='submit' name='task' value='OK' />
</form>";

echo
"<form action='saveTask.php' method='post'>
<input type='submit' name='task' value='RESET' />
</form>";

echo
"<form action='saveTask.php' method='post'>
<input type='submit' name='task' value='PWR_ON' />
</form>";

echo
"<form action='saveTask.php' method='post'>
<input type='submit' name='task' value='PWR_OFF' />
</form>";

echo
"<form action='saveTask.php' method='post'>
<input type='submit' name='task' value='SHUTDOWN' />
</form>";

echo nl2br("\nTAREFA\n");
/*if(isset($_POST['OK'])) {	echo "OK ";}
if(isset($_POST['RESET'])){	echo "RESET ";}
if(isset($_POST['PWR_ON'])) {	echo "PWR_ON ";}
if(isset($_POST['PWR_OFF'])) {	echo "PWR_OFF ";}
if(isset($_POST['SHUTDOWN'])) {	echo "SHUTDOWN ";}*/

if(isset($_GET['task'])) {	echo $_GET['task'];}
//else { echo "aqui000!";}

echo nl2br("\n\nESTADO\n");
$estado = file_get_contents('debug.txt');
echo $estado;



//Serial.print(digitalRead(STATUS_PIN)); Serial.print("\t | ");
//Serial.print(digitalRead(PWR_PIN)); Serial.print("\t  | ");
//Serial.print(digitalRead(RST_PIN)); Serial.print("\t   | ");
//Serial.print(digitalRead(LD_GRN)); Serial.print("\t    | ");
//Serial.print(digitalRead(LD_RED)); Serial.println("\t      |");}
?>