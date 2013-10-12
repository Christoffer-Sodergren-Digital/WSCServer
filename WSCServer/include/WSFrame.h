#ifndef WSFRAME_H
#define WSFRAME_H

#define MASK_SIZE 4

namespace WS{
enum WsOpcode{
    OP_CONT=0x0,
    OP_TEXT=0x1,
    OP_BINARY=0x2,
    OP_CLOSE=0x8,
    OP_PING=0x9,
    OP_PONG=0xA
};
class WSFrame
{
    public:
        WSFrame();
        WSFrame(char *p_pcData);
		WSFrame(const unsigned char *p_pcData, WsOpcode p_eOpCode);
        virtual ~WSFrame();

        void ParseFrame(char *p_pcData);
        void MakeFrame(const unsigned char *p_pcApplicationData,WsOpcode p_eOpCode);

        bool IsValid(){return m_bValid;}
        bool IsFin() const {return (bool)m_ucFin;};
        unsigned char OpCode()const{return m_ucOpcode;};
        unsigned int PayloadLen() const {return m_uiPayloadLen;};
        unsigned char *ApplicationData() {return m_pcApplicationData;}
        unsigned char *Data();
        unsigned int FrameSize() const {return m_uiFrameSize;}
    protected:
    private:


        bool m_bValid;

        unsigned char m_ucFin;
        unsigned char m_ucOpcode;
        unsigned char m_ucMask;
        unsigned int m_uiPayloadLen;
        unsigned int m_uiFrameSize;

        unsigned int *m_puiMask;
        unsigned char *m_pcApplicationData;
        unsigned char *m_pcData;

};
};
#endif // WSFRAME_H
