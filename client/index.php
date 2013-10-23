<!doctype html>
<html>
<head>
<style>
	body{
		margin:0;
		background-color:#000000;
		font-family:Arial,Helvetica,sans-serif;
		color:#eeeeee;
		overflow:hidden;
		height:100%;
	}
	a{
		color:#eeeeee;
	}
	#canvas{
		position:absolute;
		background-color:#000000;
		display:none;
		z-index:2;
		border:1px solid #444444;
	}
	#login-box{
		margin:0 auto;
		width:270px;
		border:1px solid #005784;
		border-radius:10px;
		background-color:#005784;
		padding:20px;
		margin-top:20px;
	}
	.login-input{
		float:right;
	}
	.register-form{
		display:none;
	}
	.success{
		color:#22ae22;
	}
	.failed{
		color:#ae2222;
	}
	.fullscreen-info-box{
		display:none;
		position:absolute;
		top:0;
		float:left;
		width:180px;
		border:1px solid #eeeeee;
	}
</style>
</head>
<body>
<div id="login-box">
	<h3>Cager</h3>
	<p>EMail:<input class="login-input" type="text" id="login-name" /></p>
	<p>Password: <input class="login-input" type="password" id="login-pswd" /></p>
	<input type="button" value="Login" id="login-btn" />
	<a href="#" id="register-link">Register account</a> 
	<div class="reg-result"><?php if(isset($_GET['r'])){if($_GET['r']==1){echo '<span class="success">Register ok</span>';}else{echo '<span class="failed">Register Failed</span>';}}?></div>
	<div class="register-form" id="reg-form">
		<form method="POST" action="register.php">
			<p>Email:<input class="login-input" type="text" name="em"/></p>
			<p>Password:<input class="login-input" type="password" name="pw" id="reg-pwd"/></p>
			<p>Again:<input class="login-input" type="password" name="pw2" id="reg-pwd2"/></p>
			<input type="submit" value="Register">
		</form>
	</div>
</div>
<div id="fsib" class="fullscreen-info-box">
	<h3>Fullscreen</h3>
	<p>Cager can be run in fullscreen, however only Chrome currently supports full keyboard input while in fullscreen.</p>
	<p>If you want to play Cager in fullscreen mode, please use Chrome.</p>
	<input type="button" onclick="goFullScreen()" id="fullscrnbtn" value="Go fullscreen">
	<input type="button" onclick="hidefsib()" value="Close">
</div>

<canvas id="backbuffer" width="900" height="200" style="position:absolute;left:0;top:0;background-color:#000000;display:none;z-index:1;"></canvas>
<canvas id="canvas" width="900" height="200"></canvas>
<canvas id="foreground" width="900" height="200" style="position:absolute;left:0;top:0;background-color:#000000;display:none;z-index:1;"></canvas>
<script src="js/Utils.js"></script>
<script src="js/Keyboard.js"></script>
<script src="js/Chat.js"></script>
<script src="js/Network.js"></script>
<script src="js/En.js"></script>
<script src="js/Vec2d.js"></script>
<script src="js/Tilemngr.js"></script>
<script src="js/Entities.js"></script>
<script src="js/Entitymngr.js"></script>
<script src="js/Mouse.js"></script>
<script src="js/Animation.js"></script>
<script src="js/Main2.js"></script>

</body>
</html>