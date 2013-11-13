/*
 * ChatTableViewItem.cpp
 *
 *  Created on: Nov 13, 2013
 *      Author: egv
 */

#include "ChatTableViewItem.h"

#include <FUi.h>
#include <FGraphics.h>

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

	return r;
}
