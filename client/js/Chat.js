var Chat=function(){
	this.m_Canvas=document.getElementById('chat');
	this.m_Ctx=this.m_Canvas.getContext('2d');
	this.m_Messages=[];
	this.m_Alpha=0;
	this.m_LineHeight=16;
	this.m_CurrentString="";
	this.m_ChatVisible=false;
	this.m_AnimationID=0;
	var xThis=this;
	
	Keyboard.RegisterEventListener('keypress', function(p_event){
		var w=p_event.which;
		
		if(w==13){ //return
			if(xThis.m_ChatVisible){
				if(xThis.m_CurrentString.length>0){
					var data=0x01+xThis.m_CurrentString.substr(1);
					SkyNet.Send(data);
					xThis.m_CurrentString="";
					xThis.m_ChatVisible=false;
					xThis.fadeOut();
				}else{
					xThis.m_ChatVisible=false;
					xThis.fadeOut();
				}
			}else{
				xThis.m_ChatVisible=true;
				xThis.m_CurrentString=">";
				xThis.m_Alpha=1;
			}
		}else{
			if(xThis.m_ChatVisible){
				if(w==8){return;} //firefox fix
				var c=String.fromCharCode(w);
				if(p_event.shiftKey===false){
					c=c.toLowerCase(c);
				}
				xThis.m_CurrentString+=c;
			}
		}
	});
	Keyboard.RegisterEventListener('keydown',function(p_event){
		var w=p_event.which;
		if(w==8){ //backspace
			if(xThis.m_CurrentString.length>1){
				xThis.m_CurrentString=xThis.m_CurrentString.substring(0,xThis.m_CurrentString.length-1);
			}
			return;
		}
	});
	
	this.OnMessage=function(p_msg){
		xThis.AddMessage(p_msg.detail);
	}
	
	this.AddMessage=function(p_msg){
		var ctx=xThis.m_Ctx;
		xThis.m_Messages.push(p_msg);
		
		if(xThis.m_Messages.length>50){
			xThis.m_Messages.splice(0,1);
		}
		
		if(xThis.m_Alpha<=0){
			requestAnimationFrame(xThis.fadeOut);
		}
		xThis.m_Alpha=1;
	}
	
	this.fadeOut=function(){
		if(xThis.m_ChatVisible==false){
			xThis.m_Alpha-=0.002;
			if(xThis.m_Alpha<=0){
				xThis.m_AnimationID=0;
			}
		}
		if(xThis.m_Alpha>0){
			if(xThis.m_AnimationID!=0){
				cancelAnimationFrame(xThis.m_AnimationID);
			}
			xThis.m_AnimationID=requestAnimationFrame(xThis.fadeOut);
		}
	}
	
	this.render=function(){
		if(xThis.m_Alpha<=0&&xThis.m_ChatVisible==false){
			requestAnimationFrame(xThis.render);
			return;
		}
		
		var ctx=xThis.m_Ctx;
		var w=xThis.m_Canvas.width;
		var h=xThis.m_Canvas.height-(xThis.m_LineHeight*2);
		var x=2,y=h-xThis.m_LineHeight;
		var i,iC=xThis.m_Messages.length;
		ctx.font="16px Arial";
		
		ctx.globalAlpha=xThis.m_Alpha;
		ctx.clearRect(0,0,xThis.m_Canvas.width,xThis.m_Canvas.height);
		
		if(xThis.m_ChatVisible){
			ctx.fillStyle="#FF1505";
			ctx.fillText(xThis.m_CurrentString,2,xThis.m_Canvas.height-xThis.m_LineHeight);
		}
		
		for(i=iC;i--;){
			var sMsg=xThis.m_Messages[i];
			var iNameIdx=sMsg.indexOf(':');
			if(iNameIdx==-1){ //system  message
				ctx.fillStyle="#ffff00";
				ctx.font="18px Arial";
				ctx.fillText(sMsg,x, y);
				y-=xThis.m_LineHeight;
				x=2;
				continue;
			}
			var name=sMsg.substr(0, iNameIdx);
			var content=sMsg.substr(iNameIdx);
			ctx.fillStyle="#AE0000";
			ctx.font="18px Arial";
			ctx.fillText(name,x, y);
			x+=name.length*10;
			ctx.fillStyle='#FF1505';
			ctx.font="16px Arial";
			ctx.fillText(content,x, y);
			y-=xThis.m_LineHeight;
			x=2;
		}
		ctx.globalAlpha=1;
		requestAnimationFrame(xThis.render);
	}
	
	requestAnimationFrame(this.render);
	
	document.addEventListener('NetMessage',this.OnMessage,false);
}

window.Chat=new Chat();