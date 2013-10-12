<!doctype html>
<html>
<head>
<style>
	body{
		margin:0;
		background-color:#000000;
		font-family:Arial,Helvetica,sans-serif;
		color:#eeeeee;
	}
	a{
		color:#eeeeee;
	}
	#login-box{
		margin:0 auto;
		width:256px;
		border:1px solid #005784;
		border-radius:10px;
		background-color:#005784;
		padding:20px;
		margin-top:20px;
	}
	.login-input{
		float:right;
	}
</style>
</head>
<body>
<div id="login-box">
	<h3>Cager</h3>
	<p>Name:<input class="login-input" type="text" id="login-name" /></p>
	<p>Password: <input class="login-input" type="password" id="login-pswd" /></p>
	<input type="button" value="Login" id="login-btn" />
	<a href="#">Register account</a> 
</div>
<canvas id="canvas" width="900" height="800" style="float:left;background-color:#000000;display:none;"></canvas>
<script src="js/Keyboard.js"></script>
<script src="js/Mouse.js"></script>
<script src="js/Vec2d.js"></script>
<script src="js/Tilemngr.js"></script>
<script src="js/Animation.js"></script>
<!--<script src="js/main.js"></script>-->
<script src="js/Main2.js"></script>
</body>
</html>