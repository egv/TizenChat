
#include "AppResourceId.h"
#include "ChatFormClass.h"

#include <FBase.h>
#include <FUi.h>

#include "Utils.h"
#include "TizenChatDataManager.h"
#include "MessageTableViewItem.h"
#include "DatabaseManager.h"
#include "ImagesManager.h"

using namespace Tizen::Base;
using namespace Tizen::Base::Collection;
using namespace Tizen::Ui;
using namespace Tizen::Graphics;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;

ChatFormClass::ChatFormClass(void)
{
	__pMessages = null;
	__pHeightsCache = null;
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

	Header *pHeader = GetHeader();
	pHeader->SetStyle(HEADER_STYLE_TITLE);

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
		Message* pMessage = (Message*) pArgs->GetAt(0);
		__chatId = pMessage->chatId.ToInt();
		AppLogDebug("need to show chat: %d", __chatId);

		TizenChatDataManager::GetInstance().AddDataManagerEventsListener(*this);
		TizenChatDataManager::GetInstance().LoadChatHistory(__chatId);

		Header* pHeader = GetHeader();
		if (pHeader != null)
		{
			if (__chatId < 0)
			{
				__userId = pMessage->userId.ToInt();
				// single-user dialog
//				__pAvatarLabel = new Label;
//				__pAvatarLabel->Construct(Rectangle(0, 0, 88, 88), L"");
//				pHeader->Add

				User* pUser = DatabaseManager::GetInstance().GetUserById(LongLong(__userId));
				pHeader->SetBackgroundBitmap(GetDialogHeaderBackgroundBitmap(pUser));
//				__pAvatarLabel->SetBackgroundBitmap(*(Utils::getInstance().MaskBitmap(GetAvatarBitmap(pUser, -1), String(L"thumbnail_header.png"), 88, 88)));
//				pHeader->SetTitleIcon(Utils::getInstance().MaskBitmap(GetAvatarBitmap(pUser, -1), String(L"thumbnail_header.png"), 88, 88));
			}
			else
			{
				// multi-user chat
				pHeader->SetBackgroundBitmap(GetMultichatHeaderBackgroundBitmap());
			}
		}
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

	if (pMessage->chatId.ToInt() > 0)
	{
	    User *pUser = DatabaseManager::GetInstance().GetUserById(pMessage->userId);
		pItem->SetBitmap(GetAvatarBitmap(pUser, itemIndex));
	}

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
	if (pMessage->chatId.ToInt() > 0)
	{
	    User *pUser = DatabaseManager::GetInstance().GetUserById(pMessage->userId);
		pItem->SetBitmap(GetAvatarBitmap(pUser, itemIndex));
	}

	pItem->Invalidate(true);
}

int
ChatFormClass::GetDefaultItemHeight(void)
{
	return 108;
}

//
// Image manager stuff
//
void
ChatFormClass::OnImageManagerDownloadedImage(Tizen::Graphics::Bitmap* pBitmap, Tizen::Base::Collection::HashMap* userInfo)
{
	if (userInfo == null)
	{
		return;
	}

//	Integer* listenerTag = null;
	Integer* rowNumber = null;

	rowNumber = static_cast<Integer*>(userInfo->GetValue(String(L"rowNumber")));

	if (rowNumber->ToInt() == -1)
	{
	    User *pUser = DatabaseManager::GetInstance().GetUserById(__userId);
		GetHeader()->SetBackgroundBitmap(GetDialogHeaderBackgroundBitmap(pUser));
//		__pAvatarLabel->SetBackgroundBitmap(*(Utils::getInstance().MaskBitmap(pBitmap, String(L"thumbnail_header.png"), 88, 88)));
//		__pAvatarLabel->Invalidate(true);
	}
	else
	{
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
}

void
ChatFormClass::OnImageManagerDownloadFailed(Tizen::Base::Collection::HashMap* userInfo)
{
	// doing nothing now
	AppLogDebug("got failure for loading image");
}


Tizen::Graphics::Bitmap*
ChatFormClass::GetAvatarBitmap(User* pUser, int itemIndex)
{
	Bitmap* result = null;

    if (pUser != null)
    {
        HashMap* pHashMap = new HashMap;
        pHashMap->Construct();
        pHashMap->Add(new String(L"rowNumber"), new Integer(itemIndex));
        AppLogDebug("will avatar info for user %d from url %S", pUser->id.ToInt(), pUser->photoMediumRec.GetPointer());
        result = ImagesManager::GetInstance().GetBitmapForUrl(pUser->photoMediumRec, this, pHashMap);
    }
    else
    {
        result = ImagesManager::GetInstance().GetUnknownAvatar();
    }

    return result;
}

Tizen::Graphics::Bitmap*
ChatFormClass::GetDialogHeaderBackgroundBitmap(User* pUser)
{
	if (pUser == null)
	{
		return null;
	}

	Bitmap* result = null;
	Canvas* pCanvas = new Canvas;
	Rectangle rect = Rectangle(0, 0, GetHeader()->GetSize().width, GetHeader()->GetSize().height);
	pCanvas->Construct(rect);
	pCanvas->SetBackgroundColor(Color(50, 77, 117));
	pCanvas->FillRectangle(Color::GetColor(COLOR_ID_RED), rect);
	pCanvas->DrawBitmap(Rectangle(0, 0, 88, 88), *(Utils::getInstance().MaskBitmap(GetAvatarBitmap(pUser, -1), String(L"thumbnail_header.png"), 88, 88)));

	result = new Bitmap;
	result->Construct(*pCanvas, rect);

	delete pCanvas;

	return result;
}

Tizen::Graphics::Bitmap*
ChatFormClass::GetMultichatHeaderBackgroundBitmap()
{
	Bitmap* result = null;
	Canvas* pCanvas = new Canvas;
	Rectangle rect = Rectangle(0, 0, GetHeader()->GetSize().width, GetHeader()->GetSize().height);
	pCanvas->Construct(rect);
	pCanvas->SetBackgroundColor(Color(50, 77, 117, 255));
	Bitmap* img = Utils::getInstance().GetBitmapWithName(String(L"no_photo_group.png"));
	pCanvas->DrawBitmap(Rectangle(0, 0, 88, 88), *img, Rectangle(0, 0, img->GetWidth(), img->GetHeight()));

	result = new Bitmap;
	result->Construct(*pCanvas, rect);

	delete pCanvas;

	return result;
}
