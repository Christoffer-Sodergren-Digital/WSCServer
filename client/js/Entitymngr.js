var EntityMngr=function(){
	this.m_Entities=[];
	this.m_bFullSnapReceived=false;
	this.m_SnapShotHead=0;
	this.m_MaxSnapShots=20;
	this.m_xSnapshotFrames=[];
<<<<<<< HEAD
//	this.m_xSnapshotFrames.length=this.m_MaxSnapShots;
	this.m_CurrentSnapshot=0;
	this.m_PlayerID=-1;
	this.m_ClientTime=0;
	var xThis=this;
	
	document.addEventListener('NetLoggedIn',function(e){
		xThis.m_PlayerID=e.detail.playerid;
	});
	
	this.m_EntityConversionTable=["Player","Torch","Equipment"];
=======
>>>>>>> parent of fa8c75e... Entities, collisions and movement
	
	this.m_EntityConversionTable=["Player"];
	
	var xThis=this;
	this.NewEntity=function(p_Type, p_ID){
		if(xThis.m_EntityConversionTable[p_Type-1]===undefined){return false;}
		var type=xThis.m_EntityConversionTable[p_Type-1];
		if(window[type]===undefined){return false;}
		var xEnt=new window[type]();
		xThis.m_Entities[p_ID]=xEnt;
		
		return xEnt;
	}
	this.EntityExists=function(p_ID){
		return (xThis.m_Entities.indexOf(p_ID)!=-1);
	}
	this.GetEntities=function(){
		return xThis.m_Entities;
	}
<<<<<<< HEAD
	this.GetEntitiesWithProperty=function(p_xProp,p_ret){
		for(var obj in xThis.m_Entities){
			var o=xThis.m_Entities[obj];
			if(o[p_xProp]!==undefined){
				if(p_ret.indexOf(o)==-1){
					p_ret.push(o);
				}
			}
		}
	}
	this.Update=function(p_Delta){
		xThis.m_ClientTime=Date.now();
		var renderTime=xThis.m_ClientTime-p_Delta-75;
		var xSF=xThis.m_xSnapshotFrames;
		if(renderTime<0||xSF.length<2){return;}
		
		
		var srt=-1,srn=-1;
		var current=0;
		var i,iC=xThis.m_MaxSnapShots;
		var min=10000;
		
		for(i=0;i<iC;i++){
			var idx0=((i+xThis.m_SnapShotHead)%xSF.length);
			var t0=xSF[idx0].detail.idx;
			var sample=renderTime-t0;
			if(sample>=0&&sample<min){
				min=sample;
				srt=idx0;
				srn=(((i+1)+xThis.m_SnapShotHead)%xSF.length);
				current=t0;
			}
		}
		if(srt==-1||srn==-1){return;}
		
		if(xThis.m_CurrentSnapshot!=current){ //if we havent already, parse current snapshot
			xThis.m_CurrentSnapshot=current;
			var d=xSF[srt].detail.d;
			//nogcRemove(xSF,0);
			var offset=0;
			while(offset<d.length){
				var entitytype=d[offset++];
				var entityid=((d[offset++]<<4)|d[offset++]);
				
				var xEnt=0;
				if(xThis.m_Entities[entityid]==undefined){
					xEnt=xThis.NewEntity(entitytype,entityid);
					if(xEnt.init!==undefined){
						xEnt.init(entityid,d.subarray(offset));
					}
				}else{
					xEnt=xThis.m_Entities[entityid];
					if(xEnt.Snapshot!==undefined){
						xEnt.Snapshot(d.subarray(offset));
					}
				}
				if(xEnt&&xEnt.ReadBytes!==undefined){
					var read=xEnt.ReadBytes();
					offset+=read;
				}
			}
			d=xSF[srn].detail.d;
			offset=0;
		}
		
		//var xEnts=xThis.m_Entities;
		var i,iC=xThis.m_Entities.length;
		for (var obj in xThis.m_Entities){
			if(xThis.m_Entities[obj]&&xThis.m_Entities[obj].Update===undefined){continue;}
			xThis.m_Entities[obj].Update(p_Delta);
=======
	this.Update=function(p_Delta){
		var xEnts=xThis.m_Entities;
		var i,iC=xEnts.length;
		for (var obj in xEnts){
			var o=xEnts[obj];
			if(o&&o.Update===undefined){continue;}
			o.Update(p_Delta);
>>>>>>> parent of fa8c75e... Entities, collisions and movement
		}
	}
	document.addEventListener('NetFullSnapshot', function(p_Data){
			var d=p_Data.detail.d;
			//nogcRemove(xSF,0);
			var offset=0;
			while(offset<d.length){
				var entitytype=d[offset++];
				var entityid=((d[offset++]<<4)|d[offset++]);
				
				var xEnt=0;
				if(xThis.m_Entities[entityid]==undefined){
					xEnt=xThis.NewEntity(entitytype,entityid);
					if(xEnt.init!==undefined){
						xEnt.init(entityid,d.subarray(offset));
					}
				}else{
					xEnt=xThis.m_Entities[entityid];
					if(xEnt.Snapshot!==undefined){
						xEnt.Snapshot(d.subarray(offset));
					}
				}
				if(xEnt&&xEnt.ReadBytes!==undefined){
					var read=xEnt.ReadBytes();
					offset+=read;
				}
			}
			offset=0;
	});
	document.addEventListener('NetSnapshot', function(p_Data){
		var xSF=xThis.m_xSnapshotFrames;
		xSF[xThis.m_SnapShotHead%xThis.m_MaxSnapShots]=p_Data;
		xThis.m_SnapShotHead++;
		if(xThis.m_SnapShotHead>xThis.m_MaxSnapShots){xThis.m_SnapShotHead=0;}

		xThis.m_bFullSnapReceived=true;//first snapshot should always be a full one
	});
<<<<<<< HEAD
	document.addEventListener('NetInventory', function(p_Data){
		var xSF=xThis.m_xSnapshotFrames;
		var d=p_Data.detail.d;
		var offset=0;
		while(offset<d.length){
			var entitytype=d[offset++];
			var entityid=((d[offset++]<<4)|d[offset++]);
			
			var xEnt=0;
			if(xThis.m_Entities[entityid]==undefined){
				xEnt=xThis.NewEntity(entitytype,entityid);
				if(xEnt.init!==undefined){
					xEnt.init(entityid,d.subarray(offset));
				}
			}else{
				xEnt=xThis.m_Entities[entityid];
				if(xEnt.Snapshot!==undefined){
					xEnt.Snapshot(d.subarray(offset));
				}
			}
			if(xEnt&&xEnt.ReadBytes!==undefined){
				var read=xEnt.ReadBytes();
				offset+=read;
			}
		}
		/* d=xSF[srn].detail.d;
		offset=0; */
	});
	this.PlayerDisconnect=function(p_Data){
		var d=p_Data.detail.d;
		var PlayerID=((d[0]<<8)|(d[1]&0xff));
		if(xThis.m_Entities[PlayerID]!==undefined){
			/* if(xThis.m_Entities[PlayerID].Unload){
				xThis.m_Entities[PlayerID].Unload();
			} */
			var xEnt=xThis.m_Entities[PlayerID];
			if(xEnt.m_Inventory!==undefined){
				var i,iC=xEnt.m_Inventory.length;
				for(i=0;i<iC;i++){
					var idx=xThis.m_Entities.indexOf(xEnt.m_Inventory[i]);
					if(idx!=-1){
						xEnt.m_Inventory[i]=0;
						delete xThis.m_Entities[idx];
					}
				}
			}
			if(xEnt.Unload!=undefined){
				xEnt.Unload();
			}
			delete xThis.m_Entities[PlayerID];
		}
	}
	
	document.addEventListener('NetPlayerDisconnect',xThis.PlayerDisconnect);
=======
>>>>>>> parent of fa8c75e... Entities, collisions and movement
}
window.EntityMngr=new EntityMngr();

