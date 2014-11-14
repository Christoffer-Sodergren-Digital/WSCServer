var g_loggedin=false;

document.getElementById("register-link").onclick=function(p_event){
	var x=document.getElementById("reg-form");
	x.style.display="block";
}

document.addEventListener('NetLoggedIn',function(e){
	document.getElementById('login-box').style.display='none';
<<<<<<< HEAD
	document.getElementById('fsib').style.display='block';
	var canvas=document.getElementById('canvas');
	var fbc=document.getElementById('foreground');
	canvas.style.display='block';
	fbc.style.display='block';
	var w=document.body.clientWidth;
	var h=getDocHeight();
	h=(h>667)?667:h;
	var cw=canvas.width;
	canvas.style.left=((w-cw)/2)+'px';
	canvas.height=h;
	fbc.style.left=((w-cw)/2)+'px';
	fbc.height=h;
	orgw=canvas.width;
	orgh=canvas.height;
	
=======
	document.getElementById('canvas').style.display='block';
	/* document.getElementById('background').style.display='block';
	document.getElementById('foreground').style.display='block'; */
>>>>>>> parent of fa8c75e... Entities, collisions and movement
	init();
	Keyboard.init();
},false);

var loginbtn=document.getElementById('login-btn');
loginbtn.onclick=function(){
	var name=document.getElementById('login-name').value;
	var pswd=document.getElementById('login-pswd').value;
	var data=0x02+name+';'+pswd;
	SkyNet.Send(data);
}

function init(){
<<<<<<< HEAD
	var tm=TileManager;//new TileManager(4000,4000,40);	
	tm.LoadTileAtlas('images/tileset1.png',function(){
		tm.LoadFromBitmap('images/lobby.png',function(data){
			var canvas = document.getElementById("canvas");
			var backbuffer=document.getElementById("backbuffer"); 
			var foreground=document.getElementById("foreground");

			var target=null;
			var xThis=this;
			var direction=Vec2d(0,0);
			var speed=8;
			var mousedown=false;
			var mousepos=null;
			var lastSnapshot=0;
			var snapshots=[];
			var inputBuffer=[4];
			var lastupdate=1;
			var lightsources=[];
=======
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
>>>>>>> parent of fa8c75e... Entities, collisions and movement
			var ctx=canvas.getContext('2d');		
			var fgctx=foreground.getContext('2d');
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
			var ls;
			var dx,dy,j,jC,dist,FTBDist,dn;
			var ui=window.UI;
			document.addEventListener('PlayerLoaded',function(e){
				if(EntityMngr.m_PlayerID!=-1&&e.detail.d.m_EntityID==EntityMngr.m_PlayerID){
					ui.Load('mainui.cui',ctx);
				}
			});
<<<<<<< HEAD
			var update=function(p_Delta){
			
				if(p_Delta-lastSnapshot>=100){
					lastSnapshot=p_Delta;
					Keyboard.GetKeyboardBuffer(inputBuffer);
					Mouse.GetInputBuffer(inputBuffer);
					if(inputBuffer.length>1){
						SkyNet.Send(inputBuffer, 'numbers');
					}
					Keyboard.ClearBuffer();
					Mouse.ClearBuffer();
					inputBuffer.length=0;
					inputBuffer.push(0x04);
				}
				
				count=0;
				dt=p_Delta-lastupdate; 
				lastupdate=p_Delta;
				
				EntityMngr.Update(dt);
				Keyboard.UpdateKeyboardBuffer();
				EntityMngr.GetEntitiesWithProperty('LightSource',lightsources); //this is temporary, make more elegant
				
				context.clearRect(0,0,canvas.width,canvas.height);
				xEnts=EntityMngr.GetEntities();
				iC=tiles.length;
				InB4=false;
				 for(i=0;i<iC;i++){
					tile=tiles[i];
					if(tile.m_x+TileManager.WorldOffsetX<tile.m_Canvas.width&&tile.m_x+TileManager.WorldOffsetX>-tile.m_Size){
						if(tile.m_y+TileManager.WorldOffsetY<tile.m_Canvas.height&&tile.m_y+TileManager.WorldOffsetY>-tile.m_Size){
						
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
					}
				}  
				context.globalAlpha=1;
				
				
				for(obj in xEnts){ //can't use regular for because of index disparity
					o=xEnts[obj];
					if(!o||o.Render===undefined){continue;}
					o.Render(context);
				}
				Chat.Render();
				
				ui.Render(context);
				
				requestAnimationFrame(update);
			}
			update(0); 
=======
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
>>>>>>> parent of fa8c75e... Entities, collisions and movement
		});
	}); 
}

<<<<<<< HEAD
var onfullscreenchange=function(e){
	var canvas = document.getElementById("canvas");
	var fbc=document.getElementById('foreground');
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
	UI.Recalculate(canvas.getContext('2d'));
}

=======
>>>>>>> parent of fa8c75e... Entities, collisions and movement
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

