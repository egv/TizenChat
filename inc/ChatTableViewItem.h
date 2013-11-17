/*
 * ChatTableViewItem.h
 *
 *  Created on: Nov 13, 2013
 *      Author: egv
 */

#ifndef CHATTABLEVIEWITEM_H_
#define CHATTABLEVIEWITEM_H_

#include <FUi.h>
#include <FGraphics.h>

#include "Message.h"

class ChatTableViewItem: public Tizen::Ui::Controls::TableViewItem
{
public:
	~ChatTableViewItem(void);

	result Construct (const Tizen::Graphics::Dimension &itemSize);

	void SetUserAvatar(Tizen::Graphics::Bitmap* pBitmap);
	void SetUserName(Tizen::Base::String name);
	void FillWithMessage(Message* pMessage);

	result OnDraw();


protected:
	Tizen::Ui::Controls::Label* _pAvatarLabel;
	Tizen::Ui::Controls::Label* _pTitleLabel;
	Tizen::Ui::Controls::Label* _pTextLabel;
	Tizen::Ui::Controls::Label* _pTimeLabel;
	Tizen::Ui::Controls::Label* _pNameLabel;

	Tizen::Graphics::Bitmap* __pBitmap;
};

#endif /* CHATTABLEVIEWITEM_H_ */
