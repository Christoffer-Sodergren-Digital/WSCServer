var Websocket=function(p_sHost){
	this.m_Connected=false;
	this.m_Socket=new Websocket(p_sHost);
	
	var xThis=this;
	this.m_Socket.onopen=function(p_Msg){
		console.log('Connected to host: '+p_sHost);
		xThis.m_Connected=true;
	}
	this.m_Socket.onclose=function(p_Msg){
		console.log('Disconnected');
		xThis.m_Connected=false;
	}
	this.m_Socket.onerror=function(p_Msg){console.log('Socket error: '+p_Msg);}
	this.m_Socket.onmessage=function(p_Data){
		
	}
}