var Chat=function(){
	this.m_Canvas=document.getElementById('chat');
	this.m_Ctx=this.m_Canvas.getContext('2d');
	this.m_Messages=[];
	this.m_SystemMessages=[];
	this.m_Names=[];
	this.m_Alpha=0;
	this.m_MaxMessages=30;
	this.m_LineHeight=16;
	this.m_CurrentString="";
	this.m_ChatVisible=false;
	this.m_AnimationID=0;
	var xThis=this;
	
	Keyboard.RegisterEventListener('keypress', function(p_event){
		var w=p_event.which;
<<<<<<< HEAD
=======
		
>>>>>>> parent of fa8c75e... Entities, collisions and movement
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
		var idx=p_msg.indexOf(':');
		var n, m;
		if(idx==-1){
			n="System:";
			m=p_msg;
			xThis.m_Names.push(n);
			xThis.m_Messages.push(m);
		}else{
			n=p_msg.substr(0,idx);
			m=p_msg.substr(idx);
			xThis.m_Names.push(n);
			xThis.m_Messages.push(m);
			if(xThis.m_Messages.length>xThis.m_MaxMessages){
				xThis.m_Messages.splice(0,1);
			}
			if(xThis.m_Names.length>xThis.m_MaxMessages){
				xThis.m_Names.splice(0,1);
			}
		}
		
<<<<<<< HEAD
=======
		if(xThis.m_Alpha<=0){
			requestAnimationFrame(xThis.fadeOut);
		}
>>>>>>> parent of fa8c75e... Entities, collisions and movement
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
	
<<<<<<< HEAD
	this.Render=function(){
		if(xThis.m_Alpha<0.01){xThis.m_Alpha=0;}
=======
	this.render=function(){
>>>>>>> parent of fa8c75e... Entities, collisions and movement
		if(xThis.m_Alpha<=0&&xThis.m_ChatVisible==false){
			requestAnimationFrame(xThis.render);
			return;
		}
<<<<<<< HEAD
	//	ctx.clearRect(0,0,xThis.m_Canvas.width,xThis.m_Canvas.height);
	//	var w=xThis.m_Canvas.width;
		
		var x=2,y=(xThis.m_Canvas.height-105-(xThis.m_LineHeight*2));
=======
		
		var ctx=xThis.m_Ctx;
		var w=xThis.m_Canvas.width;
		var h=xThis.m_Canvas.height-(xThis.m_LineHeight*2);
		var x=2,y=h-xThis.m_LineHeight;
>>>>>>> parent of fa8c75e... Entities, collisions and movement
		var i,iC=xThis.m_Messages.length;
		xThis.m_Ctx.font="16px Arial";
		
<<<<<<< HEAD
		xThis.m_Ctx.globalAlpha=xThis.m_Alpha;
=======
		ctx.globalAlpha=xThis.m_Alpha;
		ctx.clearRect(0,0,xThis.m_Canvas.width,xThis.m_Canvas.height);
		
>>>>>>> parent of fa8c75e... Entities, collisions and movement
		if(xThis.m_ChatVisible){
			xThis.m_Ctx.fillStyle="#FF1505";
			xThis.m_Ctx.fillText(xThis.m_CurrentString,2,y+(xThis.m_LineHeight*1.5));
		}
		iC=xThis.m_Names.length;
		xThis.m_Ctx.fillStyle="#AE0000";
		xThis.m_Ctx.font="bold 16px Arial";
		for(i=iC;i--;){  //i split this up to avoid having to change font style for each message, es ist costly
			xThis.m_Ctx.fillText(xThis.m_Names[i],x, y);
			y-=xThis.m_LineHeight;
			x=2;
		}
<<<<<<< HEAD
		y=(xThis.m_Canvas.height-105-(xThis.m_LineHeight*2));
		xThis.m_Ctx.fillStyle='#FF1505';
		xThis.m_Ctx.font="16px Arial";
		for(i=iC;i--;){
			x=(xThis.m_Names[i].length*10);
			xThis.m_Ctx.fillText(xThis.m_Messages[i],x, y);
			y-=xThis.m_LineHeight;
		}
		xThis.m_Ctx.globalAlpha=1;
=======
		ctx.globalAlpha=1;
		requestAnimationFrame(xThis.render);
>>>>>>> parent of fa8c75e... Entities, collisions and movement
	}
	
	requestAnimationFrame(this.render);
	
	document.addEventListener('NetMessage',this.OnMessage,false);
}

window.Chat=new Chat();