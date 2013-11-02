#ifndef _TIZENCHAT_FRAME_H_
#define _TIZENCHAT_FRAME_H_

#include <FApp.h>
#include <FBase.h>
#include <FSystem.h>
#include <FUi.h>
#include <FUiIme.h>
#include <FGraphics.h>
#include <gl.h>

class TizenChatFrame
	: public Tizen::Ui::Controls::Frame
{
public:
	TizenChatFrame(void);
	virtual ~TizenChatFrame(void);

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
};

#endif	//_TIZENCHAT_FRAME_H_
