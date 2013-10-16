var SkyNet=function(p_Host){
	this.m_Socket=new WebSocket(p_Host);
	this.m_Loggedin=false;
	this.m_Closed=false;
	var s=this.m_Socket;
	
	var xThis=this;
	s.onopen=function(e){
		var event=new CustomEvent('NetConnect',{'details':e});
		document.dispatchEvent(event);
		Chat.AddMessage("Connected");
	}
	s.onclose=function(e){
		var event=new CustomEvent('NetDisconnect',{'details':e});
		document.dispatchEvent(event);
		Chat.AddMessage("Disconnected");
	}
	s.onmessage=function(p_msg){
		if(xThis.m_Closed){return;}
		var reader=new FileReader();
		reader.addEventListener("loadend", function(){
			var view=new Uint8Array(reader.result);
			var appOpCode=view[0];
			var msg=view.subarray(1);
			if(xThis.m_Loggedin===false){
				if(appOpCode!=0x02){return;}
				xThis.HandleLogin(msg);
				return;
			}
			if(appOpCode==0x01){
				var context=document.getElementById('canvas').getContext('2d');
				var textBlob=new Blob([msg]);
				var fr=new FileReader();
				fr.onload=function(e){
					var message=e.target.result;
					document.dispatchEvent(new CustomEvent('NetMessage',{'detail':message}));
				}
				fr.readAsText(textBlob);
			}
			if(appOpCode==0x03){ //snapshot
				var posx=0;
				var posy=0;
				if(view.length==3){
					posx=view[1];
					posx=view[2];
				}else if(view.length==4){ //TODO pack this better on the server side
					var low=view[1];
					var high=view[2];
					posx=high<<4|(low&0xff);
					posy=view[3];
				}else if(view.length==5){
					var low=view[2];
					var high=view[1];
					posx=high<<4|(low&0xff);
					low=view[4];
					high=view[3];
					posy=high<<4|(low&0xff);
				}
				
				var obj={x:posx,y:posy};
				document.dispatchEvent(new CustomEvent('NetSnapshot',{'detail':obj}));
			}
			/* if(appOpCode==0x04){
				console.log(view);
			} */
			
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
				document.dispatchEvent(new CustomEvent('NetLoggedIn',{}));
			}
		}
		fr.readAsText(textBlob);
	}
	this.Send=function(p_data){
		var bb=new Blob([p_data],{type: 'text/plain'});
		var fr=new FileReader();
		fr.onload=function(e){
			var buff=new Uint8Array(e.target.result);
			buff[0]=buff[0]-48;
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
