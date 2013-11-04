#include "TizenChatDialogsTab.h"
#include "AppResourceId.h"

#include "TizenChatDataManager.h"

using namespace Tizen::Graphics;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;

TizenChatDialogsTab::TizenChatDialogsTab(void)
{

}

TizenChatDialogsTab::~TizenChatDialogsTab(void)
{

}

bool
TizenChatDialogsTab::Initialize(void)
{
	result r = Construct(IDC_PANEL1);
	if (IsFailed(r))
		return false;

	return true;
}

result
TizenChatDialogsTab::OnInitializing(void)
{
	result r = E_SUCCESS;

	// Layout setting
	const Form* pForm = dynamic_cast<Form*>(GetParent());
	RelativeLayout* pRelativeLayout = dynamic_cast<RelativeLayout*>(pForm->GetLandscapeLayoutN());
	pRelativeLayout->SetHorizontalFitPolicy(*this, FIT_POLICY_PARENT);
	pRelativeLayout->SetVerticalFitPolicy(*this, FIT_POLICY_PARENT);
	delete pRelativeLayout;
	pRelativeLayout = dynamic_cast<RelativeLayout*>(pForm->GetPortraitLayoutN());
	pRelativeLayout->SetHorizontalFitPolicy(*this, FIT_POLICY_PARENT);
	pRelativeLayout->SetVerticalFitPolicy(*this, FIT_POLICY_PARENT);
	delete pRelativeLayout;

	TizenChatDataManager::GetInstance().AddDataManagerEventsListener(*this);
	TizenChatDataManager::GetInstance().LoadLastMessages();

	return r;
}

result
TizenChatDialogsTab::OnTerminating(void)
{
	result r = E_SUCCESS;

	TizenChatDataManager::GetInstance().RemoveDataManagerEventsListener(*this);

	return r;
}

void
TizenChatDialogsTab::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
								const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs)
{
	// TODO:
	// Add your scene activate code here
	AppLog("OnSceneActivatedN");
}

void
TizenChatDialogsTab::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
								const Tizen::Ui::Scenes::SceneId& nextSceneId)
{
	// TODO:
	// Add your scene deactivate code here
	AppLog("OnSceneDeactivated");
}

void
TizenChatDialogsTab::OnDataManagerUpdatedMessages()
{

}

void
TizenChatDialogsTab::OnDataManagerUpdatedUser(int userId)
{

}

void
TizenChatDialogsTab::OnDataManagerGotError(Tizen::Base::String errorText)
{

}

