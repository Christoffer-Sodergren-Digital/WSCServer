#ifndef FRAME_MANAGER_H
#define FRAME_MANAGER_H

#include "ApplicationDataFrame.h"
#include "ChatFrame.h"


class ApplicationFrameManager{
public:
	static WSApplicationDataFrame *ConstructFrame(unsigned char p_ucOpCode){
		if(p_ucOpCode==WS::APP_MSG){
			return new ChatFrame();
		}
	}
protected:

private:

	ApplicationFrameManager();
	~ApplicationFrameManager();
};


#endif