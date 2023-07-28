#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "SAddStateDialog.h"
#include "SCustomClassDialog.generated.h"

USTRUCT(BlueprintType) // Map of strings variable for storing starting and target state
struct FStatePairs
{
    GENERATED_BODY()

    FString StartingState;
    FString TargetState;
};

class CUSTOMCLASSCREATOR_API SCustomClassDialog : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SCustomClassDialog)
    {}

    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

    FReply OnCreateButtonClicked() const;
    FReply OnAddButtonClicked() const;
    void OnRemoveButtonClicked(int32 Index);
    FReply OnCancelButtonClicked() const;

    TSharedPtr<SAddStateDialog> AddStateDialog; 

    // Variables for File Name
    TSharedPtr<class SEditableTextBox> FileNameTextBox;
    FString DefaultFileName;
    FString FileName;


    // Variables for File Path
    TSharedPtr<SEditableTextBox> FilePathTextBox;
    FString DefaultFilePath;
    FString DefaultProjectPath;
    FString FilePath;


    // Variables for Module Dropdown
    TArray<FString> GetModuleNames() const;
    TArray<TSharedPtr<FString>> ModuleNames;
    TSharedPtr<FString> SelectedModuleName;
    TSharedPtr<FString> LastSelectedModuleName;


    // Variables for States
    TArray<FStatePairs> StatePairs;
    TSharedPtr<class SScrollBox> StatePairsScrollBox;

    void UpdateStatePairs();

    bool IsCreateButtonEnabled() const;
    EVisibility GetWarningBoxVisibility() const;
    FText GetWarningText() const;

    static bool IsAlphabeticStart(const FString& String);
    static bool IsAlphabeticAndNumeric(const FString& String);
};