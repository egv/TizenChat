/*
 * MessageTableViewItem.cpp
 *
 *  Created on: Nov 17, 2013
 *      Author: egv
 */

#include "MessageTableViewItem.h"

#include <FBase.h>
#include <FGraphics.h>
#include <FUi.h>

#include "Utils.h"

using namespace Tizen::Base;
using namespace Tizen::Base::Collection;
using namespace Tizen::Graphics;
using namespace Tizen::Ui::Controls;

MessageTableViewItem::~MessageTableViewItem()
{
	DELETE_NON_NULL(__pBitmap);
	if (__pEnrichedText != null)
	{
		__pEnrichedText->RemoveAll(true);
		delete __pEnrichedText;
	}
}

/*
result
MessageTableViewItem::Construct(const Dimension& itemSize)
{
	RelativeLayout *pRelativeLayout = new RelativeLayout;
	pRelativeLayout->Construct();

	result r = TableViewItem::Construct(*pRelativeLayout, itemSize);
	delete pRelativeLayout;

	pRelativeLayout = static_cast<RelativeLayout*>(GetLayoutN());

	__pTextBox = new TextBox;
	_pTextLabel->Construct(Rectangle(0, 0, 0, 45), L"");
	AddControl(_pTextLabel);
	pRelativeLayout->SetRelation(*_pTextLabel, this, RECT_EDGE_RELATION_RIGHT_TO_RIGHT);
	pRelativeLayout->SetRelation(*_pTextLabel, this, RECT_EDGE_RELATION_LEFT_TO_LEFT);
	pRelativeLayout->SetRelation(*_pTextLabel, this, RECT_EDGE_RELATION_BOTTOM_TO_BOTTOM);
	pRelativeLayout->SetMargin(*_pTextLabel, 108, 0, 0, 0);
    _pTextLabel->SetTextConfig(35, LABEL_TEXT_STYLE_NORMAL);
    _pTextLabel->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
    _pTextLabel->SetTextVerticalAlignment(ALIGNMENT_TOP);

	_pTimeLabel = new Label;
	_pTimeLabel->Construct(Rectangle(0, 0, 100, 25), L"");
	AddControl(_pTimeLabel);
	pRelativeLayout->SetRelation(*_pTimeLabel, this, RECT_EDGE_RELATION_RIGHT_TO_RIGHT);
	pRelativeLayout->SetRelation(*_pTimeLabel, this, RECT_EDGE_RELATION_TOP_TO_TOP);
    _pTimeLabel->SetTextConfig(15, LABEL_TEXT_STYLE_NORMAL);
    _pTimeLabel->SetTextHorizontalAlignment(ALIGNMENT_RIGHT);
    _pTimeLabel->SetTextVerticalAlignment(ALIGNMENT_TOP);

	return r;

}
*/

void
MessageTableViewItem::FillWithMessage(Message* pMessage)
{
	if (pMessage != null)
	{
		if (__pEnrichedText != null)
		{
			__pEnrichedText->RemoveAll(true);
			delete __pEnrichedText;
		}

		__pEnrichedText = MessageTableViewItem::GetEnrichedTextForMessage(pMessage, GetSize().width);
		AppLogDebug("h: %d, text: %S", __pEnrichedText->GetTotalLineHeight(), pMessage->body.GetPointer());
		__bIsOut = pMessage->isOut.ToInt() > 0;
	}
}

void
MessageTableViewItem::SetBitmap(Tizen::Graphics::Bitmap* bitmap)
{
	DELETE_NON_NULL(__pBitmap);
	__pBitmap = bitmap;
}

int
MessageTableViewItem::HeightForMessage(Message* pMessage, int itemWidth)
{
	EnrichedText* pEnrichedText = MessageTableViewItem::GetEnrichedTextForMessage(pMessage, itemWidth);

	int result = pEnrichedText->GetSize().height + 10;

	pEnrichedText->RemoveAll(true);
	delete pEnrichedText;

	return result;
}

result
MessageTableViewItem::OnDraw()
{
	if (__pEnrichedText == null)
	{
		return E_SUCCESS;
	}

	AppLogDebug("OnDraw");

	Canvas *pCanvas = GetCanvasN();

	int startX = __bIsOut ? GetSize().width - __pEnrichedText->GetSize().width : 0;
	pCanvas->DrawText(Point(startX, 5), *__pEnrichedText);

	return E_SUCCESS;
}

Tizen::Graphics::EnrichedText*
MessageTableViewItem::GetEnrichedTextForMessage(Message* pMessage, int itemWidth)
{
	EnrichedText* pEnrichedText = new (std::nothrow) EnrichedText;

	int usedWidth = 50;

	if (pMessage->isOut.ToInt() == 0)
	{
		if (pMessage->chatId.ToInt() > 0)
		{
			usedWidth +=108;
		}
	}

	pEnrichedText->Construct(Dimension(itemWidth - usedWidth, 10));
	pEnrichedText->SetVerticalAlignment(TEXT_ALIGNMENT_TOP);
	pEnrichedText->SetHorizontalAlignment(pMessage->isOut.ToInt() == 0 ? TEXT_ALIGNMENT_LEFT : TEXT_ALIGNMENT_RIGHT);
	pEnrichedText->SetTextWrapStyle(TEXT_WRAP_WORD_WRAP);
	pEnrichedText->SetTextAbbreviationEnabled(true);

	TextElement* pTextElement = new TextElement;
	pTextElement->Construct(pMessage->body);
	{
		Font font;
		font.Construct(FONT_STYLE_PLAIN, 35);
		pTextElement->SetFont(font);
	}
	pTextElement->SetTextColor(Color::GetColor(COLOR_ID_WHITE));
	pEnrichedText->Add(*pTextElement);

	pEnrichedText->SetSize(Dimension(itemWidth - usedWidth, pEnrichedText->GetTotalLineHeight()));

	return pEnrichedText;
}
