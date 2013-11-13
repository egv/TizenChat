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
	virtual void FillWithMessage(Message* pMessage) = 0;
};

#endif /* CHATTABLEVIEWITEM_H_ */
