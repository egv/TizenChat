#include "TizenChatTab2.h"
#include "AppResourceId.h"

using namespace Tizen::Graphics;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;

TizenChatTab2::TizenChatTab2(void)
{

}

TizenChatTab2::~TizenChatTab2(void)
{

}

bool
TizenChatTab2::Initialize(void)
{
	result r = Construct(IDC_PANEL2);
	if (IsFailed(r))
		return false;

	return true;
}

result
TizenChatTab2::OnInitializing(void)
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

	return r;
}

result
TizenChatTab2::OnTerminating(void)
{
	result r = E_SUCCESS;

	// TODO: Add your termination code here

	return r;
}

void
TizenChatTab2::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
								const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs)
{
	// TODO:
	// Add your scene activate code here
	AppLog("OnSceneActivatedN");
}

void
TizenChatTab2::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
								const Tizen::Ui::Scenes::SceneId& nextSceneId)
{
	// TODO:
	// Add your scene deactivate code here
	AppLog("OnSceneDeactivated");
}
