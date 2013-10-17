var SkyNet=function(p_Host){
	this.m_Socket=new WebSocket(p_Host);
	this.m_Loggedin=false;
	this.m_Closed=false;
	var s=this.m_Socket;
	
	var xThis=this;
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
		var reader=new FileReader();
		reader.addEventListener("loadend", function(){
			var view=new Uint8Array(reader.result);
			var appOpCode=view[0];
			var data=view.subarray(1);
			if(xThis.m_Loggedin===false){
				if(appOpCode!=0x02){return;}
				xThis.HandleLogin(data);
				return;
			}
			if(appOpCode==0x01){
				var context=document.getElementById('canvas').getContext('2d');
				var textBlob=new Blob([data]);
				var fr=new FileReader();
				fr.onload=function(e){
					var message=e.target.result;
					document.dispatchEvent(CreateCustomEvent('NetMessage',message));
				}
				fr.readAsText(textBlob);
			}
			if(appOpCode==0x03){ //snapshot
				var obj={d:data};
				document.dispatchEvent(CreateCustomEvent('NetSnapshot',obj));
			}
		});
		reader.readAsArrayBuffer(p_msg.data);
	}
	this.HandleLogin=function(msg){
		var textBlob=new Blob([msg]);
		var fr=new FileReader();
		fr.onload=function(e){
			if(e.target.result=="ok"){
				xThis.m_Loggedin=true;
				Chat.AddMessage("Logged in");
				document.dispatchEvent(CreateCustomEvent('NetLoggedIn',{}));
			}
		}
		fr.readAsText(textBlob);
	}
	this.Send=function(p_data){
		if( Object.prototype.toString.call( p_data ) === '[object Array]' ) {
			p_data=p_data.join('');
		}
		var bb=new Blob([p_data],{type: 'text/plain'});
		var fr=new FileReader();
		fr.onload=function(e){
			var buff=new Uint8Array(e.target.result);
			buff[0]=buff[0]-48; //number hack TODO: fix this so that it is always the right value
			if(buff.length<=1){return;}
			 try{ s.send(buff.buffer); } catch(ex){ console.log(ex); }
		}
		fr.readAsArrayBuffer(bb);
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
