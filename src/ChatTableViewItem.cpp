/*
 * ChatTableViewItem.cpp
 *
 *  Created on: Nov 13, 2013
 *      Author: egv
 */

#include "ChatTableViewItem.h"

#include <FApp.h>
#include <FUi.h>
#include <FGraphics.h>

#include "Utils.h"
#include "Message.h"

using namespace Tizen::App;
using namespace Tizen::Graphics;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;

ChatTableViewItem::~ChatTableViewItem(void)
{
	if (__pBitmap != null)
	{
		delete __pBitmap;
	}
}

result
ChatTableViewItem::Construct (const Tizen::Graphics::Dimension &itemSize)
{
	__pBitmap = null;

	RelativeLayout *pRelativeLayout = new RelativeLayout;
	pRelativeLayout->Construct();

	result r = TableViewItem::Construct(*pRelativeLayout, itemSize);
	delete pRelativeLayout;

	pRelativeLayout = static_cast<RelativeLayout*>(GetLayoutN());

	_pTextLabel = new Label;
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

void
ChatTableViewItem::SetUserAvatar(Tizen::Graphics::Bitmap* pBitmap)
{
	if (pBitmap == null)
	{
		AppLogDebug("trying to set empty avatar");
		return;
	}

	Bitmap* pMaskBitmap = Utils::getInstance().GetBitmapWithName(String(L"thumbnail_list.png"));
	if (pMaskBitmap)
	{
		Canvas *pCanvas = new Canvas;
		pCanvas->Construct(Rectangle(0, 0, 108, 108));
		pCanvas->DrawBitmap(Rectangle(0, 0, 108, 108), *pBitmap, Rectangle(0, 0, pBitmap->GetWidth(), pBitmap->GetHeight()));
		pCanvas->DrawBitmap(Rectangle(0, 0, 108, 108), *pMaskBitmap, Rectangle(0, 0, pMaskBitmap->GetWidth(), pMaskBitmap->GetHeight()));

		if (__pBitmap != null)
		{
			delete __pBitmap;
		}

		__pBitmap = new Bitmap();
		__pBitmap->Construct(*pCanvas, Rectangle(0, 0, 108, 108));

		delete pCanvas;
	}

	delete pBitmap;
	delete pMaskBitmap;
}

void
ChatTableViewItem::FillWithMessage(Message *pMessage)
{
	String timeStr;
	Utils::getInstance().GetTimeFromTimestamp(pMessage->date, timeStr);
	_pTimeLabel->SetText(timeStr);
    _pTextLabel->SetText(pMessage->body);
}

result
ChatTableViewItem::OnDraw()
{
	if (__pBitmap != null)
	{
		Canvas* pCanvas = GetCanvasN();
		pCanvas->DrawBitmap(Rectangle(0, 0, 108, 108), *__pBitmap, Rectangle(0, 0, 108, 108));
	}

	return E_SUCCESS;
}
