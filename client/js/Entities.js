var Player=function(){
	this.m_EntityID=-1;
	this.m_x=0;
	this.m_y=0;
	this.m_Src='';
	this.m_Anim=new Animation();
	this.m_Canvas=document.getElementById('canvas');
	this.m_Loaded=false;
	this.m_ReadBytes=0;
	this.m_LerpT=0;
	this.m_LerpSpeed=30; //complete a lerp in N milliseconds
	this.m_LerpTargetX=0;
	this.m_LerpTargetY=0;
	this.m_Children=[];
	this.m_Inventory=[];
	
	var xThis=this;
	this.init=function(p_ID,p_Data){
		xThis.m_EntityID=p_ID;
		xThis.ParseData(p_Data);
		xThis.m_Loaded=true;
		En.post('assets.php',{t:'playeranim.ani'},function(e){
			var json=JSON.parse(e);
			var anim=xThis.m_Anim;
			anim.m_AnimationRate=json.rate;
			anim.SetAnimationData(json.a);
			
			xThis.m_Anim.Load(json.src,function(){
				if(xThis.m_EntityID==EntityMngr.m_PlayerID){
					var obj={d:xThis};
					document.dispatchEvent(CreateCustomEvent('PlayerLoaded',obj));
				}
			});
			
		});
		Mouse.RegisterEventListener('mousedown', xThis.onMouseDown);
	}
	this.Snapshot=function(p_Data){
		xThis.ParseData(p_Data);
	}
	this.ParseData=function(p_Data){
		//2 bytes integral part of posx float, 2 bytes fractional part. Same for posy
		var idx=0;
		var i0=((p_Data[idx++]<<8)|(p_Data[idx++]&0xff));
		var f0=((p_Data[idx++]<<8)|(p_Data[idx++]&0xff));
		var s0=(i0&0x8000)?-1:1;
		i0=(i0&0x7FFF);

		var i1=((p_Data[idx++]<<8)|(p_Data[idx++]&0xff));
		var f1=((p_Data[idx++]<<8)|(p_Data[idx++]&0xff));
		var s1=(i1&0x8000)?-1:1;
		i1=(i1&0x7FFF);
		
		var x=parseFloat(i0+'.'+f0)*s0;
		var y=parseFloat(i1+'.'+f1)*s1;
		
		xThis.m_x=x;
		xThis.m_y=y;
		
		if(xThis.m_EntityID==EntityMngr.m_PlayerID){  //Center "camera" on user avatar
			TileManager.WorldOffsetX=-((xThis.m_x)-(canvas.width/2));
			TileManager.WorldOffsetY=-((xThis.m_y)-(canvas.height/2));
		}
		
		var i,iC=p_Data[idx++]; //numitemchange
		for(i=0;i<iC;i++){ 
			var type=p_Data[idx++];
			var high=p_Data[idx++];
			var low=p_Data[idx++];
			entityid=(high<<8)|(low&0xff);
			high=p_Data[idx];
			low=p_Data[idx+1];
			var itemid=(high<<8)|(low&0xff);
			high=p_Data[idx+2];
			low=p_Data[idx+3];
			var ownerid=(high<<8)|(low&0xff);
			var j,jC=xThis.m_Inventory.length;
			var bFound=false;
			for(j=jC;j--;){
				if(xThis.m_Inventory[j].m_ItemID==itemid&&xThis.m_EntityID==ownerid){
					xThis.m_Inventory[j].ParseData(p_Data.subarray(idx));
					xThis.m_Inventory[j].Validate();
					idx+=xThis.m_Inventory[j].ReadBytes();
					bFound=true;
				}
			}
			if(!bFound){
				var ni=EntityMngr.NewEntity(type,entityid);
				ni.init(entityid,p_Data.subarray(idx));
				ni.Validate();
				idx+=ni.ReadBytes();
				xThis.m_Inventory.push(ni);
			}
		}

		xThis.m_ReadBytes=idx;
	}
	this.TargetSnapshot=function(p_Data){
		xThis.ParseTargetSnapshot(p_Data);
	}
	this.ParseTargetSnapshot=function(p_Data){
		var idx=0;
		var i0=((p_Data[idx++]<<8)|(p_Data[idx++]&0xff));
		var f0=((p_Data[idx++]<<8)|(p_Data[idx++]&0xff));
		var s0=(i0&0x8000)?-1:1;
		i0=(i0&0x7FFF);

		var i1=((p_Data[idx++]<<8)|(p_Data[idx++]&0xff));
		var f1=((p_Data[idx++]<<8)|(p_Data[idx++]&0xff));
		var s1=(i1&0x8000)?-1:1;
		i1=(i1&0x7FFF);
		
		var x=parseFloat(i0+'.'+f0)*s0;
		var y=parseFloat(i1+'.'+f1)*s1;
		
		xThis.m_LerpTargetX=x;
		xThis.m_LerpTargetY=y;

		xThis.m_ReadBytes=idx;
	}
	this.ReadBytes=function(){return xThis.m_ReadBytes;}
	this.SetChildrenAnimation=function(p_anim){
		if(xThis.m_Children.length>0){
			var i,iC=xThis.m_Children.length;
			for(i=0;i<iC;i++){
				if(xThis.m_Children[i].m_Anim!==undefined){
					xThis.m_Children[i].m_Anim.SetAnimation(p_anim);
				}
			}
		}
	}
	this.Render=function(p_ctx){
		if(p_ctx===undefined){return;}
		
		if(xThis.m_EntityID==EntityMngr.m_PlayerID){
			xThis.m_Anim.m_x=xThis.m_Canvas.width/2;
			xThis.m_Anim.m_y=xThis.m_Canvas.height/2;
		}else{
			xThis.m_Anim.m_x=xThis.m_x+TileManager.WorldOffsetX;//+canvas.width/2;
			xThis.m_Anim.m_y=xThis.m_y+TileManager.WorldOffsetY;//+canvas.height/2;
		}
		xThis.m_Anim.Render();
	}
	this.Update=function(p_Delta){
		xThis.m_Anim.Animate(p_Delta);

		var t=xThis.m_LerpT;
		xThis.m_LerpT+=p_Delta/xThis.m_LerpSpeed;
		if(xThis.m_LerpT>=1.0){
			xThis.m_LerpT=0;
			return;
		}
		var x0=xThis.m_x;
		var x1=xThis.m_LerpTargetX;
		xThis.m_x=x0+(x1-x0)*t;

		var y0=xThis.m_y;
		var y1=xThis.m_LerpTargetX;
		xThis.m_y=y0+(y1-y0)*t;
		
		
		if(1){
			xThis.m_Anim.SetAnimation(0);
			xThis.SetChildrenAnimation(0);
		}else{
			TileManager.WorldOffsetX=-((xThis.m_x)-(canvas.width/2));
			TileManager.WorldOffsetY=-((xThis.m_y)-(canvas.height/2));
			xThis.m_Anim.SetAnimation(1);
			xThis.SetChildrenAnimation(1);
		}
		
		if(xThis.m_EntityID==EntityMngr.m_PlayerID){
			TileManager.WorldOffsetX=-((xThis.m_x)-(canvas.width/2));
			TileManager.WorldOffsetY=-((xThis.m_y)-(canvas.height/2));
		}
		
	}
	this.onMouseDown=function(e){
		var hit=UI.MouseHitTest(e);
		if(!hit){
			var x=e.offsetX-TileManager.WorldOffsetX;
			var y=e.offsetY-TileManager.WorldOffsetY;
 			/* l(x);
			l(y);  */
			var end=TileManager.GetTileFromWorldPos(x,y,0);
			l(end.m_x);
			l(end.m_y);
			var start=TileManager.GetTileFromWorldPos(xThis.m_x,xThis.m_y,0);
			if(end!=null&&start!=null){
				var arr=TileManager.SolvePath(start,end);
				//l(arr);
			}
		}
	}
	this.ItemEquip=function(e){
		var item=e.detail.d;
		if(item.m_OwnerID!=xThis.m_EntityID){return;}
		var idx=xThis.m_Children.indexOf(item);
		if(item.m_Equipped){
			if(idx==-1){
				item.m_Parent=xThis;
				item.m_Anim.m_currentFrame=xThis.m_Anim.m_currentFrame;
				item.m_Anim.m_TimeIncrement=xThis.m_Anim.m_TimeIncrement;
				xThis.m_Children.push(item);
			}
		}else{
			if(idx!=-1){
				item.m_Parent=0;
				xThis.m_Children.splice(idx,1);
			}
		}
	}
	document.addEventListener('ItemEquip', xThis.ItemEquip);
		
	this.InventoryItemLoaded=function(e){
		var item=e.detail.d;
		if(item.m_OwnerID!=xThis.m_EntityID){return;}
		xThis.m_Inventory.push(e.detail.d);
	}
	document.addEventListener('InventoryItemLoaded',xThis.InventoryItemLoaded);
	this.Unload=function(){
		document.removeEventListener('ItemEquip',xThis.ItemEquip);
		document.removeEventListener('InventoryItemLoaded',xThis.InventoryItemLoaded);
	}
}
window.Player=Player;

var Torch=function(){
	
	this.m_EntityID=-1;
	this.m_Anim=new Animation();
	this.m_ReadBytes=0;
	this.m_State=1;
	this.m_x=0;
	this.m_y=0;
	this.m_LightSource=true;
	this.m_LightRadius=400;
	
	var xThis=this;
	this.init=function(p_EntityID,p_Data){
		xThis.m_EntityID=p_EntityID;
		xThis.ParseData(p_Data);
		En.post('assets.php',{t:'torchanim.ani'},function(e){
			var json=JSON.parse(e);
			var anim=xThis.m_Anim;
			anim.m_AnimationRate=json.rate;
			anim.SetAnimationData(json.a);
			
			xThis.m_Anim.Load(json.src,function(){
				
			});
			
		});
	}
	this.LightSource=function(){return xThis.m_LightSource;}
	this.Radius=function(){return xThis.m_LightRadius;}
	this.ReadBytes=function(){return xThis.m_ReadBytes;}
	this.Snapshot=function(p_Data){
		xThis.ParseData(p_Data);
	}
	this.ParseData=function(p_Data){
		var idx=0;
		xThis.m_State=p_Data[idx++];
		var i0=((p_Data[idx++]<<8)|(p_Data[idx++]&0xff));
		var f0=((p_Data[idx++]<<8)|(p_Data[idx++]&0xff));
		var s0=(i0&0x8000)?-1:1;
		i0=(i0&0x7FFF);

		var i1=((p_Data[idx++]<<8)|(p_Data[idx++]&0xff));
		var f1=((p_Data[idx++]<<8)|(p_Data[idx++]&0xff));
		var s1=(i1&0x8000)?-1:1;
		i1=(i1&0x7FFF);
		
		var x=parseFloat(i0+'.'+f0)*s0;
		var y=parseFloat(i1+'.'+f1)*s1;
		xThis.m_x=x;
		xThis.m_y=y;
		xThis.m_ReadBytes=idx;
	}
	this.Update=function(p_Delta){
		xThis.m_Anim.Animate(p_Delta);
	}
	this.Render=function(p_ctx){
		var x=xThis.m_x+TileManager.WorldOffsetX;
		var y=xThis.m_y+TileManager.WorldOffsetY;
		xThis.m_Anim.m_x=x;
		xThis.m_Anim.m_y=y;
		xThis.m_Anim.Render();
	}
}
window.Torch=Torch;
var Equipment=function(){
	this.m_ItemID=0;
	this.m_OwnerID=0;
	this.m_Type=0;
	this.m_Types=[];
	this.m_Equipped=false;
	this.m_Mods="";
	this.m_InvetoryIdx=0;
	this.m_Art="";
	this.m_BaseArt=""; //if no custom art is present
	this.m_Def=0;
	this.m_MinDmg=0;
	this.m_MaxDmg=0;
	this.m_ATS=0;
	this.m_CritP=0;
	this.m_Req=0;
	this.m_InvArt=0;
	this.m_Anim=new Animation();
	this.m_Parent=0;
	
	var xThis=this;
	this.init=function(p_EntityID,p_Data){
		xThis.m_EntityID=p_EntityID;
		xThis.ParseData(p_Data);
		var animtarget=(xThis.m_Art.length>0)?xThis.m_Art:xThis.m_BaseArt;
		var invtarget="images/ui_"+animtarget+".png";
		animtarget+=".ani";
		var obj={d:xThis};
		En.post('assets.php',{t:animtarget},function(e){
			var json=JSON.parse(e);
			if(json.result!==undefined){
				l(json.reason);
			}else{
				var anim=xThis.m_Anim;
				anim.m_AnimationRate=json.rate;
				anim.SetAnimationData(json.a);
				anim.Load(json.src,function(){
					xThis.Validate();
				});
			}
		});
		if(xThis.m_OwnerID==EntityMngr.m_PlayerID){
			xThis.m_InvArt=new Image();
			xThis.m_InvArt.onload=function(e){
				document.dispatchEvent(CreateCustomEvent('InventoryItemLoaded',obj));
			}
			xThis.m_InvArt.src=invtarget;
		}
	}
	this.Validate=function(){
		xThis.Equip();
	}
	this.Equip=function(){
		var obj={d:xThis};
		document.dispatchEvent(CreateCustomEvent('ItemEquip',obj));
	}
	this.ParseData=function(p_Data){
		var idx=0;
		xThis.m_ItemID=((p_Data[idx++]<<8)|(p_Data[idx++]&0xff));
		xThis.m_OwnerID=((p_Data[idx++]<<8)|(p_Data[idx++]&0xff));
		xThis.m_Type=((p_Data[idx++]<<8)|(p_Data[idx++]&0xff));
		xThis.m_Equipped=p_Data[idx++];
		var i,iC=p_Data[idx++];
		for(i=0;i<iC;i++){
			xThis.m_Mods+=String.fromCharCode(p_Data[idx++]);
		}
		xThis.m_InventoryIdx=p_Data[idx++];
		xThis.m_Art=p_Data[idx++]; //this will change once i implement custom art
		xThis.m_Def=p_Data[idx++];
		xThis.m_MaxDmg=p_Data[idx++];
		xThis.m_MinDmg=p_Data[idx++];
		xThis.m_ATS=p_Data[idx++];
		xThis.m_CritP=p_Data[idx++];
		xThis.m_Req=p_Data[idx++];
		iC=p_Data[idx++];
		for(i=0;i<iC;i++){
			xThis.m_BaseArt+=String.fromCharCode(p_Data[idx++]);
		}
		xThis.m_ReadBytes=idx;
	}
	this.ReadBytes=function(){return xThis.m_ReadBytes;}
	this.Update=function(p_Delta){
		if(xThis.m_Parent==0){return;}
		xThis.m_Anim.Animate(p_Delta);
	}
	this.Render=function(p_ctx){
		if(xThis.m_Parent==0){return;}
		var x=xThis.m_Parent.m_Anim.m_x;
		var y=xThis.m_Parent.m_Anim.m_y;
		xThis.m_Anim.m_x=x;
		xThis.m_Anim.m_y=y;
		xThis.m_Anim.Render();
	}
}
window.Equipment=Equipment;