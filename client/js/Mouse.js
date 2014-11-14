var Mouse=function(){
	this.m_EventListeners=[];
	this.m_InputBuffer=[];
	var xThis=this;
	
	var canvas=document.getElementById('canvas');
	if(canvas===undefined){return;}
	
	this.RegisterEventListener=function(p_Type,p_Callback){
		xThis.m_EventListeners.push({type:p_Type,callback:p_Callback});
	}
	this.GetInputBuffer=function(p_Buffer){
		p_Buffer.push.apply(p_Buffer,xThis.m_InputBuffer);
	}
	this.ClearBuffer=function(){
		xThis.m_InputBuffer.length=0;
	}
	document.onclick=function(event){
		var i,iC=xThis.m_EventListeners.length;
		for(i=0;i<iC;i++){
			if(xThis.m_EventListeners[i].type=='click'){
				xThis.m_EventListeners[i].callback(event);
			}
		}
		var px=event.offsetX-TileManager.WorldOffsetX;
		var py=event.offsetY-TileManager.WorldOffsetY;
		xThis.m_InputBuffer.push(1);
		
		var h0=(px>>8)&0xff;
		var l0=(px&0xff);
		var h1=(py>>8)&0xff;
		var l1=(py&0xff);
		
		xThis.m_InputBuffer.push(h0,l0,h1,l1);
	}
	document.onmousedown=function(event){
		var i,iC=xThis.m_EventListeners.length;
		for(i=0;i<iC;i++){
			if(xThis.m_EventListeners[i].type=='mousedown'){
				xThis.m_EventListeners[i].callback(event);
			}
		}
		
	}
	document.onmouseup=function(event){
		var i,iC=xThis.m_EventListeners.length;
		for(i=0;i<iC;i++){
			if(xThis.m_EventListeners[i].type=='mouseup'){
				xThis.m_EventListeners[i].callback(event);
			}
		}
		
	}
	/* canvas.onmousemove=function(event){
		var i,iC=xThis.m_EventListeners.length;
		for(i=0;i<iC;i++){
			if(xThis.m_EventListeners[i].type=='mousemove'){
				xThis.m_EventListeners[i].callback(event);
			}
		}
		return false;
	} */
}

window.Mouse=new Mouse();