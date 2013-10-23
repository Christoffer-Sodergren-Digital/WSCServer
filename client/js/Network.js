var SkyNet=function(p_Host){
	this.m_Socket=new WebSocket(p_Host);
	this.m_Loggedin=false;
	this.m_Closed=false;
	this.m_DataReader=new FileReader();
	this.m_SendReader=new FileReader();
	this.m_Busy=false;
	var s=this.m_Socket;
	var xThis=this;
	
	this.m_DataReader.onload=function(){
		var view=new Uint8Array(xThis.m_DataReader.result);
		var appOpCode=view[0];
		var data=view.subarray(1);  //cg monster
		if(xThis.m_Loggedin===false){
			if(appOpCode!=0x02){return;}
			xThis.HandleLogin(data);
			return;
		}
		if(appOpCode==0x01){
			var textBlob=new Blob([data]);
			var fr=new FileReader();
			fr.onload=function(e){
				var message=e.target.result;
				document.dispatchEvent(CreateCustomEvent('NetMessage',message));
				fr.onload=0;
				fr=0;
			}
			fr.readAsText(textBlob);
		}
		if(appOpCode==0x03){ //snapshot
			var obj={d:data};
			document.dispatchEvent(CreateCustomEvent('NetSnapshot',obj));
		}
		if(appOpCode==0x05){//player disconnect
			var obj={d:data}
			document.dispatchEvent(CreateCustomEvent('NetPlayerDisconnect',obj));
		}
	}
	this.m_SendReader.onload=function(e){
		var buff=new Uint8Array(e.target.result);
		buff[0]=buff[0]-48; //number hack TODO: fix this so that it is always the right value
		if(buff.length<=1){return;}
		 try{ s.send(buff.buffer); } catch(ex){ console.log(ex); }
	}
	s.onopen=function(e){
		var evnt=CreateCustomEvent('NetConnect',e);
		document.dispatchEvent(evnt);
		Chat.AddMessage("Connected");
	}
	s.onclose=function(e){
		var evnt=CreateCustomEvent('NetDisconnect',e);
		document.dispatchEvent(evnt);
		Chat.AddMessage("Disconnected");
	}
	s.onmessage=function(p_msg){
		if(xThis.m_Closed){return;}
		try{
			if(xThis.m_Busy){
				setTimeout(xThis.m_DataReader.readAsArrayBuffer(p_msg.data),10);
			}else{
				xThis.m_DataReader.readAsArrayBuffer(p_msg.data);
			}
		}catch(ex){
			l(ex);
		}
	}
	this.HandleLogin=function(msg){
		var blob=new Blob([msg]);
		var fr=new FileReader();
		fr.onload=function(e){
			var view=new Uint8Array(fr.result);
			
			var msg=String.fromCharCode(view[0])+String.fromCharCode(view[1]);
			if(msg=="ok"){
				var offset=2;
				var entityID=view[offset++];
				entityID=(entityID<<8)|view[offset++];
				entityID=(entityID<<16)|view[offset++];
				entityID=(entityID<<24)|view[offset++];
				
				xThis.m_Loggedin=true;
				Chat.AddMessage("Logged in");
				document.dispatchEvent(CreateCustomEvent('NetLoggedIn',{playerid:entityID}));
			}
			fr.onload=0;
			fr=0;
		}
		fr.readAsArrayBuffer(blob);
	}
	this.Send=function(p_data){
		if( Object.prototype.toString.call( p_data ) === '[object Array]' ) {
			p_data=p_data.join('');
		}
		var bb=new Blob([p_data],{type: 'text/plain'});
		xThis.m_SendReader.readAsArrayBuffer(bb);
	}
	this.Disconnect=function(){
		if(xThis.m_Closed){return;}
		xThis.m_Closed=true;
		s.close();
		Chat.AddMessage("Connection closing, refreshing page in 2 seconds");
		setTimeout(function(){
			location.reload();
		},2000);
		
	}
}
window.SkyNet=new SkyNet("ws://mstrp2ez.no-ip.org:10444");
