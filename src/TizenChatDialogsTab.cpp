#include "TizenChatDialogsTab.h"
#include "AppResourceId.h"

#include "TizenChatDataManager.h"

#include <FBase.h>
#include "Message.h"

using namespace Tizen::Graphics;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;

using namespace Tizen::Base;
using namespace Tizen::Base::Collection;

TizenChatDialogsTab::TizenChatDialogsTab(void)
{

}

TizenChatDialogsTab::~TizenChatDialogsTab(void)
{

}

bool
TizenChatDialogsTab::Initialize(void)
{
	result r = Construct(IDC_PANEL1);
	if (IsFailed(r))
		return false;

	return true;
}

result
TizenChatDialogsTab::OnInitializing(void)
{
	result r = E_SUCCESS;

	// Layout setting
	const Form* pForm = dynamic_cast<Form*>(GetParent());
	RelativeLayout* pRelativeLayout = dynamic_cast<RelativeLayout*>(pForm->GetLandscapeLayoutN());
	pRelativeLayout->SetHorizontalFitPolicy(*this, FIT_POLICY_PARENT);
	pRelativeLayout->SetVerticalFitPolicy(*this, FIT_POLICY_PARENT);
	delete pRelativeLayout;
	pRelativeLayout = dynamic_cast<RelativeLayout*>(pForm->GetPortraitLayoutN());
	pRelativeLayout->SetHorizontalFitPolicy(*this, FIT_POLICY_PARENT);
	pRelativeLayout->SetVerticalFitPolicy(*this, FIT_POLICY_PARENT);
	delete pRelativeLayout;

	TizenChatDataManager::GetInstance().AddDataManagerEventsListener(*this);
	TizenChatDataManager::GetInstance().LoadLastMessages();

	TableView* pTableview1 = static_cast<TableView*>(GetControl(IDC_TABLEVIEW1));  
	if(pTableview1)
	{
		pTableview1->SetItemProvider(this);
	}
	return r;
}

result
TizenChatDialogsTab::OnTerminating(void)
{
	result r = E_SUCCESS;

	TizenChatDataManager::GetInstance().RemoveDataManagerEventsListener(*this);

	return r;
}

void
TizenChatDialogsTab::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
								const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs)
{
	// TODO:
	// Add your scene activate code here
	AppLog("OnSceneActivatedN");
	TableView* pTableview1 = static_cast<TableView*>(GetControl(IDC_TABLEVIEW1));
	if(pTableview1)
	{
		pTableview1->UpdateTableView();
	}

}

void
TizenChatDialogsTab::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
								const Tizen::Ui::Scenes::SceneId& nextSceneId)
{
	// TODO:
	// Add your scene deactivate code here
	AppLog("OnSceneDeactivated");
}

void
TizenChatDialogsTab::OnDataManagerUpdatedMessages()
{

}

void
TizenChatDialogsTab::OnDataManagerUpdatedUser(int userId)
{

}

void
TizenChatDialogsTab::OnDataManagerGotError(Tizen::Base::String errorText)
{

}

int
TizenChatDialogsTab::GetItemCount(void)
{
	ArrayList *arr = TizenChatDataManager::GetInstance().GetLastMessages();

	return arr == null ? 0 : arr->GetCount();
}

TableViewItem*
TizenChatDialogsTab::CreateItem(int itemIndex, int itemWidth)
{
	TableViewItem* pItem = new TableViewItem();
    pItem->Construct(Dimension(itemWidth, GetDefaultItemHeight()), TABLE_VIEW_ANNEX_STYLE_NORMAL);

    Message *pMessage = (Message *)TizenChatDataManager::GetInstance().GetLastMessages()->GetAt(itemIndex);

    Label* pLabel = new Label();
    pLabel->Construct(Rectangle(0, 0, itemWidth, GetDefaultItemHeight()), pMessage->body);

    pItem->AddControl(pLabel);

    return pItem;
}

TableViewItem*
TizenChatDialogsTab::CreateItemF(int itemIndex, float itemWidth)
{
	// TODO: Add your implementation codes here

	return null;
}

bool
TizenChatDialogsTab::DeleteItem(int itemIndex, TableViewItem* pItem)
{
	// TODO: Add your implementation codes here
	pItem->Destroy();

	return true;
}

void
TizenChatDialogsTab::UpdateItem(int itemIndex, TableViewItem* pItem)
{
	// TODO: Add your implementation codes here
}

int
TizenChatDialogsTab::GetDefaultItemHeight(void)
{
	// TODO: Add your implementation codes here

	return 100;

}

float
TizenChatDialogsTab::GetDefaultItemHeightF(void)
{
	// TODO: Add your implementation codes here

	return 100.0f;

}
