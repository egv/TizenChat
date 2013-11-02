#include "TizenChatPanelFactory.h"
#include "TizenChatTab1.h"
#include "TizenChatTab2.h"
#include "TizenChatTab3.h"
#include "AppResourceId.h"

using namespace Tizen::Ui::Scenes;


TizenChatPanelFactory::TizenChatPanelFactory(void)
{
}

TizenChatPanelFactory::~TizenChatPanelFactory(void)
{
}

Tizen::Ui::Controls::Panel*
TizenChatPanelFactory::CreatePanelN(const Tizen::Base::String& panelId, const Tizen::Ui::Scenes::SceneId& sceneId)
{
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);
	Tizen::Ui::Controls::Panel* pNewPanel = null;

	if (panelId == IDC_PANEL1)
	{
		TizenChatTab1* pPanel = new TizenChatTab1();
		pPanel->Initialize();
		pSceneManager->AddSceneEventListener(sceneId, *pPanel);
		pNewPanel = pPanel;
	}
	else
	if (panelId == IDC_PANEL2)
	{
		TizenChatTab2* pPanel = new TizenChatTab2();
		pPanel->Initialize();
		pSceneManager->AddSceneEventListener(sceneId, *pPanel);
		pNewPanel = pPanel;
	}
	else
	if (panelId == IDC_PANEL3)
	{
		TizenChatTab3* pPanel = new TizenChatTab3();
		pPanel->Initialize();
		pSceneManager->AddSceneEventListener(sceneId, *pPanel);
		pNewPanel = pPanel;
	}
	// TODO:
	// Add your panel creation code here
	return pNewPanel;
}
