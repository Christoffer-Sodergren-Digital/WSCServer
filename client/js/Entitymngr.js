var EntityMngr=function(){
	this.m_Entities=[];
	this.m_bFullSnapReceived=false;
	this.m_xSnapshotFrames=[];
	this.m_PlayerID=0;
	var xThis=this;
	
	document.addEventListener('NetLoggedIn',function(e){
		xThis.m_PlayerID=e.detail.playerid;
	});
	
	this.m_EntityConversionTable=["Player","Torch"];
	
	
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
	this.GetEntitiesWithProperty=function(p_xProp,p_ret){
		for(var obj in xThis.m_Entities){
			var o=xThis.m_Entities[obj];
			if(o[p_xProp]!==undefined){
				p_ret.push(o);
			}
		}
	}
	this.Update=function(p_Delta){
		//var xEnts=xThis.m_Entities;
		var i,iC=xThis.m_Entities.length;
		for (var obj in xThis.m_Entities){
			var o=xThis.m_Entities[obj];
			if(o&&o.Update===undefined){continue;}
			o.Update(p_Delta);
		}
	}
	document.addEventListener('NetSnapshot', function(p_Data){
		var xSF=xThis.m_xSnapshotFrames;
		xSF.push(p_Data);
		//if(xSF.length>=3||!xThis.m_bFullSnapReceived){
			var d=xSF[0].detail.d;
			xSF.splice(0,1);
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
	//	}
		xThis.m_bFullSnapReceived=true;//first snapshot should always be a full one
	});
	
	this.PlayerDisconnect=function(p_Data){
		var d=p_Data.detail.d;
		var PlayerID=((d[0]<<8)|(d[1]&0xff));
		if(xThis.m_Entities[PlayerID]!==undefined){delete xThis.m_Entities[PlayerID];}
	}
	
	document.addEventListener('NetPlayerDisconnect',xThis.PlayerDisconnect);
}
window.EntityMngr=new EntityMngr();

