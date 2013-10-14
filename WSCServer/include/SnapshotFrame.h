#ifndef SNAPSHOT_FRAME_H
#define SNAPSHOT_FRAME_H

#include <ostream>
#include "ApplicationDataFrame.h"


class SnapshotFrame : public WS::WSApplicationDataFrame{
public:
	SnapshotFrame();
	virtual ~SnapshotFrame();
	
	void Add(std::ostream & p_xOS);

protected:
private:
	__int64 m_iTimestamp;
};


#endif