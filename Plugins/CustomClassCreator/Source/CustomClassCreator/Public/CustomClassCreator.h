#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FCustomClassCreatorModule : public IModuleInterface
{
public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void RegisterMenuExtensions();
	void AddMenuEntry(FMenuBuilder& MenuBuilder);
};
