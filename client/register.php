<?php

require_once('backend/mysqlfunctions.php');

if(!isset($_POST['em'],$_POST['pw'],$_POST['pw2'])){fail();return;}


if(strlen($_POST['pw'])<=0||strlen($_POST['pw2'])<=0){
	fail();
	return;
}

if($_POST['pw']!=$_POST['pw2']){
	fail();
	return;
}

$res=MysqlShort::Select('id','ca_user',array('email'=>$_POST['em']));
if(count($res)>0){
	fail();
}

MysqlShort::InsertSimple('ca_user',array('email'=>$_POST['em'],'password'=>md5($_POST['pw']), 'created'=>date('Y-m-d H:i:s')));
//MysqlShort::Raw('INSERT INTO ca_user (email,password) VALUES (%1, %2)',array($_POST['em'],md5($_POST['pw'])));

header('Location: index.php?r=1');

function fail(){
	header('Location: index.php?r=0');
}


?>