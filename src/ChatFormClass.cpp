
#include "AppResourceId.h"
#include "ChatFormClass.h"

#include <FBase.h>
#include <FUi.h>

using namespace Tizen::Base;
using namespace Tizen::Base::Collection;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;

ChatFormClass::ChatFormClass(void)
{
}

ChatFormClass::~ChatFormClass(void)
{
}

bool
ChatFormClass::Initialize()
{
	Form::Construct(ChatForm);

	return true;
}

result
ChatFormClass::OnInitializing(void)
{
	result r = E_SUCCESS;

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
	}
}

void
ChatFormClass::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
								const Tizen::Ui::Scenes::SceneId& nextSceneId)
{

}
