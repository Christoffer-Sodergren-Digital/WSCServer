

var Tile=function(p_Col,p_Row,p_Size,p_Wall,p_Layer,p_Atlas){
	this.m_Canvas=document.getElementById('canvas');
	this.m_Ctx=document.getElementById('canvas').getContext('2d');
	this.m_Col=p_Col;
	this.m_Row=p_Row;
	this.m_Size=p_Size;
	this.m_Wall=p_Wall;
	this.m_AtlasX=p_Wall;
	this.m_AtlasY=0;
	this.m_Layer=p_Layer;
	this.m_Loaded=false;
	this.m_Atlas=p_Atlas;
	this.m_SpriteSize=[p_Size,p_Size];
	this.m_Ca=0;
	this.m_Sa=0;
	this.m_x=this.m_Col*p_Size;
	this.m_y=this.m_Row*p_Size;
	this.m_Selected=false;
	
	var xThis=this;
	this.PreCalc=(function(){
		var angle=-45;
		angle=angle*Math.PI/180;
		xThis.m_Ca=Math.cos(angle);
		xThis.m_Sa=Math.sin(angle);
	}());
	
	this.GetIsometricPosition=function(){
		var step=xThis.m_Size;
		var x=xThis.m_Col*step;
		var y=xThis.m_Row*step;

		xThis.m_x=x+TileManager.WorldOffsetX;
		xThis.m_y=y+TileManager.WorldOffsetY;
		return [x,y];
	}
	//this.GetIsometricPosition();
	
	this.Draw=function(p_ctx){
		
	}
	this.Select=function(){
		xThis.m_Selected=!xThis.m_Selected;
	}
	this.GetWorldPos=function(){
		return [xThis.m_Col*xThis.m_Size,xThis.m_Row*xThis.m_Size];
	}
	this.Row=function(){return xThis.m_Row;}
	this.Col=function(){return xThis.m_Col;}
}

function renderTile(p_Tile){  // closures consume way to much memory, using regular function for rendering instead
	//p_Tile.m_x= (((p_Tile.m_Col*p_Tile.m_Size)*0.65)*p_Tile.m_Ca-((p_Tile.m_Row*p_Tile.m_Size)*0.65)*p_Tile.m_Sa)+TileManager.WorldOffsetX; //this used to be readable, but javascript allocates memory for temp vars, so i had to compress it
	//p_Tile.m_y= (((p_Tile.m_Row*p_Tile.m_Size)*0.65)*p_Tile.m_Ca+((p_Tile.m_Col*p_Tile.m_Size)*0.65)*p_Tile.m_Sa)+TileManager.WorldOffsetY;
	
	if(p_Tile.m_x+TileManager.WorldOffsetX>p_Tile.m_Canvas.width||p_Tile.m_x+TileManager.WorldOffsetX<-p_Tile.m_Size){return false;}
	if(p_Tile.m_y+TileManager.WorldOffsetY>p_Tile.m_Canvas.height||p_Tile.m_y+TileManager.WorldOffsetY<-p_Tile.m_Size){return false;}
	p_Tile.m_Ctx.drawImage(p_Tile.m_Atlas,
		(p_Tile.m_AtlasX*p_Tile.m_Size),
		(p_Tile.m_AtlasY*p_Tile.m_Size),
		p_Tile.m_Size,
		p_Tile.m_Size,
		p_Tile.m_x+TileManager.WorldOffsetX,
		p_Tile.m_y+TileManager.WorldOffsetY,
		p_Tile.m_Size,
		p_Tile.m_Size );  
	if(p_Tile.m_Selected){
		p_Tile.m_Ctx.fillStyle='#ff0000';
		p_Tile.m_Ctx.fillRect(p_Tile.m_x+p_Tile.m_Size/2,p_Tile.m_y+p_Tile.m_Size/2,10,10);
	}
	return true;
}

var TileManager=function(p_Width,p_Height,p_StepSize){
	this.m_Tiles=[];
	this.m_MapWidth=(p_Width===undefined)?800:p_Width;
	this.m_MapHeight=(p_Height===undefined)?800:p_Height;
	this.m_Step=(p_StepSize===undefined)?80:p_StepSize;
	this.m_TileAtlas=null;
	this.m_Type='TileManager';
	var canvas=document.getElementById('canvas');
	this.m_CanvasW=canvas.width;
	this.m_CanvasH=canvas.height;
	this.WorldOffsetX=0;
	this.WorldOffsetY=0;
	
	var xThis=this;
	this.NewTile=function(p_Col,p_Row,p_Wall,p_Layer){
		var nt=new Tile(p_Col,p_Row,xThis.m_Step,p_Wall,p_Layer,xThis.m_TileAtlas);
		xThis.m_Tiles.push(nt);
		return nt;
	}
	this.AddTile=function(p_Tile){
		if(p_Tile===undefined){return;}
		xThis.m_Tiles.push(p_Tile);
	}
	this.LoadTileAtlas=function(p_Src,p_Callback){
		xThis.m_TileAtlas=new Image();
		xThis.m_TileAtlas.onload=function(event){
			p_Callback.call();
		}
		xThis.m_TileAtlas.src=p_Src;
	}
	this.SortFunc=function(p_A,p_B){
		return (p_A.m_Layer-p_B.m_Layer);
	}
	this.GetTileFromWorldPos=function(p_x,p_y,p_Layer){
		var ts=xThis.m_Tiles;
		var i,iC=ts.length;
		var s=xThis.m_Step;
		for(i=0;i<iC;i++){
			var iso=ts[i].GetIsometricPosition();
			var x=iso[0];
			var y=iso[1];
			if(p_x>=x&&p_x<x+s){
				if(p_y>=y&&p_y<y+s){
					return ts[i];
				}
			}
		}
		return null;
	}
	this.GetTileFromRowCol=function(p_Col,p_Row,p_Layer){
		var i,iC=xThis.m_Tiles.length;
		if(p_Layer===undefined){
			for(i=iC;i--;){
				var t=xThis.m_Tiles[i];
				if(t.m_Row==p_Row&&t.m_Col==p_Col){
					return t;
				}
			}
		}else{
			for(i=iC;i--;){
				var t=xThis.m_Tiles[i];
				if(t.m_Row==p_Row&&t.m_Col==p_Col&&t.m_Layer==p_Layer){
					return t;
				}
			}
		}
		return null;
	}
	this.GetStraightAdjecent=function(p_Tile){
		var ret=new Array();
			var Gtfrc=xThis.GetTileFromRowCol;
			ret.push(Gtfrc(p_Tile.m_Col+1,p_Tile.m_Row));
			ret.push(Gtfrc(p_Tile.m_Col,p_Tile.m_Row+1));
			ret.push(Gtfrc(p_Tile.m_Col-1,p_Tile.m_Row));
			ret.push(Gtfrc(p_Tile.m_Col,p_Tile.m_Row-1));
		return ret;
	}
	this.GetAdjecent=function(p_Tile){
		var ret=new Array();
		var Gtfrc=xThis.GetTileFromRowCol;
			ret.push(Gtfrc(p_Tile.m_Col+1,p_Tile.m_Row));
			ret.push(Gtfrc(p_Tile.m_Col,p_Tile.m_Row+1));
			ret.push(Gtfrc(p_Tile.m_Col-1,p_Tile.m_Row));
			ret.push(Gtfrc(p_Tile.m_Col,p_Tile.m_Row-1));
			ret.push(Gtfrc(p_Tile.m_Col+1,p_Tile.m_Row+1));
			ret.push(Gtfrc(p_Tile.m_Col+1,p_Tile.m_Row-1));
			ret.push(Gtfrc(p_Tile.m_Col-1,p_Tile.m_Row-1));
			ret.push(Gtfrc(p_Tile.m_Col-1,p_Tile.m_Row+1));
		return ret;
	}
	this.GetAdjecentWithCost=function(p_Tile){
		var ret=new Array();
		var Gtfrc=xThis.GetTileFromRowCol;
			ret.push({t:Gtfrc(p_Tile.m_Col+1,p_Tile.m_Row),g:10});
			ret.push({t:Gtfrc(p_Tile.m_Col,p_Tile.m_Row+1),g:10});
			ret.push({t:Gtfrc(p_Tile.m_Col-1,p_Tile.m_Row),g:10});
			ret.push({t:Gtfrc(p_Tile.m_Col,p_Tile.m_Row-1),g:10});
			ret.push({t:Gtfrc(p_Tile.m_Col+1,p_Tile.m_Row+1),g:14});
			ret.push({t:Gtfrc(p_Tile.m_Col+1,p_Tile.m_Row-1),g:14});
			ret.push({t:Gtfrc(p_Tile.m_Col-1,p_Tile.m_Row-1),g:14});
			ret.push({t:Gtfrc(p_Tile.m_Col-1,p_Tile.m_Row+1),g:14});
		return ret;
	}
	this.SortLayers=function(){
		xThis.m_Tiles.sort(xThis.SortFunc);
	}
	this.GetTiles=function(){
		return xThis.m_Tiles;
	}
	this.GetTilesInViewport=function(p_Ret){
		var w=xThis.m_CanvasW;
		var h=xThis.m_CanvasH;
		
		var s=xThis.m_Step;
		var ret=(p_Ret===undefined)?new Array():p_Ret;
		var i,iC=xThis.m_Tiles.length;
		for(i=0;i<iC;i++){
			var t=xThis.m_Tiles[i];
			var crd=t.GetIsometricPosition();
			var x=crd[0];
			var y=crd[1];
			if(x>-s&&x<w+s){
				if(y>-s&&y<h+s){
					ret.push(t);
				}
			}
		}
		return ret;
	}
	this.GetTilesInViewportOnLayer=function(p_Layer,p_Ret){
		var w=xThis.m_CanvasW;
		var h=xThis.m_CanvasH;
		
		p_Ret.length=0;
		
		var s=xThis.m_Step;
		var ret=(p_Ret===undefined)?new Array():p_Ret;
		var i,iC=xThis.m_Tiles.length;
		for(i=0;i<iC;i++){
			var t=xThis.m_Tiles[i];
			if(t.m_Layer!==p_Layer){continue;}
			var crd=t.GetIsometricPosition();
			var x=crd[0];
			var y=crd[1];
			if(x>-s&&x<w+s){
				if(y>-s&&y<h+s){
					ret.push(t);
				}
			}
		}
		return ret;
	}
	this.GetLayer=function(p_Layer){
		var ret=new Array();
		var i,iC=xThis.m_Tiles.length;
		for(i=iC;i--;){
			var t=xThis.m_Tiles[i];
			if(t.m_Layer==p_Layer){
				ret.push(t);
			}
		}
		return ret;
	}
	this.WorldToIsometric=function(p_X,p_Y,p_Scale){
		var angle=-45;
		angle=angle*Math.PI/180;
		var x=p_X,y=p_Y;
		var cx=x*Math.cos(angle)-y*Math.sin(angle);
		var cy=y*Math.cos(angle)+x*Math.sin(angle);
		return [(cx*p_Scale),((cy*p_Scale))];
	}
	this.FillWith=function(p_Wall){
		var col=0,row=0;
		var mW=xThis.m_MapWidth;
		var mH=xThis.m_MapHeight;
		var tS=xThis.m_Step;
		var wall=(p_Wall===undefined)?0:p_Wall;
		var i,iC=((mW/tS)*(mH/tS));
		for(i=0;i<iC;i++){
			xThis.NewTile(col,row,wall,0);
			row=((i+1)%((mW/tS))==0&&i!=0)?row+1:row;
			col=((i+1)%((mH/tS))==0&&i!=0)?0:col+1;
		}
	}
	this.LoadFromBitmap=function(p_Src,p_Callback){
		var mapdata=new Image();
		mapdata.onload=function(){
			var context=document.getElementById('canvas').getContext('2d');
			var mapWidth=xThis.m_MapWidth;
			var mapHeight=xThis.m_MapHeight;
			var tileS=xThis.m_Step;
			
			context.drawImage(mapdata,0,0,mapdata.width,mapdata.height,0,0,mapdata.width,mapdata.height);
			var imgData=context.getImageData(0,0,mapdata.width,mapdata.height);
			var d=imgData.data;
			context.fillStyle='#000000';
			context.fillRect(0,0,mapWidth,mapHeight);
			//var colorKeys=[0,0xff0000/* ,0x00ff00,0x0000ff,0xffff00,0xff00ff,0x00ffff,0xee0000 */];
			var layerFilter=[16];
			var tilevariants=[[0,18],[1,1,1,1,16,17,17,17,17]];
			var row=0,col=0;
			var cnt=0;
			var i,iC=d.length;
			var step=4;
			var w=mapdata.width*4;
			var h=mapdata.height*4;
			for(i=0;i<iC;i+=step){
				var r=parseInt((i)/w);
				
				var low=r*w;
				var high=(r+1)*w;
				var red=d[i];
				var wall=0;
				
				if(red<2){red=0;}
				
				if(red>0){
					if(i-w>=0&&d[i-w]>10){
						wall+=1;
					}
					if(i+step<high&&d[i+step]>10){
						wall+=2;
					}
					if(i+w<w*h&&d[i+w]>10){
						wall+=4;
					}	
					if(i-step>=low&&d[i-step]>10){
						wall+=8;
					}
				}else{
					var sp=i-w;
					if(i-w>=0&&d[i-w]>10){
						var idx=i-w;
						var r1=Math.round((idx)/w);
						var low1=r1*w;
						var high1=(r1+1)*w;
						if((idx+step<high1&&d[idx+step]>10)||(idx-step>=low1&&d[idx-step]>10)){
							wall+=1;
						}
					}
				}
				
				var variant=tilevariants[wall]
				if(variant!=undefined){
					if(tilevariants[wall].length>0){
						var idx=Math.round(Math.random()*(tilevariants[wall].length-1));
						wall=tilevariants[wall][idx];
					}
				}
				xThis.NewTile(col,row,wall,(layerFilter.indexOf(wall)==-1)?0:2);
				row=((cnt+1)%(mapWidth/tileS)==0&&cnt!=0)?row+1:row;
				col=((cnt+1)%(mapHeight/tileS)==0&&cnt!=0)?0:col+1;
				cnt++;
			}
			p_Callback();
		}
		mapdata.src=p_Src;
	}
	this.SolvePath=function(p_StartTile,p_EndTile){
		var man=Math.abs(p_EndTile.Row()-p_StartTile.Row()+p_EndTile.Col()-p_StartTile.Col());
		if(p_EndTile.m_Wall!=0){return [];}
		
		var open=[{t:p_StartTile,g:0,h:man,p:null}];
		var closed=[];
		var sortFunc=function(a,b){
			return (a.g+a.h<b.g+b.h);
		}
		var Gajwc=xThis.GetAdjecentWithCost;
		var foundEnd=false;
		var sanity=800;
		while(open.length>0&&sanity--){
			open.sort(sortFunc);
			
			var ct=open.pop();
			closed.push(ct);
			if(ct.t==p_EndTile){foundEnd=true;break;}
			var adj=Gajwc(ct.t);
			var i,iC=adj.length;
			for(i=iC;i--;){
				var no=adj[i];
				if(no.t.m_Wall>0){continue;}
				var j,jC=open.length;
				var found=false;
				for(j=jC;j--;){
					if(open[j].t==no.t){ //already in open
						found=true;
						if(open[j].g>ct.g+no.g){
							open[j].p=ct;
						}
						break;
					}
				}
				jC=closed.length;
				for(j=jC;j--;){
					if(closed[j].t==no.t){
						found=true;
						break;
					}
				}
				if(!found){
					var he=Math.abs(no.t.Row()-p_EndTile.Row()+no.t.Col()-p_EndTile.Col());
					open.push({t:no.t,g:no.g+ct.g,h:he,p:ct});
				}
			}
		}
		if(foundEnd){
			var path=[];
			var curr=closed.pop();
			while(curr.p!==null){
				path.push(curr.t);
				curr=curr.p;
			}
			
			return path;
		}
		console.log("Could not find path");
		return [];
		
	}
}
window.TileManager=new TileManager(8000,8000,80);