var g_loggedin=false;
var orgw=0;
var orgh=0;
document.getElementById("register-link").onclick=function(p_event){
	var x=document.getElementById("reg-form");
	x.style.display="block";
}

function getDocHeight() {
    var D = document;
    return Math.max(
        D.body.scrollHeight, D.documentElement.scrollHeight,
        D.body.offsetHeight, D.documentElement.offsetHeight,
        D.body.clientHeight, D.documentElement.clientHeight
    );
}

document.addEventListener('NetLoggedIn',function(e){
	document.getElementById('login-box').style.display='none';
	document.getElementById('fsib').style.display='block';
	var canvas=document.getElementById('canvas');
	canvas.style.display='block';
	var w=document.body.clientWidth;
	var h=getDocHeight();
	h=(h>667)?667:h;
	var cw=canvas.width;
	canvas.style.left=((w-cw)/2)+'px';
	canvas.height=h;
	orgw=canvas.width;
	orgh=canvas.height;
	
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
	var backbuffer=document.getElementById("backbuffer"); 
	var foreground=document.getElementById("foreground");
	var tm=TileManager;//new TileManager(4000,4000,40);
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
	var lightsources=[];
	
	tm.LoadTileAtlas('images/tileset1.png',function(){
		tm.LoadFromBitmap('images/lobby.png',function(data){
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
				var context=ctx;
				var tempAxis=new Vec2d(10,0);
				tm.SortLayers();
				var tiles=tm.GetTiles();
				var keyboardBuffer;  
				var xEnts;
				var tile;
				var obj;
				var o;
				var InB4;
				var dt;
				var count=0;
				var dx,dy,j,jC,dist,FTBDist,dn;
				var update=function(p_Delta){
					if(p_Delta-lastSnapshot>=100){
						lastSnapshot=p_Delta;
						keyboardBuffer=Keyboard.GetKeyboardBuffer();
						if(keyboardBuffer.length>1){
							SkyNet.Send(keyboardBuffer);
						}
						Keyboard.ClearBuffer();
					}
					
					count=0;
					dt=p_Delta-lastupdate; //used for lerping
					lastupdate=p_Delta;
					
					EntityMngr.Update(dt);
					Keyboard.UpdateKeyboardBuffer();
					lightsources.length=0;
					EntityMngr.GetEntitiesWithProperty('LightSource',lightsources);
					
					context.clearRect(0,0,canvas.width,canvas.height);
					xEnts=EntityMngr.GetEntities();
					iC=tiles.length;
					InB4=false;
					 for(i=0;i<iC;i++){
						tile=tiles[i];
						j,jC=lightsources.length;
						dx=((canvas.width/2)-TileManager.WorldOffsetX)-tile.m_x;
						dy=((canvas.height/2)-TileManager.WorldOffsetY)-tile.m_y;
						dist=Math.sqrt(dx*dx+dy*dy);
						FTBDist=400;
						for(j=0;j<jC;j++){
							ls=lightsources[j];
							dx=ls.m_x-tile.m_x;
							dy=ls.m_y-tile.m_y;
							dn=Math.sqrt(dx*dx+dy*dy);
							if(dn<dist){
								dist=dn;
								FTBDist=ls.Radius();
							}
						}
						if(dist>FTBDist){
							context.globalAlpha=0;
						}else{
							context.globalAlpha=(1-(dist/FTBDist));
						}

						count+=(renderTile(tile))?1:0;
					} 
					context.globalAlpha=1;
					
					for(obj in xEnts){ //can't use regular for because of index disparity
						o=xEnts[obj];
						if(!o||o.Render===undefined){continue;}
						o.Render(context);
					}
					Chat.Render();
					
					requestAnimationFrame(update);
				}
				update(0); 
		});
	}); 
}

var onfullscreenchange=function(e){
	var canvas = document.getElementById("canvas");
	if(document.webkitIsFullScreen){
		canvas.style.left=0+'px';
		canvas.style.top=0+'px';
		canvas.width=document.width;
		canvas.height=document.height-24; 
	}else{	
		var w=document.width;
		var h=document.height;
		var cw=orgw;
		canvas.style.left=((w-cw)/2)+'px';
		canvas.width=orgw;
		canvas.height=orgh;
	}
}

function goFullScreen(){
    var canvas = document.getElementById("canvas");
	var chat=document.getElementById("chat");
	
	if(canvas.webkitRequestFullScreen){
        canvas.webkitRequestFullScreen(Element.ALLOW_KEYBOARD_INPUT);
		document.addEventListener('webkitfullscreenchange',onfullscreenchange);
	}
		
}

function hidefsib(){
	document.getElementById('fsib').style.display='none';
}

