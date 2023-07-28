#include "CustomClassCreator.h"
#include "SCustomClassDialog.h"
#include "LevelEditor.h"

#define LOCTEXT_NAMESPACE "FCustomClassCreatorModule"

void FCustomClassCreatorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	RegisterMenuExtensions();
}

void FCustomClassCreatorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

void FCustomClassCreatorModule::RegisterMenuExtensions()
{
    // Register the custom menu extension
    FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
    TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender);
    MenuExtender->AddMenuExtension(
        "FileProject",
        EExtensionHook::After,
        nullptr,
        FMenuExtensionDelegate::CreateRaw(this, &FCustomClassCreatorModule::AddMenuEntry)
    );
    LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
}

void FCustomClassCreatorModule::AddMenuEntry(FMenuBuilder& MenuBuilder)
{
    // Add the "Create C++ Class" menu entry
    MenuBuilder.BeginSection("CustomClassCreator", LOCTEXT("CustomClassCreatorMenu", "Custom Class Creator"));
    {
        MenuBuilder.AddMenuEntry(
            LOCTEXT("CreateCPlusPlusClass", "Create Custom C++ Class"),
            LOCTEXT("CreateCPlusPlusClass_ToolTip", "Create a custom new C++ class."),
            FSlateIcon(),
            FUIAction(FExecuteAction::CreateLambda([] {
                // Open the custom dialog when the menu entry is clicked
                TSharedPtr<SWindow> DialogWindow = SNew(SWindow)
                    .Title(LOCTEXT("CustomClassDialog_Title", "Create Custom C++ Class"))
                    .SizingRule(ESizingRule::Autosized)
                    .SupportsMaximize(false)
                    .SupportsMinimize(false);

                TSharedRef<SCustomClassDialog> CustomClassDialog = SNew(SCustomClassDialog);
                DialogWindow->SetContent(CustomClassDialog);

                FSlateApplication::Get().AddWindow(DialogWindow.ToSharedRef());
            }))
        );
    }
    MenuBuilder.EndSection();
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCustomClassCreatorModule, CustomClassCreator)