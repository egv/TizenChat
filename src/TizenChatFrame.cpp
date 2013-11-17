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

	static const wchar_t* PANEL_BLANK = L"";

	pSceneManager->RegisterScene(L"Messages", L"IDF_FORM", L"IDC_PANEL1");
	pSceneManager->RegisterScene(L"Contacts", L"IDF_FORM", L"IDC_PANEL2");
	pSceneManager->RegisterScene(L"Search", L"IDF_FORM", L"IDC_PANEL3");
	pSceneManager->RegisterScene(L"Settings", L"IDF_FORM", L"SettingsPanel");
	pSceneManager->RegisterScene(L"LOGIN_SCENE", L"LoginForm", PANEL_BLANK);
	pSceneManager->RegisterScene(L"IDSCN_1", L"GetTokenForm", PANEL_BLANK);
	pSceneManager->RegisterScene(L"CHAT_SCENE", L"ChatForm", PANEL_BLANK);

	// Go to the scene.
	result r = E_FAILURE;
	if (Utils::getInstance().accessToken() != null)
	{
		r = pSceneManager->GoForward(ForwardSceneTransition(L"Messages"));
	}
	else
	{
		r = pSceneManager->GoForward(ForwardSceneTransition(L"LOGIN_SCENE"));
	}

	return r;
}

result
TizenChatFrame::OnTerminating(void)
{
	result r = E_SUCCESS;

	return r;
}
