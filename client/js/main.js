var mapWidth=4000;
var mapHeight=4000;
var tileS=40;
var tiles=[];
var WorldOffsetX=0;
var WorldOffsetY=(mapHeight/2)*0.2;
var l=function(a){console.log(a);};
var selected=[];
var tiles=[];
var selectionMarker={x:0,y:0,w:tileS,h:tileS};
var renderAll=true;
var wallType=0;
var currentLayer=0;
var keyMap=[];
var storageRequired=5242880;
var atlas=null;
var mapdata=null;
var map=[];
//var shadowmap=

var tile=function(p_x,p_y,p_Wall){
	this.m_x=p_x;
	this.m_y=p_y;
	this.m_Wall=p_Wall;
	this.m_Selected=false;
	this.m_Colors=[];
	this.m_Colors['default']='#00AA00';
	this.m_Colors['west']='#00EE00';
	this.m_Colors['east']='#004400'
	this.m_Colors['selected']='#ff0000';
	this.m_Layer=0;
	this.m_Loaded=false;
	var xThis=this;
	
	this.draw=function(){
		var ctx=document.getElementById('canvas').getContext('2d');
		ctx.lineWidth=0;
		ctx.drawImage(atlas,(xThis.m_Wall*tileS)+1,1,39,39,xThis.m_x+WorldOffsetX,xThis.m_y+WorldOffsetY,tileS,tileS);
	}
}


function GetTileFromWorldPos(p_X,p_Y,p_Layer){
	if(p_Layer<0||p_Layer>=tiles.length){return null;}
	var i,iC=tiles[p_Layer].length;
	for(i=0;i<iC;i++){
		var t=tiles[p_Layer][i];
		if(!t){continue;}
		if(t.m_x==p_X&&t.m_y==p_Y){
			return t;
		}
	}
	return null;
}
function SortLayers(a,b){
	return a.m_Layer<b.m_Layer;
}
function WorldToIsometric(p_X,p_Y,p_Scale){
	var angle=-45;
	angle=angle*Math.PI/180;
	var x=p_X,y=p_Y;
	var cx=x*Math.cos(angle)-y*Math.sin(angle);
	var cy=y*Math.cos(angle)+x*Math.sin(angle);
	return [(cx*p_Scale),((cy*p_Scale))];
}

function BuildComplex(p_x,p_y,p_Type){
	var block3x3=[[{t:4,l:3},{t:3,l:3},{t:3,l:3}],
				[{t:1,l:1},{t:3,l:1},{t:3,l:1}],
				[{t:6,l:1},{t:2,l:1},{t:5,l:1}]
	];
	var container=[];
	container['block3x3']=block3x3;
	
	if(container[p_Type]===undefined){
		return;
	}
	
	var block=container[p_Type];
	var cx=p_x;
	var cy=p_y;
	var j,jC=block.length;
	for(j=0;j<jC;j++){
		var n, nC=block[j].length;
		for(n=0;n<nC;n++){
			var t=block[j][n];
			if(t==null){continue;}
			var crd=WorldToIsometric(cx+n,cy,tileS);
			var nt=new tile(crd[0],crd[1],t.t);
			nt.m_Layer=t.l;
			if(tiles[t.l]===undefined){
				tiles[t.l]=new Array();
			}
			tiles[t.l].push(nt);
		}
		cy++;
	}
}

function init(){
	//if(navigator.userAgent.toLowerCase());
	mapdata=new Image();
	mapdata.onload=function(){
		var context=document.getElementById('canvas').getContext('2d');
		if(context.mozImageSmoothingEnabled!==undefined){
			//context.mozImageSmoothingEnabled=false;
		}
		context.drawImage(mapdata,0,0,mapdata.width,mapdata.height,0,0,mapdata.width,mapdata.height);
		var imgData=context.getImageData(0,0,mapdata.width,mapdata.height);

		var d=imgData.data;
		context.fillStyle='#000000';
		context.fillRect(0,0,mapWidth,mapHeight);
		var colorKeys=[0,0xff0000,0x00ff00,0x0000ff,0xffff00,0xff00ff,0x00ffff,0xee0000];
		
		var i,iC=d.length;
		for(i=0;i<iC;i+=4){
			var red=d[i];
			var green=d[i+1];
			var blue=d[i+2];
			var alpha=d[i+3];
			
			//firefox fix
			if(red<2){red=0;}
			if(green<2){green=0;}
			if(blue<2){blue=0;}
			//end fix
			
			var h = red;
			h=(h<<8)|(green&0xff);
			h=(h<<8)|(blue&0xff);// ((1 << 24) + (red << 16) + (green << 8) + blue).toString(16).substr(1);
			var idx=colorKeys.indexOf(h);
			map.push((idx==-1)?0:idx);
		}
		
		atlas=new Image();
		atlas.onload=function(){
			initWorld();
		}
		atlas.src='images/atlas.png'; 
		
		/* 0xff0000=1
		0x00ff00=2
		0x0000ff=3
		0xffff00=4
		0xff00ff=5
		0x00ffff=6
		0xee0000=7 */
		
		
	}
	mapdata.src='images/map0.png';
	
}

function initWorld(){
		var context=document.getElementById('canvas').getContext('2d');
		context.font="12px Georgia";
		tiles.push(new Array());
		
		var x=0,y=0;
		var i,iC=2;map.length;
		
		currentLayer=0;
		context.fillStyle='red';
		 for(i=0;i<iC;i++){
			var crd=WorldToIsometric(x/1.5,y/1.5,tileS);
			var filler=new tile(crd[0],crd[1],0);
			filler.m_Layer=0;
			var nt=new tile(crd[0],crd[1],map[i]);
			nt.m_Layer=1;
			tiles[currentLayer].push(filler);
			tiles[currentLayer].push(nt);
			filler.draw();
			nt.draw();
			y=((i+1)%((mapWidth/tileS))==0&&i!=0)?y+1:y;
			x=((i+1)%((mapWidth/tileS))==0&&i!=0)?0:x+1;
			l(nt);
		} 

		
		
		iC=255;
		for(i=0;i<iC;i++){
			keyMap[i]=false;
		}
		
		var xAnim=new Animation('images/walkanim.png');
		xAnim.m_x=400;
		xAnim.m_y=250;

		var canvas=document.getElementById('canvas');
		var date=new Date();
		var lastTime=date.getTime();
		var frames=0;
		var fps=0;
		 setInterval(function(){
			return;
			context.fillStyle='#000000';
			context.fillRect(0,0,mapWidth,mapHeight);
			
			var i,iC=tiles[currentLayer].length;
			if(!renderAll){
				var j,jC=tiles.length;
				for(j=0;j<jC;j++){
					iC=tiles[j].length;
					if(j==currentLayer){
						context.globalAlpha =1.0;
					}else{
						context.globalAlpha =0.4;
					}
					for(i=0;i<iC;i++){
						tiles[j][i].draw();
					}
				}
			}else{
				var j,jC=tiles.length;
				var rendered=0;
				for(j=0;j<jC;j++){
					if(tiles[j]===undefined){continue;}
					iC=tiles[j].length;
					for(i=0;i<iC;i++){
					 	/* if(tiles[j][i].m_Layer==0){continue;}  */
						var ct=tiles[j][i];
						if(ct.m_x+WorldOffsetX<-(tileS)||ct.m_x+WorldOffsetX>((canvas.width)+tileS)){continue;}
						if(ct.m_y+WorldOffsetY<-(tileS)||ct.m_y+WorldOffsetY>((canvas.height)+tileS)){continue;}
						
						ct.draw();
						rendered++;
					}
				}
			}
			context.globalAlpha =1.0;
			
			xAnim.Animate();
			
			frames++;
			date=new Date();
			var currS=date.getTime();
			context.fillStyle="red";
			context.fillText("Rendered: "+rendered+" tiles",10, 10);
			if(currS-lastTime>=1000){
				fps=frames;
				frames=0;
				lastTime=currS;
			}
			context.fillText("FPS: "+fps,10, 20);
		},17); 

		document.onkeydown=function(event){
			var wi=event.which;
		//	l(wi);
			keyMap[wi]=true;
			if(keyMap[40]){
				return false;
			}
			if(keyMap[38]){
				return false;
			}
			if(keyMap[37]){
				return false;
			}
			if(keyMap[39]){
				return false;
			}
		}
		document.onkeyup=function(event){
			var wi=event.which;
			keyMap[wi]=false;
		}
		document.onclick=function(event){
			
		}
}

function initWS(){
	var host = "ws://127.0.0.1:10444";
  try{
    socket = new WebSocket(host);
    l('WebSocket - status '+socket.readyState);
    socket.onopen    = function(msg){ l("Welcome - status "+this.readyState); };
    socket.onmessage = function(msg){ console.log(msg); l("Received: "+msg.data); };
    socket.onclose   = function(msg){ l("Disconnected - status "+this.readyState); };
	socket.onerror 	 = function(msg){l(msg);}
  }
  catch(ex){ l(ex); }
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
