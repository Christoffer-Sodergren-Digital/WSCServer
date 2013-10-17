var Player=function(){
	this.m_EntityID=-1;
	this.m_x=0;
	/* this.m_LerpX=0;
	this.m_LerpY=0; */
	this.m_y=0;
	this.m_Src='';
	this.m_Anim=new Animation();
	this.m_Loaded=false;
	this.m_ReadBytes=0;
	this.m_LerpT=0;
	this.m_LerpSpeed=80; //complete a lerp in LerpSpeed milliseconds
	this.m_LerpTargets=[];
	
	var xThis=this;
	this.init=function(p_ID,p_Data){
		xThis.m_EntityID=p_ID;
		xThis.ParseData(p_Data);
		xThis.m_Loaded=true;
	}
	this.Snapshot=function(p_Data){
		xThis.ParseData(p_Data);
	}
	this.ParseData=function(p_Data){
		//2 bytes integral part of posx float, 2 bytes fractional part. Same for posy
		var idx=0;
		var i0=((p_Data[idx++]<<4)|(p_Data[idx++]&0xff));
		var f0=((p_Data[idx++]<<4)|(p_Data[idx++]&0xff));

		var i1=((p_Data[idx++]<<4)|(p_Data[idx++]&0xff));
		var f1=((p_Data[idx++]<<4)|(p_Data[idx++]&0xff));
		
		if(xThis.m_Loaded){
			xThis.m_LerpTargets.push({x:parseFloat(i0+'.'+f0),y:parseFloat(i1+'.'+f1)});
		}else{
			xThis.m_x=parseFloat(i0+'.'+f0);
			xThis.m_y=parseFloat(i1+'.'+f1);
		}

		xThis.m_ReadBytes=idx;
	}
	this.ReadBytes=function(){return xThis.m_ReadBytes;}
	this.Render=function(p_ctx){
		if(p_ctx===undefined){return;}
		p_ctx.fillStyle="#af4444";
		p_ctx.fillRect(xThis.m_x,xThis.m_y,20,20);
	}
	this.Update=function(p_Delta){
		//lerp
		//l(xThis.m_LerpTargets.length);
		if(xThis.m_LerpTargets.length<=0){return;}
		var t=xThis.m_LerpT;
		xThis.m_LerpT+=p_Delta/xThis.m_LerpSpeed;
		if(xThis.m_LerpT>=1.0){
			xThis.m_LerpTargets.splice(0,1);
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
	}
}

window.Player=Player;