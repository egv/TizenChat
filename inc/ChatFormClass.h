
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
 	, public Tizen::Ui::Controls::IExpandableEditAreaEventListener
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

	// expandable area
	virtual void OnExpandableEditAreaLineAdded(Tizen::Ui::Controls::ExpandableEditArea& source, int newLineCount);
	virtual void OnExpandableEditAreaLineRemoved(Tizen::Ui::Controls::ExpandableEditArea& source, int newLineCount);

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

	Tizen::Ui::Controls::TableView* __pTableView;

	Tizen::Ui::Controls::Panel* __pHeaderPanel;
	Tizen::Ui::Controls::Label* __pAvatarLabel;
	Tizen::Ui::Controls::Label* __pDotLabel;
	Tizen::Ui::Controls::Label* __pNameLabel;
	Tizen::Ui::Controls::Label* __pStatusLabel;

	Tizen::Ui::Controls::Panel* __pFooterPanel;
	Tizen::Ui::Controls::ExpandableEditArea* __pExpEditArea;

	Tizen::Graphics::Bitmap* GetAvatarBitmap(User* pUser, int itemIndex);
	void DrawDialogHeaderBackgroundBitmap(User* pUser);
	Tizen::Graphics::Bitmap* GetMultichatHeaderBackgroundBitmap();
};

#endif
