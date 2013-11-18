
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
	AppLog("on initializing main form");
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
	pSceneManager->GoForward(SceneTransitionId(ID_SCNT_3));
	AppAssert(pSceneManager);

	switch(actionId)
	{
	case ID_HEADER_ITEM1:
		pSceneManager->GoForward(ForwardSceneTransition(L"Messages"));
		break;
	case ID_HEADER_ITEM2:
		pSceneManager->GoForward(ForwardSceneTransition(L"Contacts"));
		break;
	case ID_HEADER_ITEM3:
		pSceneManager->GoForward(ForwardSceneTransition(L"Search"));
		break;
	case ID_HEADER_ITEM4:
		pSceneManager->GoForward(ForwardSceneTransition(L"Settings"));
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
}

void
TizenChatMainForm::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
		const Tizen::Ui::Scenes::SceneId& nextSceneId)
{
	AppLogDebug("OnSceneDeactivated");
}
