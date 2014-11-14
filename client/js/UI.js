var BaseWidget=function(p_Name,p_Properties){
	this.m_Name=p_Name;
	this.m_Properties=p_Properties;
	this.m_Children=[];
	this.m_Parent=0;
	this.m_x=0;
	this.m_y=0;
	this.m_w=0;
	this.m_h=0;
	this.m_bgcolor="#ff0000";
	this.m_imgsrc="";
	this.m_Img=0;
	this.m_ImgLoaded=true;
	this.m_EventHandlers=[];
	this.m_Text=0;
	this.m_TextColor="#ffffff";
	this.m_Font="12px Arial";
	this.m_TextShadow=0;
	this.m_TextShadowX=0;
	this.m_TextShadowY=0;
	this.m_Visible=true;
	this.m_CustomData=0;
	
	var xThis=this;
	this.ParseProperties=function(p_context){
		var p=xThis.m_Properties;
		xThis.m_x=(p.hasOwnProperty("x"))?p.x:0;
		xThis.m_y=(p.hasOwnProperty("y"))?p.y:0;
		xThis.m_w=(p.hasOwnProperty("w"))?p.w:0;
		xThis.m_h=(p.hasOwnProperty("h"))?p.h:0;
		
		
		if(xThis.m_y=="bottom"){
			var cnvs=p_context.canvas;
			if(xThis.m_Parent){
				xThis.m_y=xThis.m_Parent.m_y+xThis.m_Parent.m_h-xThis.m_h;
			}else{
				xThis.m_y=cnvs.height-xThis.m_h;
			}
		}else{
			var pP=xThis.m_Parent;
			if(pP){
				xThis.m_x+=pP.m_x;
				xThis.m_y+=pP.m_y;
			}
		}

		xThis.m_Visible=(p.hasOwnProperty("visible"))?p.visible:true;
		
		xThis.m_bgcolor=(p.hasOwnProperty("bgcolor"))?p.bgcolor:0;
		xThis.m_imgsrc=(p.hasOwnProperty("imgsrc"))?p.imgsrc:"";
		if(xThis.m_imgsrc!=""){
			if(!xThis.m_Img){
				xThis.m_ImgLoaded=false;
				xThis.m_Img=new Image();
				xThis.m_Img.onload=function(e){
					xThis.m_ImgLoaded=true;
				}
				xThis.m_Img.src=xThis.m_imgsrc;
			}
		}
		
		xThis.m_Text=(p.hasOwnProperty("text"))?p.text:0;
		xThis.m_TextColor=(p.hasOwnProperty("textcolor"))?p.textcolor:"#ffffff";
		xThis.m_Font=(p.hasOwnProperty("font"))?p.font:12;
		xThis.m_TextShadow=(p.hasOwnProperty("textshadow"))?p.textshadow:"#000000";
		xThis.m_TextShadowX=(p.hasOwnProperty("textshadowx"))?p.textshadowx:0;
		xThis.m_TextShadowY=(p.hasOwnProperty("textshadowy"))?p.textshadowy:0;
		
		
		
	}
	this.SetCustomData=function(p_Data){
		xThis.m_CustomData=p_Data;
	}
	this.RegisterEventListener=function(p_event,p_handler){
		xThis.m_EventHandlers[p_event]=p_handler;
	}
	this.Recalculate=function(p_ctx){
		xThis.ParseProperties(p_ctx);
		var i,iC=xThis.m_Children.length;
		for(i=0;i<iC;i++){
			xThis.m_Children[i].Recalculate(p_ctx);
		}
	}
	this.MouseHitTest=function(e){
		var x=e.offsetX;
		var y=e.offsetY;
		if(xThis.m_Visible){
			if(x>=xThis.m_x&&x<=xThis.m_x+xThis.m_w){
				if(y>=xThis.m_y&&y<=xThis.m_y+xThis.m_h){
					if(xThis.m_EventHandlers['click']!==undefined){
						return true;
					}
					var i,iC=xThis.m_Children.length;
					for(i=iC;i--;){
						if(xThis.m_Children[i].MouseHitTest(e)){
							return true;
						}
					}
				}
			}
		}
		return false;
	}
	this.Render=function(p_ctx,p_shallow){
		if(!xThis.m_ImgLoaded){return;}
		if(!xThis.m_Visible){return;}
		if(xThis.m_Img){
			p_ctx.drawImage(xThis.m_Img,xThis.m_x,xThis.m_y);
		}else if(xThis.m_bgcolor){
			p_ctx.fillStyle=xThis.m_bgcolor;
			p_ctx.fillRect(xThis.m_x,xThis.m_y,xThis.m_w,xThis.m_h);
		}
		if(p_shallow===undefined){
			var i,iC=xThis.m_Children.length;
			for(i=0;i<iC;i++){
				xThis.m_Children[i].Render(p_ctx);
			}
		}
		if(xThis.m_Text){
			p_ctx.fillStyle=xThis.m_TextColor;
			p_ctx.font=xThis.m_Font;
			p_ctx.textBaseline="top";
			if(xThis.m_TextShadow){
				p_ctx.shadowColor=xThis.m_TextShadow;
				p_ctx.shadowOffsetX=xThis.m_TextShadowX;
				p_ctx.shadowOffsetY=xThis.m_TextShadowY;
			}
			p_ctx.fillText(xThis.m_Text,xThis.m_x,xThis.m_y);
			p_ctx.shadowColor='#000000';
			p_ctx.shadowOffsetX=0;
			p_ctx.shadowOffsetY=0;
		}
		if(p_shallow===undefined){
			var i,iC=xThis.m_Children.length;
			for(i=0;i<iC;i++){
				xThis.m_Children[i].Render(p_ctx);
			}
		}
	}
}
window.BaseWidget=BaseWidget;

var UI=function(){
	this.m_Widgets=[];
	this.m_Root=0;
	var xThis=this;
	
	Mouse.RegisterEventListener('click',function(event){
		var i, iC=xThis.m_Widgets.length;
		for(i=0;i<iC;i++){
			if(xThis.m_Widgets[i].m_EventHandlers['click']!==undefined){
				if(xThis.m_Widgets[i].m_Visible){
					if(window[xThis.m_Widgets[i].m_EventHandlers['click']]){
						window[xThis.m_Widgets[i].m_EventHandlers['click']](event,xThis.m_Widgets[i],xThis);
					}
				}
			}
		}
	});
	Keyboard.RegisterEventListener('keypress',function(event){
		var i, iC=xThis.m_Widgets.length;
		for(i=0;i<iC;i++){
			if(xThis.m_Widgets[i].m_EventHandlers['keypress']!==undefined){
				if(xThis.m_Widgets[i].m_Visible){
					if(window[xThis.m_Widgets[i].m_EventHandlers['keypress']]){
						window[xThis.m_Widgets[i].m_EventHandlers['keypress']](event,xThis.m_Widgets[i],xThis);
					}
				}
			}
		}
	});
	this.MouseHitTest=function(e){
		return xThis.m_Root.MouseHitTest(e);
	}
	this.Load=function(p_Path,p_context){
		En.post('assets.php',{t:p_Path},function(p_data){
			var canvas=p_context.canvas;
			var root=new BaseWidget("root", {x:0,y:0,w:canvas.width,h:canvas.height});
			root.ParseProperties(p_context);
			xThis.m_Root=root;
			var json=JSON.parse(p_data);
			
			var meta=0;
			if(json.hasOwnProperty('meta')){
				if(json.meta.hasOwnProperty('behaviourasset')){
					var src=json.meta.behaviourasset;
					var domel=document.createElement('script');
					document.body.appendChild(domel);
					domel.onload=function(){
						var i,iC=xThis.m_Widgets.length;
						for(i=0;i<iC;i++){
							if(xThis.m_Widgets[i].m_EventHandlers["postload"]!==undefined){
								window[xThis.m_Widgets[i].m_EventHandlers["postload"]]({},xThis.m_Widgets[i],xThis);
							}
						}
					}
					domel.src=src;
				}
			}
			if(json.elements===undefined){
				return;
			}
			var o;
			for(o in json.elements){
				var p={};
				var obj=json.elements[o];
				if(obj.hasOwnProperty('p')){
					p=obj.p;
				}
				var nw=new BaseWidget(o,p);
				nw.ParseProperties(p_context);
				xThis.m_Root.m_Children.push(nw);
				if(obj.hasOwnProperty('events')){
					var inner;
					for(inner in obj.events){
						nw.RegisterEventListener(inner, obj.events[inner]);
					}
				}
				if(obj.hasOwnProperty('cw')){
					xThis.ParseChildren(obj.cw,nw,p_context);
				}
				
				xThis.m_Widgets.push(nw);
			}
		});
	}
	this.Recalculate=function(p_ctx){
		xThis.m_Root.Recalculate(p_ctx);
	}
	this.ParseChildren=function(p_Children,p_Parent,p_context){
		var o;
		for(o in p_Children){
			var p={};
			var obj=p_Children[o];
			if(obj.hasOwnProperty('p')){
				p=obj.p;
			}
			var nw=new BaseWidget(o,p);
			if(obj.hasOwnProperty('events')){
				var inner;
				for(inner in obj.events){
					nw.RegisterEventListener(inner, obj.events[inner]);
				}
			}
			
			nw.m_Parent=p_Parent;
			nw.ParseProperties(p_context);
			p_Parent.m_Children.push(nw);
			if(obj.hasOwnProperty('cw')){
				xThis.ParseChildren(obj.cw,nw);
			}
			xThis.m_Widgets.push(nw);
		}
	}
	this.Render=function(p_ctx){
		if(!xThis.m_Root){return;}
		xThis.m_Root.Render(p_ctx);
	}
	this.GetWidgetFromName=function(p_name){
		var i, iC=xThis.m_Widgets.length;
		for(i=0;i<iC;i++){
			if(xThis.m_Widgets[i].m_Name==p_name){
				return xThis.m_Widgets[i];
			}
		}
		return null;
	}
}
var ui=new UI();
window.UI=ui;