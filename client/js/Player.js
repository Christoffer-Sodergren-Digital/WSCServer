var Player=function(p_AnimSrc){
	this.m_x=0;
	this.m_y=0;
	this.m_Src=p_AnimSrc;
	this.m_Anim=new Animation();
	this.m_Loaded=false;
	
	var xThis=this;
	this.Init=function(){
		xThis.m_Anim.Load(xThis.m_Src,function(){
			xThis.m_Loaded=true;
		});
	}
}