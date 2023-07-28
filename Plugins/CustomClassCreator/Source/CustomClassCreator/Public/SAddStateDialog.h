#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

struct FStatePairs;
class SCustomClassDialog; 

class CUSTOMCLASSCREATOR_API SAddStateDialog : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SAddStateDialog)
    {}

    SLATE_ARGUMENT(TArray<FStatePairs>*, StatePairsPtr)           // Passing StatePairs variable from CustomClassDialog
	SLATE_ARGUMENT(TSharedPtr<SCustomClassDialog>, ParentWidget)  // Passing CustomClassDialog Widget
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

    FReply OnAddButtonClicked() const;
    FReply OnCancelButtonClicked() const;

    bool IsAddButtonEnabled() const;

    FString StartingState;
    FString TargetState;

    TArray<FStatePairs>* StatePairsPtr;
	TSharedPtr<SCustomClassDialog> ParentWidget;

    EVisibility GetWarningBoxVisibility() const;
    FText GetWarningText() const;

    static bool IsAlphabeticStart(const FString& String);
    static bool IsAlphabeticAndNumeric(const FString& String);
};