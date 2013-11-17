#ifndef _TIZENCHAT_TAB1_H_
#define _TIZENCHAT_TAB1_H_

#include <FApp.h>
#include <FBase.h>
#include <FSystem.h>
#include <FUi.h>
#include <FUiIme.h>
#include <FGraphics.h>
#include <gl.h>

#include "User.h"

#include "IImagesManagerDelegate.h"
#include "ITizenChatDataManagerEventsListener.h"

class TizenChatDialogsTab
	: public Tizen::Ui::Controls::Panel
	, public Tizen::Ui::Scenes::ISceneEventListener
 	, public Tizen::Ui::Controls::ITableViewItemProvider
 	, public Tizen::Ui::Controls::ITableViewItemEventListener
	, public ITizenChatDataManagerEventsListener
 	, public IImagesManagerDelegate
{
public:
	TizenChatDialogsTab(void);
	virtual ~TizenChatDialogsTab(void);
	bool Initialize(void);

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);

	//
	// Scene handling stuff
	//
	virtual void OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
								   const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs);
	virtual void OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
									const Tizen::Ui::Scenes::SceneId& nextSceneId);

	//
	// Data manager delegate interface
	//
	virtual void OnDataManagerUpdatedLongPollServerData();
	virtual void OnDataManagerUpdatedMessages();
	virtual void OnDataManagerUpdatedUser(int userId);
	virtual void OnDataManagerGotError(Tizen::Base::LongLong errorCode, Tizen::Base::String errorText);

	//
	// Images manager stuff
	//
	virtual void OnImageManagerDownloadedImage(Tizen::Graphics::Bitmap* pBitmap, Tizen::Base::Collection::HashMap* userInfo);
	virtual void OnImageManagerDownloadFailed(Tizen::Base::Collection::HashMap* userInfo);

	//
	// table stuff
	//
	virtual int GetItemCount(void);
	virtual Tizen::Ui::Controls::TableViewItem* CreateItem(int itemIndex, int itemWidth);
	virtual bool DeleteItem(int itemIndex, Tizen::Ui::Controls::TableViewItem* pItem);
	virtual void UpdateItem(int itemIndex, Tizen::Ui::Controls::TableViewItem* pItem);
	virtual int GetDefaultItemHeight(void);

	//
	// table view item stuff
	//
	virtual void OnTableViewItemStateChanged(Tizen::Ui::Controls::TableView& tableView, int itemIndex, Tizen::Ui::Controls::TableViewItem* pItem, Tizen::Ui::Controls::TableViewItemStatus status);
	virtual void OnTableViewContextItemActivationStateChanged(Tizen::Ui::Controls::TableView& tableView, int itemIndex, Tizen::Ui::Controls::TableViewContextItem* pContextItem, bool activated);
	virtual void OnTableViewItemReordered(Tizen::Ui::Controls::TableView& tableView, int itemIndexFrom, int itemIndexTo);

private:
	Tizen::Base::Collection::ArrayList* __pMessagesList;

	static const int INVALID_TOKEN_ERROR_CODE = 5;

	void LoadChatHistory();

	Tizen::Graphics::Bitmap* GetAvatarBitmap(User* pUser, int itemIndex);

};

#endif // _TIZENCHAT_TAB1_H_
