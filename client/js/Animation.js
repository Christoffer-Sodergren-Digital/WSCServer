

var Animation=function(p_Src){
	this.m_Ctx=document.getElementById('canvas').getContext('2d');
	this.m_currentFrame=0;
	this.m_currentAnim=0;
	this.m_AnimationRate=100;
	this.m_x=0;
	this.m_y=0;
	this.m_FrameW=8; //fix this to be loaded from server later
	this.m_FrameH=11;
	this.m_Loaded=false;
	this.m_IntervalHandle=-1;
	this.m_SpriteSheet=null;
	this.m_LastFrame=0;
	this.m_Stopped=false;
	
	var xThis=this;
	this.Load=function(p_Src,p_Callback){
		xThis.m_SpriteSheet=new Image();
		xThis.m_SpriteSheet.onload=function(){
			xThis.m_Loaded=true;
			if(p_Callback!==undefined){p_Callback();}
		}
		xThis.m_SpriteSheet.src=p_Src;
	}
	this.Animate=function(p_Delta){
		var ctx=xThis.m_Ctx;
		if(ctx===null){return;}
		if(!xThis.m_Loaded){return;}
		if(xThis.m_Stopped){return;}
		ctx.drawImage(xThis.m_SpriteSheet,
				xThis.m_currentFrame*xThis.m_FrameW,
				(xThis.m_currentAnim*xThis.m_FrameH),
				xThis.m_FrameW,
				xThis.m_FrameH,
				xThis.m_x+xThis.m_FrameW/2,
				xThis.m_y-xThis.m_FrameH/2,
				xThis.m_FrameW,
				xThis.m_FrameH);
		var d=p_Delta;
		if(d-xThis.m_LastFrame>xThis.m_AnimationRate){
			xThis.m_currentFrame++;
			if(xThis.m_currentFrame>5){
				xThis.m_currentFrame=0;
			}
			
			xThis.m_LastFrame=d;
		}
	}
	this.Stop=function(){
		/* clearInterval(xThis.m_IntervalHandle);
		xThis.m_IntervalHandle=-1; */
		xThis.m_Stopped=true;
	}
	this.SetAnimation=function(p_Anim){
		xThis.m_currentAnim=p_Anim;
		if(xThis.m_currentAnim>=4){
			xThis.m_currentAnim=0;
		}
		if(xThis.m_currentAnim<0){
				xThis.m_currentAnim=3;
		}
	}
	if(p_Src!==undefined&&p_Src.length>0){this.Load(p_Src);}
}