
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
	__pTableView = null;
	RelativeLayout* pRelativeLayout = new RelativeLayout;
	pRelativeLayout->Construct();

	Form::Construct(*pRelativeLayout, FORM_STYLE_NORMAL | FORM_STYLE_INDICATOR);

	delete pRelativeLayout;

	pRelativeLayout = (RelativeLayout*) GetLayoutN();

	{
		RelativeLayout* pHeaderRelativeLayout = new RelativeLayout;
		pHeaderRelativeLayout->Construct();

		__pHeaderPanel = new Panel;
		__pHeaderPanel->Construct(*pHeaderRelativeLayout, Rectangle(0, 0, GetWidth(), 100));
		delete pHeaderRelativeLayout;
		pHeaderRelativeLayout = (RelativeLayout*)__pHeaderPanel->GetLayoutN();

		__pAvatarLabel = new Label;
		__pAvatarLabel->Construct(Rectangle(12, 12, 88, 88), L"");
		__pHeaderPanel->AddControl(__pAvatarLabel);
		pHeaderRelativeLayout->SetMargin(*__pAvatarLabel, 12, 12, 12, 12);
		pHeaderRelativeLayout->SetRelation(*__pAvatarLabel, __pHeaderPanel, RECT_EDGE_RELATION_LEFT_TO_LEFT);
		pHeaderRelativeLayout->SetRelation(*__pAvatarLabel, __pHeaderPanel, RECT_EDGE_RELATION_TOP_TO_TOP);
		pHeaderRelativeLayout->SetRelation(*__pAvatarLabel, __pHeaderPanel, RECT_EDGE_RELATION_BOTTOM_TO_BOTTOM);

		__pNameLabel = new Label;
		__pNameLabel->Construct(Rectangle(0, 0, 0, 40), L"");
		__pHeaderPanel->AddControl(__pNameLabel);
		pHeaderRelativeLayout->SetMargin(*__pNameLabel, 10, 0, 10, 0);
		pHeaderRelativeLayout->SetRelation(*__pNameLabel, __pAvatarLabel, RECT_EDGE_RELATION_LEFT_TO_RIGHT);
		pHeaderRelativeLayout->SetRelation(*__pNameLabel, __pHeaderPanel, RECT_EDGE_RELATION_TOP_TO_TOP);
		pHeaderRelativeLayout->SetRelation(*__pNameLabel, __pHeaderPanel, RECT_EDGE_RELATION_RIGHT_TO_RIGHT);
		__pNameLabel->SetTextConfig(35, LABEL_TEXT_STYLE_BOLD);
		__pNameLabel->SetTextColor(Color::GetColor(COLOR_ID_WHITE));
		__pNameLabel->SetTextHorizontalAlignment(ALIGNMENT_LEFT);

		__pStatusLabel = new Label;
		__pStatusLabel->Construct(Rectangle(0, 0, 0, 40), L"");
		__pHeaderPanel->AddControl(__pStatusLabel);
		pHeaderRelativeLayout->SetMargin(*__pStatusLabel, 10, 0, 0, 10);
		pHeaderRelativeLayout->SetRelation(*__pStatusLabel, __pAvatarLabel, RECT_EDGE_RELATION_LEFT_TO_RIGHT);
		pHeaderRelativeLayout->SetRelation(*__pStatusLabel, __pHeaderPanel, RECT_EDGE_RELATION_BOTTOM_TO_BOTTOM);
		pHeaderRelativeLayout->SetRelation(*__pStatusLabel, __pHeaderPanel, RECT_EDGE_RELATION_RIGHT_TO_RIGHT);
		pHeaderRelativeLayout->SetRelation(*__pStatusLabel, __pNameLabel, RECT_EDGE_RELATION_TOP_TO_BOTTOM);
		__pStatusLabel->SetTextConfig(30, LABEL_TEXT_STYLE_NORMAL);
		__pStatusLabel->SetTextColor(Color::GetColor(COLOR_ID_WHITE));
		__pStatusLabel->SetTextHorizontalAlignment(ALIGNMENT_LEFT);


	}

	AddControl(__pHeaderPanel);
	pRelativeLayout->SetRelation(*__pHeaderPanel, this, RECT_EDGE_RELATION_TOP_TO_TOP);
	pRelativeLayout->SetRelation(*__pHeaderPanel, this, RECT_EDGE_RELATION_LEFT_TO_LEFT);
	pRelativeLayout->SetRelation(*__pHeaderPanel, this, RECT_EDGE_RELATION_RIGHT_TO_RIGHT);
	pRelativeLayout->SetVerticalFitPolicy(*__pHeaderPanel, FIT_POLICY_FIXED);
	pRelativeLayout->SetHorizontalFitPolicy(*__pHeaderPanel, FIT_POLICY_PARENT);

	{
		RelativeLayout* pFooterRelativeLayout = new RelativeLayout;
		pFooterRelativeLayout->Construct();

		__pFooterPanel = new Panel;
		__pFooterPanel->Construct(*pFooterRelativeLayout, Rectangle(0, 0, GetWidth(), 100));
		delete pFooterRelativeLayout;

		pFooterRelativeLayout = (RelativeLayout*)__pFooterPanel->GetLayoutN();

		__pExpEditArea = new ExpandableEditArea;
		__pExpEditArea->Construct(Rectangle(0, 0, 0, 0), EXPANDABLE_EDIT_AREA_STYLE_NORMAL, EXPANDABLE_EDIT_AREA_TITLE_STYLE_NONE, 5);
		__pExpEditArea->SetGuideText(L"Введите сообщение");
		__pExpEditArea->SetGuideTextColor(Color(106, 110, 140));
		__pFooterPanel->AddControl(__pExpEditArea);

		pFooterRelativeLayout->SetRelation(*__pExpEditArea, __pFooterPanel, RECT_EDGE_RELATION_LEFT_TO_LEFT);
		pFooterRelativeLayout->SetRelation(*__pExpEditArea, __pFooterPanel, RECT_EDGE_RELATION_TOP_TO_TOP);
		pFooterRelativeLayout->SetRelation(*__pExpEditArea, __pFooterPanel, RECT_EDGE_RELATION_BOTTOM_TO_BOTTOM);
		pFooterRelativeLayout->SetMargin(*__pExpEditArea, 5, 5, 5, 5);

		Button* pAttachButton = new Button;
		pAttachButton->Construct(Rectangle(0, 0, 73, 73), L"");
		pAttachButton->SetNormalBitmap(Point(5, 5), *(Utils::getInstance().GetBitmapWithName(L"keyboard_icon_attach.png")));
		pAttachButton->SetColor(BUTTON_STATUS_NORMAL, Color(33, 38, 45));
		__pFooterPanel->AddControl(pAttachButton);

		pFooterRelativeLayout->SetRelation(*pAttachButton, __pFooterPanel, RECT_EDGE_RELATION_RIGHT_TO_RIGHT);
		pFooterRelativeLayout->SetRelation(*pAttachButton, __pFooterPanel, RECT_EDGE_RELATION_BOTTOM_TO_BOTTOM);
		pFooterRelativeLayout->SetMargin(*pAttachButton, 5, 5, 5, 5);
		pFooterRelativeLayout->SetRelation(*__pExpEditArea, pAttachButton, RECT_EDGE_RELATION_RIGHT_TO_LEFT);
	}

	AddControl(__pFooterPanel);
	pRelativeLayout->SetRelation(*__pFooterPanel, this, RECT_EDGE_RELATION_BOTTOM_TO_BOTTOM);
	pRelativeLayout->SetRelation(*__pFooterPanel, this, RECT_EDGE_RELATION_LEFT_TO_LEFT);
	pRelativeLayout->SetRelation(*__pFooterPanel, this, RECT_EDGE_RELATION_RIGHT_TO_RIGHT);
	pRelativeLayout->SetVerticalFitPolicy(*__pFooterPanel, FIT_POLICY_FIXED);
	pRelativeLayout->SetHorizontalFitPolicy(*__pFooterPanel, FIT_POLICY_PARENT);

	__pTableView = new TableView;
	__pTableView->Construct(Rectangle(0, 100, GetWidth(), 500), true, TABLE_VIEW_SCROLL_BAR_STYLE_FADE_OUT);
	AddControl(__pTableView);
	pRelativeLayout->SetRelation(*__pTableView, this, RECT_EDGE_RELATION_LEFT_TO_LEFT);
	pRelativeLayout->SetRelation(*__pTableView, this, RECT_EDGE_RELATION_TOP_TO_TOP);
	pRelativeLayout->SetRelation(*__pTableView, this, RECT_EDGE_RELATION_RIGHT_TO_RIGHT);
	pRelativeLayout->SetRelation(*__pTableView, __pFooterPanel, RECT_EDGE_RELATION_BOTTOM_TO_TOP);
	pRelativeLayout->SetRelation(*__pTableView, __pHeaderPanel, RECT_EDGE_RELATION_TOP_TO_BOTTOM);
	pRelativeLayout->SetHorizontalFitPolicy(*__pTableView, FIT_POLICY_PARENT);

	__pHeightsCache = new (std::nothrow) HashMap();
	__pHeightsCache->Construct();

	return true;
}

result
ChatFormClass::OnInitializing(void)
{
	result r = E_SUCCESS;

	if(__pTableView != null)
	{
		__pTableView->SetItemProvider(this);
	}

	SetFormBackEventListener(this);

	__pExpEditArea->AddExpandableEditAreaEventListener(*this);
	__pFooterPanel->SetBackgroundColor(Color(18, 23, 29));
	__pHeaderPanel->SetBackgroundColor(Color(50+15, 77+20, 117+20));

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

			if (__chatId < 0)
			{
				__userId = pMessage->userId.ToInt();
				User* pUser = DatabaseManager::GetInstance().GetUserById(LongLong(__userId));
				DrawDialogHeaderBackgroundBitmap(pUser);
			}
			else
			{
				// multi-user chat
//				pHeader->SetBackgroundBitmap(GetMultichatHeaderBackgroundBitmap());
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

	if(__pTableView != null)
	{
		__pTableView->UpdateTableView();
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
	    DrawDialogHeaderBackgroundBitmap(pUser);
	}
	else
	{
		if(__pTableView)
		{
			AppLogDebug("got success for loading image for row %d, row count: %d", rowNumber->ToInt(), __pTableView->GetItemCount());
			if (rowNumber->ToInt() < __pTableView->GetItemCount())
			{
				__pTableView->RefreshItem(rowNumber->ToInt(), TABLE_VIEW_REFRESH_TYPE_ITEM_MODIFY);
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

void
ChatFormClass::DrawDialogHeaderBackgroundBitmap(User* pUser)
{
	if (pUser == null)
	{
		return;
	}

	__pAvatarLabel->SetBackgroundBitmap(*(Utils::getInstance().MaskBitmap(GetAvatarBitmap(pUser, -1), String(L"thumbnail_header.png"), 88, 88)));

	String name;
	name.Format(100, L"%S %S", pUser->firstName.GetPointer(), pUser->lastName.GetPointer());
	__pNameLabel->SetText(name);
	__pStatusLabel->SetText(pUser->online.ToInt() == 1 ? L"Online" : L"Offline");
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

void
ChatFormClass::OnExpandableEditAreaLineAdded(Tizen::Ui::Controls::ExpandableEditArea& source, int newLineCount)
{
	AppLogDebug("line added, size w: %d,  h: %d", source.GetWidth(), source.GetHeight());
	__pFooterPanel->SetSize(source.GetWidth() + 10, source.GetHeight() + 10);
	__pFooterPanel->Invalidate(true);
}


void
ChatFormClass::OnExpandableEditAreaLineRemoved(Tizen::Ui::Controls::ExpandableEditArea& source, int newLineCount)
{
	AppLogDebug("line removed, size w: %d,  h: %d", source.GetWidth(), source.GetHeight());
	__pFooterPanel->SetSize(source.GetWidth() + 10, source.GetHeight() + 10);
	__pFooterPanel->Invalidate(true);
}
