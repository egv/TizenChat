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
	result Construct (const Tizen::Graphics::Dimension &itemSize);
	void FillWithMessage(Message* pMessage);

protected:
	Tizen::Ui::Controls::Label* _pAvatarLabel;
	Tizen::Ui::Controls::Label* _pTitleLabel;
	Tizen::Ui::Controls::Label* _pTextLabel;
	Tizen::Ui::Controls::Label* _pTimeLabel;
	Tizen::Ui::Controls::Label* _pNameLabel;

};

#endif /* CHATTABLEVIEWITEM_H_ */
