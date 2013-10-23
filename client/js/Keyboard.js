var Keyboard=function(){
	this.m_Key=[];
	this.m_EventListeners=[];
	var VK_DOWN=1;
	var VK_LEFT=2;
	var VK_UP=3;
	var VK_RIGHT=4;
	this.m_BufferFilter=[{k:40,vk:VK_DOWN},{k:37,vk:VK_LEFT},{k:38,vk:VK_UP},{k:39,vk:VK_RIGHT}];
	this.m_KeyboardBuffer=[4];
	
	
	var xThis=this;
	this.Pressed=function(p_Key){
		if(p_Key<0|p_Key>=xThis.m_Key.length){return false;}
		return xThis.m_Key[p_Key];
	}
	this.RegisterEventListener=function(p_Type,p_Callback){
		xThis.m_EventListeners.push({type:p_Type,callback:p_Callback});
	}
	this.UpdateKeyboardBuffer=function(){
		var filter=xThis.m_BufferFilter;
		var i,iC=filter.length;
		for(i=iC;i--;){
			if(xThis.m_Key[filter[i].k]){
				xThis.m_KeyboardBuffer.push(filter[i].vk);
			}
		}
	}
	this.init=function(){
		document.onkeydown=function(event){
			xThis.m_Key[event.which]=true;
			var i,iC=xThis.m_EventListeners.length;
			for(i=0;i<iC;i++){
				if(xThis.m_EventListeners[i].type=='keydown'){
					xThis.m_EventListeners[i].callback(event);
				}
			}
			var vk=0;
			var tmp=xThis.m_BufferFilter;
			var w=event.which;
			var i,iC=tmp.length;
			var preventDefault=false;
			for(i=iC;i--;){
				var o=tmp[i];
				if(o.k==w){
					vk=o.vk;
					preventDefault=true;
					break;
				}
			} 
			
			if(event.which==8||preventDefault){ //prevent backspace navigation
				event.preventDefault();
				return false;
			}
		
		}
		document.onkeyup=function(event){
			xThis.m_Key[event.which]=false;
			var i,iC=xThis.m_EventListeners.length;
			for(i=0;i<iC;i++){
				if(xThis.m_EventListeners[i].type=='keyup'){
					xThis.m_EventListeners[i].callback(event);
				}
			}
		}
		document.onkeypress=function(event){
			l(event);
			var i,iC=xThis.m_EventListeners.length;
			for(i=0;i<iC;i++){
				if(xThis.m_EventListeners[i].type=='keypress'){
					xThis.m_EventListeners[i].callback(event);
				}
			}
			
		}
	}
	
	
	this.GetKeyboardBuffer=function(){
		return xThis.m_KeyboardBuffer;
	}
	this.ClearBuffer=function(){
		xThis.m_KeyboardBuffer.length=0;
		xThis.m_KeyboardBuffer.push(4);
		//xThis.m_KeyboardBuffer=[4];
	}
}

window.Keyboard=new Keyboard();