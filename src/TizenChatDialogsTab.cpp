#include "TizenChatDialogsTab.h"
#include "AppResourceId.h"

#include "TizenChatDataManager.h"

#include <FBase.h>

#include "DatabaseManager.h"
#include "Message.h"
#include "ImagesManager.h"
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

	TableView* pTableview1 = static_cast<TableView*>(GetControl(IDC_TABLEVIEW1));
	if(pTableview1)
	{
		pTableview1->AddTableViewItemEventListener(*this);
		pTableview1->SetItemProvider(this);
	}

	return r;
}

result
TizenChatDialogsTab::OnTerminating(void)
{
	result r = E_SUCCESS;
	return r;
}

void
TizenChatDialogsTab::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
								const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs)
{
	AppLog("OnSceneActivatedN");
	TizenChatDataManager::GetInstance().AddDataManagerEventsListener(*this);
	LoadChatHistory();
}

void
TizenChatDialogsTab::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
								const Tizen::Ui::Scenes::SceneId& nextSceneId)
{
	AppLog("OnSceneDeactivated, next: %S", nextSceneId.GetPointer());
	TizenChatDataManager::GetInstance().RemoveDataManagerEventsListener(*this);
	ImagesManager::GetInstance().RemoveListener(this);
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
	if (errorCode.ToInt() == INVALID_TOKEN_ERROR_CODE)
	{
		AppLogDebug("will forward to login screen");
		SceneManager* pSceneManager = SceneManager::GetInstance();
		pSceneManager->GoForward(SceneTransitionId(ID_SCNT_7));
	}
	else
	{
	    MessageBox messageBox;
	    messageBox.Construct(L"Error", errorText, MSGBOX_STYLE_OK, 3000);

	    int modalResult = 0;

	    // Calls ShowAndWait() : Draws and Shows itself and processes events
	    messageBox.ShowAndWait(modalResult);
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
    pItem->SetUserAvatar(GetAvatarBitmap(pMessage->userId, itemIndex));
    pItem->FillWithMessage(pMessage);

    return pItem;
}

TableViewItem*
TizenChatDialogsTab::CreateItemF(int itemIndex, float itemWidth)
{
	return null;
}

bool
TizenChatDialogsTab::DeleteItem(int itemIndex, TableViewItem* pItem)
{
	pItem->Destroy();

	return true;
}

void
TizenChatDialogsTab::UpdateItem(int itemIndex, TableViewItem* pItem)
{
	ChatTableViewItem* pChatItem = static_cast<ChatTableViewItem*>(pItem);
    Message *pMessage = (Message *)__pMessagesList->GetAt(itemIndex);
    pChatItem->SetUserAvatar(GetAvatarBitmap(pMessage->userId, itemIndex));
    pChatItem->FillWithMessage(pMessage);
}

int
TizenChatDialogsTab::GetDefaultItemHeight(void)
{
	return 108;
}

float
TizenChatDialogsTab::GetDefaultItemHeightF(void)
{
	return 108.0f;
}

//
// Table item reaction stuff
//
void TizenChatDialogsTab::OnTableViewItemStateChanged(Tizen::Ui::Controls::TableView& tableView, int itemIndex, Tizen::Ui::Controls::TableViewItem* pItem, Tizen::Ui::Controls::TableViewItemStatus status)
{
	AppLogDebug("item state changed, itemIndex: %d, state: %d", itemIndex, status);
	if (status == TABLE_VIEW_ITEM_STATUS_SELECTED)
	{
		Message *pMessage = (Message *)__pMessagesList->GetAt(itemIndex);
		SceneManager* pSceneManager = SceneManager::GetInstance();
		ArrayList* arrayList = new (std::nothrow) ArrayList();
		arrayList->Construct();
		arrayList->Add(new Integer(pMessage->chatId.ToInt()));

		result r = pSceneManager->GoForward(ForwardSceneTransition(L"CHAT_SCENE"), arrayList);
		if (IsFailed(r))
		{
			AppLogDebug("failed to go forward: %d", r);
		}
	}
}

void TizenChatDialogsTab::OnTableViewContextItemActivationStateChanged(Tizen::Ui::Controls::TableView& tableView, int itemIndex, Tizen::Ui::Controls::TableViewContextItem* pContextItem, bool activated)
{

}

void TizenChatDialogsTab::OnTableViewItemReordered(Tizen::Ui::Controls::TableView& tableView, int itemIndexFrom, int itemIndexTo)
{

}

//
// Image manager stuff
//
void
TizenChatDialogsTab::OnImageManagerDownloadedImage(Tizen::Graphics::Bitmap* pBitmap, Tizen::Base::Collection::HashMap* userInfo)
{
	if (userInfo == null)
	{
		return;
	}

//	Integer* listenerTag = null;
	Integer* rowNumber = null;

	rowNumber = static_cast<Integer*>(userInfo->GetValue(String(L"rowNumber")));

	TableView* pTableview1 = static_cast<TableView*>(GetControl(IDC_TABLEVIEW1));
	if(pTableview1)
	{
		AppLogDebug("got success for loading image for row %d, row count: %d", rowNumber->ToInt(), pTableview1->GetItemCount());
		if (rowNumber->ToInt() < pTableview1->GetItemCount())
		{
			pTableview1->RefreshItem(rowNumber->ToInt(), TABLE_VIEW_REFRESH_TYPE_ITEM_MODIFY);
		}
	}
}

void
TizenChatDialogsTab::OnImageManagerDownloadFailed(Tizen::Base::Collection::HashMap* userInfo)
{
	// doing nothing now
	AppLogDebug("got failure for loading image");
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

Tizen::Graphics::Bitmap*
TizenChatDialogsTab::GetAvatarBitmap(Tizen::Base::LongLong userId, int itemIndex)
{
	Bitmap* result = null;
    User *pUser = DatabaseManager::GetInstance().GetUserById(userId);

    if (pUser != null)
    {
//    	AppLogDebug("found user with id: %d", userId.ToInt());
//    	pUser->Log();
        HashMap* pHashMap = new HashMap;
        pHashMap->Construct();
        pHashMap->Add(new String(L"rowNumber"), new Integer(itemIndex));
        AppLogDebug("will avatar info for user %d from url %S", pUser->id.ToInt(), pUser->photoMediumRec.GetPointer());
        result = ImagesManager::GetInstance().GetBitmapForUrl(pUser->photoMediumRec, this, pHashMap);

        delete pUser;
    }
    else
    {
    	AppLogDebug("can not find user with id: %d", userId.ToInt());
        result = ImagesManager::GetInstance().GetUnknownAvatar();
    }

    return result;
}
