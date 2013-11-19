/*
 * MessageTableViewItem.h
 *
 *  Created on: Nov 17, 2013
 *      Author: egv
 */

#ifndef MESSAGETABLEVIEWITEM_H_
#define MESSAGETABLEVIEWITEM_H_

#include "Message.h"
#include <FUi.h>
#include <FGraphics.h>

class MessageTableViewItem: public Tizen::Ui::Controls::TableViewItem
{
public:
	MessageTableViewItem() : __pEnrichedText(null), __pBitmap(null) {};
	virtual ~MessageTableViewItem();

//	result Construct (const Tizen::Graphics::Dimension &itemSize);

	static int HeightForMessage(Message* pMessage, int itemWidth);

	void FillWithMessage(Message* pMessage);
	void SetBitmap(Tizen::Graphics::Bitmap* bitmap);

	result OnDraw();

private:
	long long __date;

	Tizen::Ui::Controls::TextBox* __pTextBox;

	Tizen::Graphics::EnrichedText* __pEnrichedText;
	Tizen::Graphics::Bitmap* __pBitmap;
	bool __bIsOut;
	bool __isMultichat;

	static Tizen::Graphics::EnrichedText* GetEnrichedTextForMessage(Message* pMessage, int itemWidth);
};

#endif /* MESSAGETABLEVIEWITEM_H_ */
