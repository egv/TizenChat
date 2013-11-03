
#include "AppResourceId.h"
#include "SettingsPanelClass.h"

using namespace Tizen::Base;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;


SettingsPanelClass::SettingsPanelClass(void)
{
}

SettingsPanelClass::~SettingsPanelClass(void)
{
}

bool
SettingsPanelClass::Initialize()
{
	Panel::Construct(SettingsPanel);

	return true;
}

result
SettingsPanelClass::OnInitializing(void)
{
	result r = E_SUCCESS;

	// TODO: Add your initialization code here

	return r;
}

result
SettingsPanelClass::OnTerminating(void)
{
	result r = E_SUCCESS;

	// TODO: Add your termination code here

	return r;
}



void
SettingsPanelClass::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
								const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs)
{
	// TODO: Add your implementation codes here

}

void
SettingsPanelClass::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
								const Tizen::Ui::Scenes::SceneId& nextSceneId)
{
	// TODO: Add your implementation codes here

}
