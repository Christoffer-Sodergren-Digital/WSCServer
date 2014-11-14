#ifndef APPLICATIONDATAFRAME_H
#define APPLICATIONDATAFRAME_H


namespace WS{

enum ApplicationFrameOpCode{
    APP_MSG=0x01,
	APP_LOGIN=0x02,
	APP_SNAPSHOT=0x03,
	APP_INPUTBUFFER=0x04,
	APP_PLAYER_DISCONNECT=0x05,
	APP_PLAYER_FETCH_INVENTORY=0x06,
	APP_PLAYER_EQUIP_ITEM=0x07,
	APP_FULL_SNAPSHOT=0x08
};

class WSApplicationDataFrame
{
    public:
        WSApplicationDataFrame();
        WSApplicationDataFrame(unsigned char *p_pcData, int p_iLen, ApplicationFrameOpCode p_eOpCode);
        virtual ~WSApplicationDataFrame();

        virtual void MakeFrame(unsigned char *p_pcData, int p_iLen, ApplicationFrameOpCode p_eOpCode);
        void Finalize();

        virtual void ParseFrame(unsigned char *p_pcData, int p_iLen);

        unsigned char OpCode()const{return m_ucOpCode;};

        unsigned int FrameSize()const{return m_uiFrameSize;}
        unsigned int DataSize()const{return m_uiDataSize;}

        unsigned char *Frame();

    protected:
         void CleanFrame();

         bool m_bValidFrame;

        unsigned char m_ucOpCode;

        unsigned int m_uiDataOffset;
        unsigned int m_uiFrameSize;
        unsigned int m_uiDataSize;

        unsigned char *m_pucData;
        unsigned char *m_pucFrame;
    private:
};

}

#endif // APPLICATIONDATAFRAME_H
