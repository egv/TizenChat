
#include "AppResourceId.h"
#include "LoginFormClass.h"

using namespace Tizen::Base;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;



LoginFormClass::LoginFormClass(void)
{
}

LoginFormClass::~LoginFormClass(void)
{
}

bool
LoginFormClass::Initialize()
{
	Form::Construct(LoginForm);

	return true;
}

result
LoginFormClass::OnInitializing(void)
{
	result r = E_SUCCESS;

	// TODO: Add your initialization code here

	Button* pLogin_button = static_cast<Button*>(GetControl(LOGIN_BUTTON));  
	if(pLogin_button)
	{
		pLogin_button->SetActionId(100);
		pLogin_button->AddActionEventListener(*this);
	}
	return r;
}

result
LoginFormClass::OnTerminating(void)
{
	result r = E_SUCCESS;

	// TODO: Add your termination code here

	return r;
}



void
LoginFormClass::OnActionPerformed(const Tizen::Ui::Control& source, int actionId)
{
	SceneManager* pSceneManager = SceneManager::GetInstance();
	pSceneManager->GoForward(SceneTransitionId(ID_SCNT_4));
	// TODO: Add your implementation codes here

}

void
LoginFormClass::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
								const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs)
{
	// TODO: Add your implementation codes here

}

void
LoginFormClass::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
								const Tizen::Ui::Scenes::SceneId& nextSceneId)
{
	// TODO: Add your implementation codes here

}
