#ifndef APPLICATIONDATAFRAME_H
#define APPLICATIONDATAFRAME_H


namespace WS{

enum ApplicationFrameOpCode{
    APP_MSG=0x01,
	APP_LOGIN=0x02
};

class WSApplicationDataFrame
{
    public:
        WSApplicationDataFrame();
        WSApplicationDataFrame(unsigned char *p_pcData, ApplicationFrameOpCode p_eOpCode);
        virtual ~WSApplicationDataFrame();

        virtual void MakeFrame(unsigned char *p_pcData, ApplicationFrameOpCode p_eOpCode);
        void Finalize();

        virtual void ParseFrame(unsigned char *p_pcData);

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