<?php
class arduino{
 public $link='';
 function __construct($temp_zone_chaude, $temp_zone_froide, $id){
  $this->connect();
  $this->storeInDB($temp_zone_chaude, $temp_zone_froide, $id);
 }

 function connect(){
  $this->link = mysqli_connect('mysql-redcommand.alwaysdata.net','230393','Maxime1612') or die('Cannot connect to the DB');
  mysqli_select_db($this->link,'redcommand_terrarium') or die('Cannot select the DB');
 }

 function storeInDB($temp_zone_chaude, $temp_zone_froide){
  $query = "UPDATE devices set temp_zone_chaude='".$temp_zone_chaude."', temp_zone_froide=-'".$temp_zone_froide."' WHERE id='".$id."'";
  $result = mysqli_query($this->link,$query) or die('Errant query:  '.$query);
}

}
if($_GET['temp_zone_chaude'] != '' and  $_GET['temp_zone_froide'] != '' and  $_GET['id'] != ''){
 $arduino=new arduino($_GET['temp_zone_chaude'],$_GET['temp_zone_froide'],$_GET['id']);
}
?>
