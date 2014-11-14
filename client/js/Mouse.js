var Mouse=function(){
	this.m_EventListeners=[];
	var xThis=this;
	
	var canvas=document.getElementById('canvas');
	if(canvas===undefined){return;}
	
	this.RegisterEventListener=function(p_Type,p_Callback){
		xThis.m_EventListeners.push({type:p_Type,callback:p_Callback});
	}
	
	canvas.onclick=function(event){
		var i,iC=xThis.m_EventListeners.length;
		for(i=0;i<iC;i++){
			if(xThis.m_EventListeners[i].type=='click'){
				xThis.m_EventListeners[i].callback(event);
			}
		}
		return false;
	}
	canvas.onmousedown=function(event){
		var i,iC=xThis.m_EventListeners.length;
		for(i=0;i<iC;i++){
			if(xThis.m_EventListeners[i].type=='mousedown'){
				xThis.m_EventListeners[i].callback(event);
			}
		}
		return false;
	}
	canvas.onmouseup=function(event){
		var i,iC=xThis.m_EventListeners.length;
		for(i=0;i<iC;i++){
			if(xThis.m_EventListeners[i].type=='mouseup'){
				xThis.m_EventListeners[i].callback(event);
			}
		}
		return false;
	}
	canvas.onmousemove=function(event){
		var i,iC=xThis.m_EventListeners.length;
		for(i=0;i<iC;i++){
			if(xThis.m_EventListeners[i].type=='mousemove'){
				xThis.m_EventListeners[i].callback(event);
			}
		}
		return false;
	}
}

window.Mouse=new Mouse();