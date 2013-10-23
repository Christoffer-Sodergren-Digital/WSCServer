

var Animation=function(p_Src){
	this.m_Ctx=document.getElementById('canvas').getContext('2d');
	this.m_currentFrame=0;
	this.m_currentAnim=0;
	this.m_AnimationRate=200;
	this.m_x=0;
	this.m_y=0;
	this.m_Animations=[];
	/* this.m_FrameW=40; //fix this to be loaded from server later
	this.m_FrameH=40; */
	this.m_Loaded=false;
	this.m_TimeIncrement=0;
	this.m_SpriteSheet=null;
	this.m_LastFrame=0;
	this.m_Stopped=false;
	
	var xThis=this;
	this.Load=function(p_Src,p_Callback){
		xThis.m_SpriteSheet=new Image();
		xThis.m_SpriteSheet.onload=function(){
			
			if(p_Callback!==undefined){p_Callback();}
		}
		xThis.m_SpriteSheet.src=p_Src;
	}
	this.SetAnimationData=function(p_data){
		xThis.m_Animations=p_data;
		xThis.m_Loaded=true;
	}
	this.Animate=function(p_Delta){
		if(xThis.m_Stopped||!xThis.m_Loaded){return;}
		xThis.m_TimeIncrement+=p_Delta;
		
		if(xThis.m_TimeIncrement>xThis.m_AnimationRate){
			xThis.m_currentFrame++;
		//	l(xThis.m_Animations[xThis.m_currentAnim].length);
			if(xThis.m_currentFrame>=xThis.m_Animations[xThis.m_currentAnim].length){
				xThis.m_currentFrame=0;
			}
			//xThis.m_LastFrame=d;
			xThis.m_TimeIncrement=0;
		}
	}
	this.Render=function(){
		if(!xThis.m_Loaded){return;}
		if(xThis.m_Animations.length<=0){return;}
		
		xThis.m_Ctx.drawImage(xThis.m_SpriteSheet,
			xThis.m_Animations[xThis.m_currentAnim][xThis.m_currentFrame].x,
			xThis.m_Animations[xThis.m_currentAnim][xThis.m_currentFrame].y,
			xThis.m_Animations[xThis.m_currentAnim][xThis.m_currentFrame].w,
			xThis.m_Animations[xThis.m_currentAnim][xThis.m_currentFrame].h,
			xThis.m_x-(fw*0.5),
			xThis.m_y,
			fw,
			fh);
	}
	this.Stop=function(){
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