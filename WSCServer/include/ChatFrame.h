#ifndef CHATFRAME_H
#define CHATFRAME_H

#include <string>
#include <ApplicationDataFrame.h>

using namespace WS;

class ChatFrame : public WSApplicationDataFrame
{
    public:
        ChatFrame();
        virtual ~ChatFrame();

        virtual void ParseFrame(unsigned char *p_pcData);
		virtual void MakeFrame(unsigned char *p_pcData,int p_iSize, ApplicationFrameOpCode p_eOpCode);

		void Whisper(bool p_bWhisper){m_bWhisper=p_bWhisper;}
		bool Whisper(){return m_bWhisper;}
		std::string & Message(){return m_sMessage;}

    protected:
    private:

	bool m_bWhisper;
	std::string m_sWhisperTarget;
	std::string m_sMessage;


};

#endif // CHATFRAME_H
