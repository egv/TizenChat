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
//		Dimension extent;
//		int actualLength;
//		__pEnrichedText->GetTextExtent(0, __pEnrichedText->GetTextLength(), extent, actualLength);
//		AppLogDebug("w: %d, h: %d, ew: %d, eh: %d, l: %d, al: %d, text: %S", __pEnrichedText->GetWidth(), __pEnrichedText->GetTotalLineHeight(), extent.width, extent.height, __pEnrichedText->GetTextLength(), actualLength, pMessage->body.GetPointer());
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

	if (__pBitmap != null)
	{
		delete __pBitmap;
	}

	__pBitmap = Utils::getInstance().MaskBitmap(pBitmap, String(L"thumbnail_multi.png"), AVATAR_SIZE, AVATAR_SIZE);
}

int
MessageTableViewItem::HeightForMessage(Message* pMessage, int itemWidth)
{
	EnrichedText* pEnrichedText = MessageTableViewItem::GetEnrichedTextForMessage(pMessage, itemWidth);

	int result = pEnrichedText->GetSize().height + 40;

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

	if (__bIsOut)
	{
		Bitmap* bubble = Utils::getInstance().GetBitmapWithName(L"my_bubble.#.png");
		pCanvas->DrawNinePatchedBitmap(Rectangle(GetSize().width - __pEnrichedText->GetSize().width - 5 - 61, 5, __pEnrichedText->GetSize().width + 61, __pEnrichedText->GetSize().height + 30), *bubble);
		pCanvas->DrawText(Point(GetSize().width - __pEnrichedText->GetSize().width - 5 - 45, 20), *__pEnrichedText);
	}
	else
	{
		Bitmap* bubble = Utils::getInstance().GetBitmapWithName(L"their_bubble.#.png");

		int startX = 5 + ((__pBitmap != null && !__bIsOut) ? AVATAR_SIZE : 0);
		pCanvas->DrawNinePatchedBitmap(Rectangle(startX, 5, __pEnrichedText->GetSize().width + 61, __pEnrichedText->GetSize().height + 30), *bubble);
		pCanvas->DrawText(Point(startX + 35, 20), *__pEnrichedText);

		if (__pBitmap != null && !__bIsOut)
		{
			pCanvas->DrawBitmap(Rectangle(0, 0, AVATAR_SIZE, AVATAR_SIZE), *__pBitmap, Rectangle(0, 0, __pBitmap->GetWidth(), __pBitmap->GetHeight()));
		}
	}

	return E_SUCCESS;
}

Tizen::Graphics::EnrichedText*
MessageTableViewItem::GetEnrichedTextForMessage(Message* pMessage, int itemWidth)
{
	EnrichedText* pEnrichedText = new (std::nothrow) EnrichedText;

	int usedWidth = 66;

	if (pMessage->isOut.ToInt() == 0 && pMessage->chatId.ToInt() > 0)
	{
		usedWidth += AVATAR_SIZE + 5;
	}

	pEnrichedText->Construct(Dimension(itemWidth - usedWidth, 1000));
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

	Dimension extent;
	int actualLength;
	int www = itemWidth - usedWidth;
	pEnrichedText->GetTextExtent(0, pEnrichedText->GetTextLength(), extent, actualLength);

	pEnrichedText->SetSize(Dimension(www < extent.width ? www : extent.width, pEnrichedText->GetTotalLineHeight()));
	pEnrichedText->Refresh();

	return pEnrichedText;
}
