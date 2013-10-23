var Player=function(){
	this.m_EntityID=-1;
	this.m_x=0;
	this.m_y=0;
	this.m_Src='';
	this.m_Anim=new Animation();
	this.m_Loaded=false;
	this.m_ReadBytes=0;
	this.m_LerpT=0;
	this.m_LerpSpeed=80; //complete a lerp in N milliseconds
	this.m_LerpTargets=[];
	
	var xThis=this;
	this.init=function(p_ID,p_Data){
		xThis.m_EntityID=p_ID;
		xThis.ParseData(p_Data);
		xThis.m_Loaded=true;
		En.post('assets.php',{t:'playeranim.ani'},function(e){
			var json=JSON.parse(e);
			var anim=xThis.m_Anim;
			anim.m_AnimationRate=json.rate;
			anim.SetAnimationData(json.a);
			
			xThis.m_Anim.Load(json.src,function(){
				
			});
			
		});
	}
	this.Snapshot=function(p_Data){
		xThis.ParseData(p_Data);
	}
	this.ParseData=function(p_Data){
		//2 bytes integral part of posx float, 2 bytes fractional part. Same for posy
		var idx=0;
		var i0=((p_Data[idx++]<<8)|(p_Data[idx++]&0xff));
		var f0=((p_Data[idx++]<<8)|(p_Data[idx++]&0xff));
		var s0=(i0&0x8000)?-1:1;
		i0=(i0&0x7FFF);

		var i1=((p_Data[idx++]<<8)|(p_Data[idx++]&0xff));
		var f1=((p_Data[idx++]<<8)|(p_Data[idx++]&0xff));
		var s1=(i1&0x8000)?-1:1;
		i1=(i1&0x7FFF);
		
		var x=parseFloat(i0+'.'+f0)*s0;
		var y=parseFloat(i1+'.'+f1)*s1;
		
		if(xThis.m_Loaded){
			var obj={x:x,y:y};
			xThis.m_LerpTargets.push(obj);
		}else{
			xThis.m_x=x;
			xThis.m_y=y;
		}
		
		if(xThis.m_EntityID==EntityMngr.m_PlayerID){
			TileManager.WorldOffsetX=-((xThis.m_x)-(canvas.width/2));
			TileManager.WorldOffsetY=-((xThis.m_y)-(canvas.height/2));
		}

		xThis.m_ReadBytes=idx;
	}
	this.ReadBytes=function(){return xThis.m_ReadBytes;}
	this.Render=function(p_ctx){
		if(p_ctx===undefined){return;}
		var canvas=document.getElementById('canvas');
		
		p_ctx.fillStyle="#af4444";
		if(xThis.m_EntityID==EntityMngr.m_PlayerID){
			xThis.m_Anim.m_x=canvas.width/2;
			xThis.m_Anim.m_y=canvas.height/2;
		}else{
			xThis.m_Anim.m_x=xThis.m_x+TileManager.WorldOffsetX;//+canvas.width/2;
			xThis.m_Anim.m_y=xThis.m_y+TileManager.WorldOffsetY;//+canvas.height/2;
		}
		xThis.m_Anim.Render();
	}
	this.Update=function(p_Delta){
		xThis.m_Anim.Animate(p_Delta);
		if(xThis.m_LerpTargets.length<=0){
			if(xThis.m_EntityID==EntityMngr.m_PlayerID){
				var pressed=false;
				if(Keyboard.Pressed(37)){
					xThis.m_x-=2;
					pressed=true;
				}
				if(Keyboard.Pressed(39)){
					xThis.m_x+=2;
					pressed=true;
				}
				if(Keyboard.Pressed(38)){
					xThis.m_y-=2;
					pressed=true;
				}
				if(Keyboard.Pressed(40)){
					xThis.m_y+=2;
					pressed=true;
				}
				
				if(!pressed){
					xThis.m_Anim.SetAnimation(0);
				}else{
					TileManager.WorldOffsetX=-((xThis.m_x)-(canvas.width/2));
					TileManager.WorldOffsetY=-((xThis.m_y)-(canvas.height/2));
					xThis.m_Anim.SetAnimation(1);
				}
			}
				
			return;
		}
		xThis.m_Anim.SetAnimation(1);
		var t=xThis.m_LerpT;
		xThis.m_LerpT+=p_Delta/xThis.m_LerpSpeed;
		if(xThis.m_LerpT>=1.0){
			nogcRemove(xThis.m_LerpTargets,0);
			xThis.m_LerpT=0;
			if(xThis.m_LerpTargets.length<=0){return;}
		}
		var lerpTarget=xThis.m_LerpTargets[0];
		var x0=xThis.m_x;
		var x1=lerpTarget.x;
		xThis.m_x=x0+(x1-x0)*t;

		var y0=xThis.m_y;
		var y1=lerpTarget.y;
		xThis.m_y=y0+(y1-y0)*t;
		
		if(xThis.m_EntityID==EntityMngr.m_PlayerID){
			TileManager.WorldOffsetX=-((xThis.m_x)-(canvas.width/2));
			TileManager.WorldOffsetY=-((xThis.m_y)-(canvas.height/2));
		}
	}
}
window.Player=Player;

var Torch=function(){
	
	this.m_EntityID=-1;
	this.m_Anim=new Animation();
	this.m_ReadBytes=0;
	this.m_State=1;
	this.m_x=0;
	this.m_y=0;
	this.m_LightSource=true;
	this.m_LightRadius=400;
	
	var xThis=this;
	this.init=function(p_EntityID,p_Data){
		xThis.m_EntityID=p_EntityID;
		xThis.ParseData(p_Data);
		En.post('assets.php',{t:'torchanim.ani'},function(e){
			var json=JSON.parse(e);
			var anim=xThis.m_Anim;
			anim.m_AnimationRate=json.rate;
			anim.SetAnimationData(json.a);
			
			xThis.m_Anim.Load(json.src,function(){
				
			});
			
		});
	}
	this.LightSource=function(){return xThis.m_LightSource;}
	this.Radius=function(){return xThis.m_LightRadius;}
	this.ReadBytes=function(){return xThis.m_ReadBytes;}
	this.ParseData=function(p_Data){
		var idx=0;
		xThis.m_State=p_Data[idx++];
		var i0=((p_Data[idx++]<<8)|(p_Data[idx++]&0xff));
		var f0=((p_Data[idx++]<<8)|(p_Data[idx++]&0xff));
		var s0=(i0&0x8000)?-1:1;
		i0=(i0&0x7FFF);

		var i1=((p_Data[idx++]<<8)|(p_Data[idx++]&0xff));
		var f1=((p_Data[idx++]<<8)|(p_Data[idx++]&0xff));
		var s1=(i1&0x8000)?-1:1;
		i1=(i1&0x7FFF);
		
		var x=parseFloat(i0+'.'+f0)*s0;
		var y=parseFloat(i1+'.'+f1)*s1;
		xThis.m_x=x;
		xThis.m_y=y;
		xThis.m_ReadBytes=idx;
	}
	this.Update=function(p_Delta){
		xThis.m_Anim.Animate(p_Delta);
	}
	this.Render=function(p_ctx){
		p_ctx.fillStyle="#ff0000";
		var x=xThis.m_x+TileManager.WorldOffsetX;
		var y=xThis.m_y+TileManager.WorldOffsetY;
		xThis.m_Anim.m_x=x;
		xThis.m_Anim.m_y=y;
		xThis.m_Anim.Render();
	}
}
window.Torch=Torch;