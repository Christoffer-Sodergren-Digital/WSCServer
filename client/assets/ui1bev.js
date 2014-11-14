function oninventorystubclick(event,widget){
	var x=event.layerX;
	var y=event.layerY;
	
	if(x<widget.m_x||x>widget.m_x+widget.m_w){return;}
	if(y<widget.m_y||y>widget.m_y+widget.m_h){return;}
	
	var canvas=document.getElementById('canvas');
	var ctx=canvas.getContext('2d');
	
	var inv=UI.GetWidgetFromName("inventory");
	var vis=inv.m_Properties["visible"];
	inv.m_Properties["visible"]=!vis;
	inv.Recalculate(ctx);
}
function oninventorystubkeydown(event,widget){
	var key=String.fromCharCode(event.which);
	if(key=='i'){
		var canvas=document.getElementById('canvas');
		var ctx=canvas.getContext('2d');
		
		var inv=UI.GetWidgetFromName("inventory");
		var vis=inv.m_Properties["visible"];
		inv.m_Properties["visible"]=!vis;
		inv.Recalculate(ctx);
	}
}
function oninventoryload(event,widget,ui){
		var i,iC=126;
		var step=40;
		var x=-1,y=0;
		for(i=0;i<iC;i++){
			if(i%14==0&&i!=0){
				x=0;
				y++;
			}else{
				x++;
			}
			var cell=new BaseWidget("inv_cell_"+i,{x:(x*step)+10,y:(y*step)+40,w:step,h:step,imgsrc:"images/ui_inventory_cellbg.png"});
			cell.m_Parent=widget;
			widget.m_Children.push(cell);
		}

	var data=0x06+"asd";
	SkyNet.Send(data);
	document.addEventListener('InventoryItemLoaded',function(e){
		var item=e.detail.d;
		var idx=item.m_InventoryIdx+1;
		if(idx>=widget.m_Children.length){return};
		var p=widget.m_Children[idx];
		var invtarget="images/ui_"+item.m_BaseArt+".png";
		var ico=new BaseWidget("ico"+idx,{x:10,y:10,w:20,h:20,imgsrc:invtarget});
		ico.SetCustomData(item);
		ico.RegisterEventListener('click','oninventoryitemclick');
		p.m_Children.push(ico);
		ui.m_Widgets.push(ico);
		ico.m_Parent=p;
		widget.Recalculate();
	});
}
function oninventoryitemclick(event,widget){
	var x=event.layerX;
	var y=event.layerY;
	if(x<widget.m_x||x>widget.m_x+widget.m_w){return;}
	if(y<widget.m_y||y>widget.m_y+widget.m_h){return;}
	if(widget.m_CustomData==0){return;}
	
	var item=widget.m_CustomData;
	var eq=(item.m_Equipped==0)?1:0;
	var data=[0x07,item.m_ItemID,eq];
	SkyNet.Send(data);
	
	return false;
}
