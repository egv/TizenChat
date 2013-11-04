#ifndef _TIZENCHAT_TAB1_H_
#define _TIZENCHAT_TAB1_H_

#include <FApp.h>
#include <FBase.h>
#include <FSystem.h>
#include <FUi.h>
#include <FUiIme.h>
#include <FGraphics.h>
#include <gl.h>

#include "ITizenChatDataManagerEventsListener.h"

class TizenChatDialogsTab
	: public Tizen::Ui::Controls::Panel
	, public Tizen::Ui::Scenes::ISceneEventListener
	, public ITizenChatDataManagerEventsListener
 	, public Tizen::Ui::Controls::ITableViewItemProvider
{
public:
	TizenChatDialogsTab(void);
	virtual ~TizenChatDialogsTab(void);
	bool Initialize(void);

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);

	virtual void OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
								   const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs);
	virtual void OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
									const Tizen::Ui::Scenes::SceneId& nextSceneId);

	virtual void OnDataManagerUpdatedMessages();
	virtual void OnDataManagerUpdatedUser(int userId);
	virtual void OnDataManagerGotError(Tizen::Base::String errorText);

	// table stuff
	virtual int GetItemCount(void);
	virtual Tizen::Ui::Controls::TableViewItem* CreateItem(int itemIndex, int itemWidth);
	virtual Tizen::Ui::Controls::TableViewItem* CreateItemF(int itemIndex, float itemWidth);
	virtual bool DeleteItem(int itemIndex, Tizen::Ui::Controls::TableViewItem* pItem);
	virtual void UpdateItem(int itemIndex, Tizen::Ui::Controls::TableViewItem* pItem);
	virtual int GetDefaultItemHeight(void);
	virtual float GetDefaultItemHeightF(void);
};

#endif // _TIZENCHAT_TAB1_H_
