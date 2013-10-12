var Keyboard=function(){
	this.m_Key=[];
	this.m_EventListeners=[];
	
	var xThis=this;
	this.Pressed=function(p_Key){
		if(p_Key<0|p_Key>=xThis.m_Key.length){return false;}
		return xThis.m_Key[p_Key];
	}
	this.RegisterEventListener=function(p_Type,p_Callback){
		xThis.m_EventListeners.push({type:p_Type,callback:p_Callback});
	}
	document.onkeydown=function(event){
		xThis.m_Key[event.which]=true;
		var i,iC=xThis.m_EventListeners.length;
		for(i=0;i<iC;i++){
			if(xThis.m_EventListeners[i].type=='keydown'){
				xThis.m_EventListeners[i].callback(event);
			}
		}
		//return false;
	}
	document.onkeyup=function(event){
		xThis.m_Key[event.which]=false;
		var i,iC=xThis.m_EventListeners.length;
		for(i=0;i<iC;i++){
			if(xThis.m_EventListeners[i].type=='keyup'){
				xThis.m_EventListeners[i].callback(event);
			}
		}
		//return false;
	}
}

window.Keyboard=new Keyboard();