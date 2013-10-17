var g_loggedin=false;

document.getElementById("register-link").onclick=function(p_event){
	var x=document.getElementById("reg-form");
	x.style.display="block";
}

document.addEventListener('NetLoggedIn',function(e){
	document.getElementById('login-box').style.display='none';
	document.getElementById('canvas').style.display='block';
	/* document.getElementById('background').style.display='block';
	document.getElementById('foreground').style.display='block'; */
	init();
	Keyboard.init();
},false);

function drawBubble(ctx, message, x, y, w, h, radius){
	var r = x + w;
	var b = y + h;
	ctx.beginPath();
	ctx.strokeStyle="black";
	ctx.lineWidth="2";
	ctx.moveTo(x+radius, y);
	ctx.lineTo(x+radius/2, y-10);
	ctx.lineTo(x+radius * 2, y);
	ctx.lineTo(r-radius, y);
	ctx.quadraticCurveTo(r, y, r, y+radius);
	ctx.lineTo(r, y+h-radius);
	ctx.quadraticCurveTo(r, b, r-radius, b);
	ctx.lineTo(x+radius, b);
	ctx.quadraticCurveTo(x, b, x, b-radius);
	ctx.lineTo(x, y+radius);
	ctx.quadraticCurveTo(x, y, x+radius, y);
	ctx.stroke();
	ctx.fillStyle="white";
	ctx.fill();
	ctx.fillStyle="black";
	ctx.font="12px Arial";
	var line=Math.ceil((w+20)/10);
	var offset=0;
	var row=y+20;
	while(offset<message.length){
		var sub=message.substr(offset,line);
		ctx.fillText(sub,x+20,row);
		row+=14;
		offset+=line;
	}
}

var loginbtn=document.getElementById('login-btn');
loginbtn.onclick=function(){
	var name=document.getElementById('login-name').value;
	var pswd=document.getElementById('login-pswd').value;
	var data=0x02+name+';'+pswd;
	SkyNet.Send(data);
}

function init(){
	var canvas = document.getElementById("canvas");
	/* var backbuffer=document.getElementById("backbuffer"); */
	var tm=new TileManager(4000,4000,40);
	var target=null;
	var anim=new Animation();
	var xThis=this;
	var direction=Vec2d(0,0);
	var speed=8;
	var mousedown=false;
	var mousepos=null;
	var lastSnapshot=0;
	var snapshots=[];
	var lastupdate=1;
	
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
				var context=ctx;
				/* var bb=backbuffer.getContext('2d'); */
				var tempAxis=new Vec2d(10,0);
				tm.SortLayers();
				var arr=[];
				arr.length=0;
				tm.GetTilesInViewport(arr);
				arr.sort(tm.SortFunc);
				var bg=tm.GetTilesInViewPortOnLayer(0);
				var fg=tm.GetTilesInViewPortOnLayer(2);
				function update(p_Delta){
					if(p_Delta-lastSnapshot>=100){
						lastSnapshot=p_Delta;
						var kb=Keyboard.GetKeyboardBuffer();
						Keyboard.ClearBuffer();
						if(kb.length>1){
							SkyNet.Send(kb);
						}
					}
					
					var dt=p_Delta-lastupdate; //used for lerping
					lastupdate=p_Delta;
					
					EntityMngr.Update(dt);
					Keyboard.UpdateKeyboardBuffer();
					
					context.clearRect(0,0,canvas.width,canvas.height);
					var i,iC=bg.length;
					for(i=0;i<iC;i++){
						bg[i].Draw(context);
					}
					var xEnts=EntityMngr.GetEntities();
					for(var obj in xEnts){ //can't use regular for because of index disparity
						var o=xEnts[obj];
						if(!o||o.Render===undefined){continue;}
						o.Render(context);
					}
					iC=fg.length;
					for(i=0;i<iC;i++){
						fg[i].Draw(context);
					}
					
					
					//context.drawImage(backbuffer,0,0);
					requestAnimationFrame(update);
				}
				update(0); 
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

