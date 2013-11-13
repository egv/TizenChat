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

#include "Message.h"
#include "ImagesManager.h"

using namespace Tizen::App;
using namespace Tizen::Graphics;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;


result
ChatTableViewItem::Construct (const Tizen::Graphics::Dimension &itemSize)
{
	RelativeLayout *pRelativeLayout = new RelativeLayout;
	pRelativeLayout->Construct();

	result r = TableViewItem::Construct(*pRelativeLayout, itemSize);
	delete pRelativeLayout;

	pRelativeLayout = static_cast<RelativeLayout*>(GetLayoutN());

	/*
	 * 	Tizen::Ui::Controls::Label _pAvatarLabel;
	Tizen::Ui::Controls::Label _pTitleLabel;
	Tizen::Ui::Controls::Label _pTextLabel;
	Tizen::Ui::Controls::Label _pTimeLabel;
	Tizen::Ui::Controls::Label _pNameLabel;

	 *
	 */

	_pAvatarLabel = new Label;
	_pAvatarLabel->Construct(Rectangle(0, 0, itemSize.height, itemSize.height), L"");
	AddControl(_pAvatarLabel);
	pRelativeLayout->SetRelation(*_pAvatarLabel, this, RECT_EDGE_RELATION_TOP_TO_TOP);
	pRelativeLayout->SetRelation(*_pAvatarLabel, this, RECT_EDGE_RELATION_LEFT_TO_LEFT);
	pRelativeLayout->SetRelation(*_pAvatarLabel, this, RECT_EDGE_RELATION_BOTTOM_TO_BOTTOM);

	_pTextLabel = new Label;
	_pTextLabel->Construct(Rectangle(0, 0, 0, 45), L"");
	AddControl(_pTextLabel);
	pRelativeLayout->SetRelation(*_pTextLabel, this, RECT_EDGE_RELATION_RIGHT_TO_RIGHT);
	pRelativeLayout->SetRelation(*_pTextLabel, _pAvatarLabel, RECT_EDGE_RELATION_LEFT_TO_RIGHT);
	pRelativeLayout->SetRelation(*_pTextLabel, this, RECT_EDGE_RELATION_BOTTOM_TO_BOTTOM);
    _pTextLabel->SetTextConfig(35, LABEL_TEXT_STYLE_NORMAL);
    _pTextLabel->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
    _pTextLabel->SetTextVerticalAlignment(ALIGNMENT_TOP);


	return r;
}

void
ChatTableViewItem::FillWithMessage(Message *pMessage)
{
	Bitmap *pBitmap = ImagesManager::GetInstance().GetBitmapForUrl(null, 108, 108, null, null);

    AppResource *pAppResource = App::GetInstance()->GetAppResource();
    Bitmap* result = null;

    if (pAppResource)
    {
    	Bitmap* pMaskBitmap = pAppResource->GetBitmapN(L"thumbnail_list.png", BITMAP_PIXEL_FORMAT_ARGB8888);

    	if (pBitmap)
        {
    		Canvas *pCanvas = new Canvas();
    		pCanvas->Construct(Rectangle(0, 0, 108, 108));
    		pCanvas->DrawBitmap(Rectangle(0, 0, 108, 108), *pBitmap, Rectangle(0, 0, pBitmap->GetWidth(), pBitmap->GetHeight()));
    		pCanvas->DrawBitmap(Rectangle(0, 0, 108, 108), *pMaskBitmap, Rectangle(0, 0, pMaskBitmap->GetWidth(), pMaskBitmap->GetHeight()));

    		result = new Bitmap();
    		result->Construct(*pCanvas, Rectangle(0, 0, 108, 108));
        }
    }

    _pAvatarLabel->SetBackgroundBitmap(*result);

    _pTextLabel->SetText(pMessage->body);
}

