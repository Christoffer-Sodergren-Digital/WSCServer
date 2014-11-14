var SkyNet=function(p_Host){
	this.m_Socket=new WebSocket(p_Host);
	this.m_Loggedin=false;
<<<<<<< HEAD
	this.m_Closed=true;
	/* this.m_DataReader=new FileReader();
	this.m_SendReader=new FileReader(); */
	this.m_Busy=false;
	this.m_SnapshotIdx=0;
=======
	this.m_Closed=false;
>>>>>>> parent of fa8c75e... Entities, collisions and movement
	var s=this.m_Socket;
	
<<<<<<< HEAD
	//this.onload=
	//this.onsendload=
=======
	var xThis=this;
>>>>>>> parent of fa8c75e... Entities, collisions and movement
	s.onopen=function(e){
		xThis.m_Closed=false;
		var evnt=CreateCustomEvent('NetConnect',e);
		document.dispatchEvent(evnt);
		Chat.AddMessage("Connected");
	}
	s.onclose=function(e){
		var evnt=CreateCustomEvent('NetDisconnect',e);
		document.dispatchEvent(evnt);
		Chat.AddMessage("Disconnected");
	}
<<<<<<< HEAD
	s.onmessage=onnetmessage;
	
=======
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
>>>>>>> parent of fa8c75e... Entities, collisions and movement
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
	this.Send=function(p_data,p_type){
		if( Object.prototype.toString.call( p_data ) === '[object Array]' ) {
			p_data=p_data.join('');
		}
<<<<<<< HEAD
		var bb=new Blob([p_data],{type: 'application/octet-binary'});
=======
		var bb=new Blob([p_data],{type: 'text/plain'});
>>>>>>> parent of fa8c75e... Entities, collisions and movement
		var fr=new FileReader();
		fr.onload=function(e){
			var buff=new Uint8Array(e.target.result);
			buff[0]=buff[0]-48; //number hack TODO: fix this so that it is always the right value
<<<<<<< HEAD
			if(p_type!==undefined){
				if(p_type=='numbers'){
					var i,iC=buff.length;
					for(i=1;i<iC;i++){
						buff[i]-=48;
					}
				}
			}
			if(buff.length<=1){return;}
			 try{ s.send(buff.buffer); } catch(ex){ console.log(ex); }
			 fr.onload=0;
			 fr=0;
=======
			if(buff.length<=1){return;}
			 try{ s.send(buff.buffer); } catch(ex){ console.log(ex); }
>>>>>>> parent of fa8c75e... Entities, collisions and movement
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
window.SkyNet=new SkyNet("ws://127.0.0.1:10444");
function onnetmessage(p_msg){
	var filereader=new FileReader();
	filereader.onload=function(e){
		var view=new Uint8Array(filereader.result);
		var appOpCode=view[0];
		var data=view.subarray(1);  //cg monster
		if(SkyNet.m_Loggedin===false){
			if(appOpCode!=0x02){return;}
			SkyNet.HandleLogin(data);
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
			var obj={d:data,idx:Date.now()};
			document.dispatchEvent(CreateCustomEvent('NetSnapshot',obj));
		}
		if(appOpCode==0x05){//player disconnect
			var obj={d:data}
			document.dispatchEvent(CreateCustomEvent('NetPlayerDisconnect',obj));
		}
		if(appOpCode==0x06){
			var obj={d:data};
			document.dispatchEvent(CreateCustomEvent('NetInventory',obj));
		}
		if(appOpCode==0x08){
			var obj={d:data};
			document.dispatchEvent(CreateCustomEvent('NetFullSnapshot',obj));
		}
		filereader.onload=0;
		filereader=0;
	}
	filereader.readAsArrayBuffer(p_msg.data);
}
