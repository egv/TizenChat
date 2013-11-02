#include "TizenChatFormFactory.h"
#include "TizenChatMainForm.h"
#include "AppResourceId.h"
#include "LoginFormClass.h"
#include "GetTokenFormClass.h"

using namespace Tizen::Ui::Scenes;


TizenChatFormFactory::TizenChatFormFactory(void)
{
}

TizenChatFormFactory::~TizenChatFormFactory(void)
{
}

Tizen::Ui::Controls::Form*
TizenChatFormFactory::CreateFormN(const Tizen::Base::String& formId, const Tizen::Ui::Scenes::SceneId& sceneId)
{
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);
	Tizen::Ui::Controls::Form* pNewForm = null;

	if (formId == IDF_FORM)
	{
		TizenChatMainForm* pForm = new TizenChatMainForm();
		pForm->Initialize();
		pNewForm = pForm;
	}
	else if (formId == LoginForm)
	{
		LoginFormClass* pForm = new LoginFormClass();
		pForm->Initialize();
		pNewForm = pForm;
	}
	else if (formId == GetTokenForm)
	{
		GetTokenFormClass* pForm = new GetTokenFormClass();
		pForm->Initialize();
		pNewForm = pForm;
	}


	// TODO:
	// Add your form creation code here

	return pNewForm;
}
