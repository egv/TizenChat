
#include "TizenChatMainForm.h"
#include "AppResourceId.h"

#include "Utils.h"

using namespace Tizen::Base;
using namespace Tizen::Base::Collection;
using namespace Tizen::App;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;

TizenChatMainForm::TizenChatMainForm(void)
{
}

TizenChatMainForm::~TizenChatMainForm(void)
{
}

bool
TizenChatMainForm::Initialize(void)
{
	Construct(IDF_FORM);

	return true;
}

result
TizenChatMainForm::OnInitializing(void)
{
	AppLogDebug("access token: %S", Utils::getInstance().accessToken()->GetPointer());

	result r = E_SUCCESS;

	// TODO:
	// Add your initialization code here
	Header* pHeader = GetHeader();
	if (pHeader)
	{
		pHeader->AddActionEventListener(*this);
	}

	// Setup back event listener
	SetFormBackEventListener(this);

	return r;
}

result
TizenChatMainForm::OnTerminating(void)
{
	result r = E_SUCCESS;

	return r;
}

void
TizenChatMainForm::OnActionPerformed(const Tizen::Ui::Control& source, int actionId)
{
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);

	switch(actionId)
	{
	case ID_HEADER_ITEM1:
		pSceneManager->GoForward(SceneTransitionId(ID_SCNT_1));
		break;
	case ID_HEADER_ITEM2:
		pSceneManager->GoForward(SceneTransitionId(ID_SCNT_2));
		break;
	case ID_HEADER_ITEM3:
		pSceneManager->GoForward(SceneTransitionId(ID_SCNT_6));
		break;
	case ID_HEADER_ITEM4:
		pSceneManager->GoForward(SceneTransitionId(ID_SCNT_3));
		break;

	default:
		break;
	}
}

void
TizenChatMainForm::OnFormBackRequested(Tizen::Ui::Controls::Form& source)
{
	UiApp* pApp = UiApp::GetInstance();
	AppAssert(pApp);
	pApp->Terminate();
}

void
TizenChatMainForm::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
		const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs)
{
	// TODO: Add your implementation codes here

}

void
TizenChatMainForm::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
		const Tizen::Ui::Scenes::SceneId& nextSceneId)
{
	// TODO: Add your implementation codes here

}
