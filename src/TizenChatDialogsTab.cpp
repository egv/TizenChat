#include "TizenChatDialogsTab.h"
#include "AppResourceId.h"

#include "TizenChatDataManager.h"

#include <FBase.h>

#include "DatabaseManager.h"
#include "Message.h"
#include "ChatTableViewItem.h"

using namespace Tizen::Graphics;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;

using namespace Tizen::Base;
using namespace Tizen::Base::Collection;

TizenChatDialogsTab::TizenChatDialogsTab(void)
{
	__pMessagesList = null;
}

TizenChatDialogsTab::~TizenChatDialogsTab(void)
{
	if (__pMessagesList != null)
	{
		delete __pMessagesList;
	}
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

	LoadChatHistory();

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
TizenChatDialogsTab::OnDataManagerUpdatedLongPollServerData()
{
	LoadChatHistory();
}

void
TizenChatDialogsTab::OnDataManagerUpdatedMessages()
{
	if (__pMessagesList != null)
	{
		delete __pMessagesList;
	}

	__pMessagesList = DatabaseManager::GetInstance().GetLastMessages();

	TableView* pTableview1 = static_cast<TableView*>(GetControl(IDC_TABLEVIEW1));
	if(pTableview1)
	{
		pTableview1->UpdateTableView();
	}
}

void
TizenChatDialogsTab::OnDataManagerUpdatedUser(int userId)
{

}

void
TizenChatDialogsTab::OnDataManagerGotError(Tizen::Base::LongLong errorCode, Tizen::Base::String errorText)
{
    MessageBox messageBox;
    messageBox.Construct(L"Error", errorText, MSGBOX_STYLE_OK, 3000);

    int modalResult = 0;

    // Calls ShowAndWait() : Draws and Shows itself and processes events
    messageBox.ShowAndWait(modalResult);

	if (errorCode.ToInt() == INVALID_TOKEN_ERROR_CODE)
	{
		AppLogDebug("will forward to login screen");
		SceneManager* pSceneManager = SceneManager::GetInstance();
		pSceneManager->GoForward(SceneTransitionId(ID_SCNT_7));
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Table View things
//
//////////////////////////////////////////////////////////////////////////////////////////////////

int
TizenChatDialogsTab::GetItemCount(void)
{
	ArrayList *arr = __pMessagesList;

	return arr == null ? 0 : arr->GetCount();
}

TableViewItem*
TizenChatDialogsTab::CreateItem(int itemIndex, int itemWidth)
{
	ChatTableViewItem* pItem = new ChatTableViewItem;
    pItem->Construct(Dimension(itemWidth, GetDefaultItemHeight()));

    Message *pMessage = (Message *)__pMessagesList->GetAt(itemIndex);

    pItem->FillWithMessage(pMessage);

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
	ChatTableViewItem* pChatItem = static_cast<ChatTableViewItem*>(pItem);
    Message *pMessage = (Message *)__pMessagesList->GetAt(itemIndex);
    pChatItem->FillWithMessage(pMessage);
}

int
TizenChatDialogsTab::GetDefaultItemHeight(void)
{
	// TODO: Add your implementation codes here

	return 108;

}

float
TizenChatDialogsTab::GetDefaultItemHeightF(void)
{
	// TODO: Add your implementation codes here

	return 108.0f;

}

//
// private things
//
void
TizenChatDialogsTab::LoadChatHistory()
{
	if (TizenChatDataManager::GetInstance().GetLongPollServerData() == null)
	{
		TizenChatDataManager::GetInstance().ObtainLongPollServerData();
	}
	else
	{
		TizenChatDataManager::GetInstance().LoadLastMessages();
	}
}

