
#include "AppResourceId.h"
#include "ChatFormClass.h"

#include <FBase.h>
#include <FUi.h>

#include "Utils.h"
#include "TizenChatDataManager.h"
#include "MessageTableViewItem.h"
#include "DatabaseManager.h"

using namespace Tizen::Base;
using namespace Tizen::Base::Collection;
using namespace Tizen::Ui;
using namespace Tizen::Graphics;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;

ChatFormClass::ChatFormClass(void)
{
}

ChatFormClass::~ChatFormClass(void)
{
	DELETE_NON_NULL(__pHeightsCache);
	DELETE_NON_NULL(__pMessages);
}

bool
ChatFormClass::Initialize()
{
	Form::Construct(ChatForm);

	__pHeightsCache = new (std::nothrow) HashMap();
	__pHeightsCache->Construct();

	return true;
}

result
ChatFormClass::OnInitializing(void)
{
	result r = E_SUCCESS;

	TableView* pTableview1 = static_cast<TableView*>(GetControl(IDC_TABLEVIEW1));
	if(pTableview1)
	{
		pTableview1->SetItemDividerColor(Color::GetColor(COLOR_ID_MAGENTA));
		pTableview1->SetItemProvider(this);
	}

	SetFormBackEventListener(this);

	return r;
}

result
ChatFormClass::OnTerminating(void)
{
	result r = E_SUCCESS;

	return r;
}

void ChatFormClass::OnFormBackRequested(Tizen::Ui::Controls::Form& source)
{
	AppLogDebug("OnFormBackRequested");
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);
	pSceneManager->GoBackward(BackwardSceneTransition(SCENE_TRANSITION_ANIMATION_TYPE_NONE));
}

// Scene event listener
void
ChatFormClass::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
								const Tizen::Ui::Scenes::SceneId& currentSceneId,
								Tizen::Base::Collection::IList* pArgs)
{
	AppLogDebug("OnSceneActivatedN");
	if (pArgs->GetCount())
	{
		Integer* chatId = (Integer*) pArgs->GetAt(0);
		AppLogDebug("need to show chat: %d", chatId->ToInt());
		__chatId = chatId->ToInt();

		TizenChatDataManager::GetInstance().AddDataManagerEventsListener(*this);
		TizenChatDataManager::GetInstance().LoadChatHistory(__chatId);
	}
}

void
ChatFormClass::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
								const Tizen::Ui::Scenes::SceneId& nextSceneId)
{
	TizenChatDataManager::GetInstance().RemoveDataManagerEventsListener(*this);
}

// data manager delegate
void
ChatFormClass::OnDataManagerUpdatedMessages()
{
	if (__pMessages != null)
	{
		__pMessages->RemoveAll(true);
		delete __pMessages;
	}

	__pMessages = DatabaseManager::GetInstance().GetChatMessages(__chatId);

	AppLogDebug("got %d messages in chat %d", __pMessages->GetCount(), __chatId);

	TableView* pTableview1 = static_cast<TableView*>(GetControl(IDC_TABLEVIEW1));
	if(pTableview1)
	{
		pTableview1->UpdateTableView();
	}
}

void
ChatFormClass::OnDataManagerGotError(Tizen::Base::LongLong errorCode, Tizen::Base::String errorText)
{
	if (errorCode.ToInt() == INVALID_TOKEN_ERROR_CODE)
	{
		AppLogDebug("will forward to login screen");
		SceneManager* pSceneManager = SceneManager::GetInstance();
		pSceneManager->GoForward(ForwardSceneTransition(LOGIN_SCENE));
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

//
// Table view stuff here
//

int
ChatFormClass::GetItemCount(void)
{
	return __pMessages == null ? 0 : __pMessages->GetCount();
}

Tizen::Ui::Controls::TableViewItem*
ChatFormClass::CreateItem(int itemIndex, int itemWidth)
{
	Message* pMessage = (Message*)__pMessages->GetAt(itemIndex);

	MessageTableViewItem* pItem = new MessageTableViewItem;
	pItem->Construct(Dimension(itemWidth, MessageTableViewItem::HeightForMessage(pMessage, itemWidth)));
	pItem->FillWithMessage(pMessage);

	return pItem;
}

bool ChatFormClass::DeleteItem(int itemIndex, Tizen::Ui::Controls::TableViewItem* pItem)
{
	delete pItem;

	return true;
}

void ChatFormClass::UpdateItem(int itemIndex, Tizen::Ui::Controls::TableViewItem* _pItem)
{
	Message* pMessage = (Message*)__pMessages->GetAt(itemIndex);
	MessageTableViewItem* pItem = (MessageTableViewItem*)_pItem;
	pItem->FillWithMessage(pMessage);
	pItem->Invalidate(true);
}

int ChatFormClass::GetDefaultItemHeight(void)
{
	return 108;
}
