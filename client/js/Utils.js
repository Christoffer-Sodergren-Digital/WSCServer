<<<<<<< HEAD
var l=function(a){console.log(a);}function getInternetExplorerVersion()// Returns the version of Internet Explorer or a -1// (indicating the use of another browser).{  var rv = -1; // Return value assumes failure.  if (navigator.appName == 'Microsoft Internet Explorer')  {    var ua = navigator.userAgent;    var re  = new RegExp("MSIE ([0-9]{1,}[\.0-9]{0,})");    if (re.exec(ua) != null)      rv = parseFloat( RegExp.$1 );  }  return rv;}function checkVersion(){  var msg = "You're not using Internet Explorer.";  var ver = getInternetExplorerVersion();  if ( ver > -1 )  {    if ( ver >= 8.0 )       msg = "You're using a recent copy of Internet Explorer."    else      msg = "You should upgrade your copy of Internet Explorer.";  }  l( msg );}var iev=getInternetExplorerVersion();function CreateCustomEvent(p_Name,p_Data){	if(iev>9){		var evt = document.createEvent("CustomEvent");		evt.initCustomEvent(p_Name, false, false, p_Data);		return evt;	}else if(iev==-1){ //not ie		return new CustomEvent(p_Name,{detail:p_Data});	}else{		l("You are using version "+iev+" of internet explorer, that version is not supported please upgrade to a real browser");		location.reload();	}}function nogcRemove(p_arr,p_idx){	var iNL=p_arr.length-1;	if(p_idx==p_arr.length-1){		p_arr.length=iNL;		return p_arr;	}	p_arr[p_idx]=p_arr[p_idx+1];	p_arr.length=iNL;}
=======
var l=function(a){console.log(a);}function getInternetExplorerVersion()// Returns the version of Internet Explorer or a -1// (indicating the use of another browser).{  var rv = -1; // Return value assumes failure.  if (navigator.appName == 'Microsoft Internet Explorer')  {    var ua = navigator.userAgent;    var re  = new RegExp("MSIE ([0-9]{1,}[\.0-9]{0,})");    if (re.exec(ua) != null)      rv = parseFloat( RegExp.$1 );  }  return rv;}function checkVersion(){  var msg = "You're not using Internet Explorer.";  var ver = getInternetExplorerVersion();  if ( ver > -1 )  {    if ( ver >= 8.0 )       msg = "You're using a recent copy of Internet Explorer."    else      msg = "You should upgrade your copy of Internet Explorer.";  }  l( msg );}function CreateCustomEvent(p_Name,p_Data){	var iev=getInternetExplorerVersion();	if(iev>9){		var evt = document.createEvent("CustomEvent");		evt.initCustomEvent(p_Name, false, false, p_Data);		return evt;	}else if(iev==-1){ //not ie		return new CustomEvent(p_Name,{detail:p_Data});	}else{		l("You are using version "+iev+" of internet explorer, that version is not supported please upgrade to a real browser");		location.reload();	}}
>>>>>>> parent of fa8c75e... Entities, collisions and movement
