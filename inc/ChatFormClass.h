
#ifndef _CHATFORMCLASS_H_
#define _CHATFORMCLASS_H_

#include <FBase.h>
#include <FUi.h>
#include <FGraphics.h>

#include "User.h"
#include "IImagesManagerDelegate.h"
#include "ITizenChatDataManagerEventsListener.h"

class ChatFormClass :
	public Tizen::Ui::Controls::Form
	, public Tizen::Ui::Scenes::ISceneEventListener
 	, public Tizen::Ui::Controls::IFormBackEventListener
 	, public Tizen::Ui::Controls::ITableViewItemProvider
 	, public ITizenChatDataManagerEventsListener
 	, public IImagesManagerDelegate
{

// Construction
public:
	ChatFormClass(void);
	virtual ~ChatFormClass(void);
	bool Initialize();
	result OnInitializing(void);
	result OnTerminating(void);

	// IFormBackEventListener
	virtual void OnFormBackRequested(Tizen::Ui::Controls::Form& source);

	// ISceneEventListener
	virtual void OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousScene, const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs);
	virtual void OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId, const Tizen::Ui::Scenes::SceneId& nextSceneId);

	// ITableViewItemProvider
	virtual int GetItemCount(void);
	virtual Tizen::Ui::Controls::TableViewItem* CreateItem(int itemIndex, int itemWidth);
	virtual bool DeleteItem(int itemIndex, Tizen::Ui::Controls::TableViewItem* pItem);
	virtual void UpdateItem(int itemIndex, Tizen::Ui::Controls::TableViewItem* pItem);
	virtual int GetDefaultItemHeight(void);

	// data manager delegate
	virtual void OnDataManagerUpdatedLongPollServerData() {};
	virtual void OnDataManagerUpdatedUser(int userId) {};
	virtual void OnDataManagerUpdatedMessages();
	virtual void OnDataManagerGotError(Tizen::Base::LongLong errorCode, Tizen::Base::String errorText);

	// Images manager stuff
	virtual void OnImageManagerDownloadedImage(Tizen::Graphics::Bitmap* pBitmap, Tizen::Base::Collection::HashMap* userInfo);
	virtual void OnImageManagerDownloadFailed(Tizen::Base::Collection::HashMap* userInfo);

private:
	static const int INVALID_TOKEN_ERROR_CODE = 5;

	int __chatId;
	Tizen::Base::Collection::HashMap* __pHeightsCache;
	Tizen::Base::Collection::ArrayList* __pMessages;
	int __userId;

	Tizen::Graphics::Bitmap* GetAvatarBitmap(User* pUser, int itemIndex);
	Tizen::Graphics::Bitmap* GetDialogHeaderBackgroundBitmap(User* pUser);
	Tizen::Graphics::Bitmap* GetMultichatHeaderBackgroundBitmap();
};

#endif
