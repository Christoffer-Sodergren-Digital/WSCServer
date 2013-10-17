var EntityMngr=function(){
	this.m_Entities=[];
	this.m_bFullSnapReceived=false;
	this.m_xSnapshotFrames=[];
	
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
	this.Update=function(p_Delta){
		var xEnts=xThis.m_Entities;
		var i,iC=xEnts.length;
		for (var obj in xEnts){
			var o=xEnts[obj];
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
}
window.EntityMngr=new EntityMngr();

