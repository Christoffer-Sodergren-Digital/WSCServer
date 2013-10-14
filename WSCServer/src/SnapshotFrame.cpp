#include "SnapshotFrame.h"
#include <ctime>

SnapshotFrame::SnapshotFrame() : m_iTimestamp(0){
}

SnapshotFrame::~SnapshotFrame(){}

void SnapshotFrame::Add(std::ostream & p_xOS){
	time_t iTimestamp=time(NULL);

}