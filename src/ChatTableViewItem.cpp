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
	_pTextLabel->Construct(Rectangle(0, 0, 0, 35), L"");
	AddControl(_pTextLabel);
	pRelativeLayout->SetRelation(*_pTextLabel, this, RECT_EDGE_RELATION_RIGHT_TO_RIGHT);
	pRelativeLayout->SetRelation(*_pTextLabel, this, RECT_EDGE_RELATION_LEFT_TO_LEFT);
	pRelativeLayout->SetRelation(*_pTextLabel, this, RECT_EDGE_RELATION_BOTTOM_TO_BOTTOM);
	pRelativeLayout->SetMargin(*_pTextLabel, 108, 0, 0, 10);
    _pTextLabel->SetTextConfig(30, LABEL_TEXT_STYLE_NORMAL);
    _pTextLabel->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
    _pTextLabel->SetTextVerticalAlignment(ALIGNMENT_TOP);

	_pTimeLabel = new Label;
	_pTimeLabel->Construct(Rectangle(0, 0, 150, 35), L"");
	AddControl(_pTimeLabel);
	pRelativeLayout->SetRelation(*_pTimeLabel, this, RECT_EDGE_RELATION_RIGHT_TO_RIGHT);
	pRelativeLayout->SetRelation(*_pTimeLabel, this, RECT_EDGE_RELATION_TOP_TO_TOP);
    _pTimeLabel->SetTextConfig(30, LABEL_TEXT_STYLE_BOLD);
	pRelativeLayout->SetMargin(*_pTimeLabel, 0, 0, 10, 0);
    _pTimeLabel->SetTextHorizontalAlignment(ALIGNMENT_RIGHT);
    _pTimeLabel->SetTextVerticalAlignment(ALIGNMENT_TOP);

	_pNameLabel = new Label;
	_pNameLabel->Construct(Rectangle(0, 0, 0, 35), L"");
	AddControl(_pNameLabel);
	pRelativeLayout->SetRelation(*_pNameLabel, _pTimeLabel, RECT_EDGE_RELATION_RIGHT_TO_RIGHT);
	pRelativeLayout->SetRelation(*_pNameLabel, this, RECT_EDGE_RELATION_LEFT_TO_LEFT);
	pRelativeLayout->SetRelation(*_pNameLabel, this, RECT_EDGE_RELATION_TOP_TO_TOP);
	pRelativeLayout->SetMargin(*_pNameLabel, 108, 0, 10, 0);
    _pNameLabel->SetTextConfig(30, LABEL_TEXT_STYLE_BOLD);
    _pNameLabel->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
    _pNameLabel->SetTextVerticalAlignment(ALIGNMENT_TOP);

	return r;
}

void
ChatTableViewItem::SetUserName(Tizen::Base::String name)
{
	_pNameLabel->SetText(name);
}


void
ChatTableViewItem::SetUserAvatar(Tizen::Graphics::Bitmap* pBitmap)
{
	if (pBitmap == null)
	{
		AppLogDebug("trying to set empty avatar");
		return;
	}

	if (__pBitmap != null)
	{
		delete __pBitmap;
	}

	__pBitmap = Utils::getInstance().MaskBitmap(pBitmap, String(L"thumbnail_list.png"), 108, 108);
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
