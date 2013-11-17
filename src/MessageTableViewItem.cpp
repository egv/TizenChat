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

#define AVATAR_SIZE 74

MessageTableViewItem::~MessageTableViewItem()
{
	DELETE_NON_NULL(__pBitmap);
	if (__pEnrichedText != null)
	{
		__pEnrichedText->RemoveAll(true);
		delete __pEnrichedText;
	}
}

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
		__isMultichat = pMessage->chatId.ToInt() > 0;
	}
}

void
MessageTableViewItem::SetBitmap(Tizen::Graphics::Bitmap* pBitmap)
{
	DELETE_NON_NULL(__pBitmap);

	if (pBitmap == null)
	{
		AppLogDebug("trying to set empty avatar");
		return;
	}

	Bitmap* pMaskBitmap = Utils::getInstance().GetBitmapWithName(String(L"thumbnail_multi.png"));
	if (pMaskBitmap)
	{
		Canvas *pCanvas = new Canvas;
		pCanvas->Construct(Rectangle(0, 0, AVATAR_SIZE, AVATAR_SIZE));
		pCanvas->DrawBitmap(Rectangle(0, 0, AVATAR_SIZE, AVATAR_SIZE), *pBitmap, Rectangle(0, 0, pBitmap->GetWidth(), pBitmap->GetHeight()));
		pCanvas->DrawBitmap(Rectangle(0, 0, AVATAR_SIZE, AVATAR_SIZE), *pMaskBitmap, Rectangle(0, 0, pMaskBitmap->GetWidth(), pMaskBitmap->GetHeight()));

		if (__pBitmap != null)
		{
			delete __pBitmap;
		}

		__pBitmap = new Bitmap();
		__pBitmap->Construct(*pCanvas, Rectangle(0, 0, AVATAR_SIZE, AVATAR_SIZE));

		delete pCanvas;
	}

	delete pBitmap;
	delete pMaskBitmap;
}

int
MessageTableViewItem::HeightForMessage(Message* pMessage, int itemWidth)
{
	EnrichedText* pEnrichedText = MessageTableViewItem::GetEnrichedTextForMessage(pMessage, itemWidth);

	int result = pEnrichedText->GetSize().height + 10;

	pEnrichedText->RemoveAll(true);
	delete pEnrichedText;

	return result < AVATAR_SIZE + 10 ? AVATAR_SIZE + 10 : result;
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

	if (pCanvas == null)
	{
		return E_FAILURE;
	}

	int startX = __bIsOut ? GetSize().width - __pEnrichedText->GetSize().width : (__isMultichat ? AVATAR_SIZE + 10 : 5);
	pCanvas->DrawText(Point(startX, 5), *__pEnrichedText);

	if (__pBitmap != null && !__bIsOut)
	{
		pCanvas->DrawBitmap(Rectangle(0, 0, AVATAR_SIZE, AVATAR_SIZE), *__pBitmap, Rectangle(0, 0, __pBitmap->GetWidth(), __pBitmap->GetHeight()));
	}

	return E_SUCCESS;
}

Tizen::Graphics::EnrichedText*
MessageTableViewItem::GetEnrichedTextForMessage(Message* pMessage, int itemWidth)
{
	EnrichedText* pEnrichedText = new (std::nothrow) EnrichedText;

	int usedWidth = 5;

	if (pMessage->isOut.ToInt() == 0)
	{
		if (pMessage->chatId.ToInt() > 0)
		{
			usedWidth += AVATAR_SIZE + 5;
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
