var l=function(a){console.log(a);}
var player={x:450,y:250};
var g_loggedin=false;
var g_socket=new WebSocket("ws://mstrp2ez.no-ip.org:10444");
g_socket.onopen=function(){l("Connected");}
g_socket.onclose=function(){l("Disconnected");}
g_socket.onmessage=function(msg){
	if(g_loggedin===false){
		HandleLogin(msg);
		return;
	}
	
}
g_socket.onerror=function(msg){l(msg);}

function HandleLogin(p_msg){
	var reader=new FileReader();
	reader.addEventListener("loadend", function(){
		var view=new Uint8Array(reader.result);
		var appOpCode=view[0];
		var msg=view.subarray(1);
		if(appOpCode!=0x02){}
		
		var textBlob=new Blob([msg]);
		var fr=new FileReader();
		fr.onload=function(e){
			if(e.target.result=="ok"){
				g_loggedin=true;
				l("Logged in");
				document.getElementById('login-box').style.display="none";
				var canvas=document.getElementById('canvas');
				canvas.style.display="block";
				init();
			}
		}
		fr.readAsText(textBlob);
	});
	reader.readAsArrayBuffer(p_msg.data);
}

var loginbtn=document.getElementById('login-btn');
loginbtn.onclick=function(){
	var s=g_socket;
	var name=document.getElementById('login-name').value;
	var pswd=document.getElementById('login-pswd').value;
	var data=0x02+name+';'+pswd;
	
	var bb=new Blob([data],{type: 'text/plain'});
	var fr=new FileReader();
	fr.onload=function(e){
		var buff=new Uint8Array(e.target.result);
		buff[0]=buff[0]-48;
		 try{ s.send(buff.buffer); } catch(ex){ log(ex); }
	}
	fr.readAsArrayBuffer(bb);
}

function init(){
	var canvas = document.getElementById("canvas");
	var tm=new TileManager(4000,4000,40);
	var target=null;
	var anim=new Animation();
	anim.m_x=400;
	anim.m_y=300;
	var xThis=this;
	var direction=Vec2d(0,0);
	var speed=8;
	var mousedown=false;
	var mousepos=null;
	
	tm.LoadTileAtlas('images/atlas.png',function(){
		tm.LoadFromBitmap('images/map0.png',function(data){
			var ctx=canvas.getContext('2d');		
			Mouse.RegisterEventListener('mousedown',function(event){
				mousedown=true;
			});
			Mouse.RegisterEventListener('mouseup',function(event){
				mousedown=false;
			});
			Mouse.RegisterEventListener('mousemove',function(event){
				mousepos={x:event.x,y:event.y};
			});
			Keyboard.RegisterEventListener('keydown',function(event){
				if(event.which==107){
					anim.SetAnimation(anim.m_currentAnim+1);
				}
				if(event.which==109){
					anim.SetAnimation(anim.m_currentAnim-1);
				}
			});
			anim.Load('images/idleanim2.png',function(){
				var tempAxis=new Vec2d(10,0);
				tm.SortLayers();
				var arr=[];
				arr.length=0;
				tm.GetTilesInViewport(arr);
				arr.sort(tm.SortFunc);
				function render(p_Delta){
					ctx.clearRect(0,0,tm.m_MapWidth,tm.m_MapHeight);
					var i,iC=arr.length;
					var bInB4=false;
					for(i=0;i<iC;i++){
						if(!bInB4&&arr[i].m_Layer==2){
							bInB4=true;
							anim.Animate(p_Delta);
						}
						arr[i].draw();
					}
					requestAnimationFrame(render);
				}
				render(0);
			});
		});
	}); 
}

function goFullScreen(){
    var canvas = document.getElementById("canvas");
    if(canvas.requestFullScreen)
        canvas.requestFullScreen();
    else if(canvas.webkitRequestFullScreen)
        canvas.webkitRequestFullScreen();
    else if(canvas.mozRequestFullScreen)
        canvas.mozRequestFullScreen();
		
	canvas.width=1024;
	canvas.height=768;
}

