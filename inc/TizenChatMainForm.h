#ifndef _TIZENCHAT_MAIN_FORM_H_
#define _TIZENCHAT_MAIN_FORM_H_

#include <FApp.h>
#include <FBase.h>
#include <FSystem.h>
#include <FUi.h>
#include <FUiIme.h>
#include <FGraphics.h>
#include <gl.h>

class TizenChatMainForm
	: public Tizen::Ui::Controls::Form
	, public Tizen::Ui::IActionEventListener
	, public Tizen::Ui::Controls::IFormBackEventListener
	, public Tizen::Ui::Scenes::ISceneEventListener

{
public:
	TizenChatMainForm(void);
	virtual ~TizenChatMainForm(void);
	bool Initialize(void);

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnFormBackRequested(Tizen::Ui::Controls::Form& source);
	virtual void OnActionPerformed(const Tizen::Ui::Control& source, int actionId);

protected:
	static const int ID_HEADER_ITEM1 = 101;
	static const int ID_HEADER_ITEM2 = 102;
	static const int ID_HEADER_ITEM3 = 103;
	static const int ID_HEADER_ITEM4 = 104;

	virtual void OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
								   const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs);
	virtual void OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
									const Tizen::Ui::Scenes::SceneId& nextSceneId);
};

#endif	//_TIZENCHAT_MAIN_FORM_H_
