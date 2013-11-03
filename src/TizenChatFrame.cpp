#include "TizenChatFrame.h"
#include "TizenChatFormFactory.h"
#include "TizenChatPanelFactory.h"
#include "AppResourceId.h"
#include "Utils.h"


using namespace Tizen::Base;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;

TizenChatFrame::TizenChatFrame(void)
{
}

TizenChatFrame::~TizenChatFrame(void)
{
}

result
TizenChatFrame::OnInitializing(void)
{
	// TODO: grow some balls and set default font at last
//	{
//		result r;
//		String fontName(L"TizenSansRegular.ttf");
//		r = UiConfiguration::SetDefaultFont(fontName);
//
//		if (r == E_SUCCESS)
//		{
//			AppLogDebug("default font set");
//		}
//		else
//		{
//			AppLogDebug("failed setting font");
//		}
//	}

	// Prepare Scene management.
	SceneManager* pSceneManager = SceneManager::GetInstance();
	static TizenChatFormFactory formFactory;
	static TizenChatPanelFactory panelFactory;
	pSceneManager->RegisterFormFactory(formFactory);
	pSceneManager->RegisterPanelFactory(panelFactory);
	pSceneManager->RegisterScene(L"workflow");

	// Go to the scene.
	result r = E_FAILURE;
	if (Utils::getInstance().accessToken() != null)
	{
		r = pSceneManager->GoForward(SceneTransitionId(ID_SCNT_NEXT_START));
	}
	else
	{
		r = pSceneManager->GoForward(SceneTransitionId(ID_SCNT_START));
	}

	// TODO: Add your initialization code here
	return r;
}

result
TizenChatFrame::OnTerminating(void)
{
	result r = E_SUCCESS;

	// TODO:
	// Add your termination code here
	return r;
}
