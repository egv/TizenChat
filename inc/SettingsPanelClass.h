
#ifndef _SETTINGSPANELCLASS_H_
#define _SETTINGSPANELCLASS_H_

#include <FBase.h>
#include <FUi.h>



class SettingsPanelClass :
	public Tizen::Ui::Controls::Panel,
 	public Tizen::Ui::Scenes::ISceneEventListener
{

// Construction
public:
	SettingsPanelClass(void);
	virtual ~SettingsPanelClass(void);
	bool Initialize();
	result OnInitializing(void);
	result OnTerminating(void);

// Implementation
protected:

// Generated call-back functions
public:


	virtual void OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
								   const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs);
	virtual void OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
									const Tizen::Ui::Scenes::SceneId& nextSceneId);
};

#endif
