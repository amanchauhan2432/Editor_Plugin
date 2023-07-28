#include "SAddStateDialog.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Input/SButton.h"
#include "Styling/CoreStyle.h"
#include "SCustomClassDialog.h"

void SAddStateDialog::Construct(const FArguments& InArgs)
{
    StatePairsPtr = InArgs._StatePairsPtr; // Referencing StatePairs variable from CustomClassDialog screen
	ParentWidget = InArgs._ParentWidget; // Referencing CustomClassDialog widget


    // Main UI CODE
	ChildSlot
    [
        SNew(SBox)
        .WidthOverride(700.0f)
        .HeightOverride(350.0f)
        [
            SNew(SScrollBox)
            + SScrollBox::Slot()
            [
                SNew(SVerticalBox)

                + SVerticalBox::Slot()
                .Padding(50.f, 50.f)
                [
                    SNew(SHorizontalBox)

                    + SHorizontalBox::Slot() // Starting State Box
                    [
                        SNew(STextBlock)
                        .Text(FText::FromString(TEXT("Starting State")))
                    ]

                    + SHorizontalBox::Slot()
                    .Padding(FMargin(-250, 0.f, 50.f, 0.f))
                    [
                        SNew(SEditableTextBox)
                        .Text(FText::FromString(StartingState))
                        .OnTextChanged_Lambda([this](const FText& InText) {
                            StartingState = InText.ToString();
                        })
                    ]
                ]

                + SVerticalBox::Slot()
                .Padding(50.f, 0.f)
                [
                    SNew(SHorizontalBox)

                    + SHorizontalBox::Slot() // Target State Box
                    [
                        SNew(STextBlock)
                        .Text(FText::FromString(TEXT("Target State")))
                    ]

                    + SHorizontalBox::Slot()
                    .Padding(FMargin(-250, 0.f, 50.f, 0.f))
                    [
                        SNew(SEditableTextBox)
                        .Text(FText::FromString(TargetState))
                        .OnTextChanged_Lambda([this](const FText& InText) {
                            TargetState = InText.ToString();
                        })
                    ]
                ]

                + SVerticalBox::Slot() // Warning Box
                .Padding(FMargin(50.f, 75.f, 50.f, -100.f))
                [
                    SNew(SBorder)
                    .BorderBackgroundColor(FLinearColor::Red)
                    .Visibility(this, &SAddStateDialog::GetWarningBoxVisibility)
                    [
                        SNew(STextBlock)
                        .Text(this, &SAddStateDialog::GetWarningText)
                        .Justification(ETextJustify::Center)
                    ]
                ]

                + SVerticalBox::Slot()
                .Padding(FMargin(50.f, 150.f, 50.f, 0.f))
                [
                    SNew(SHorizontalBox)

                    + SHorizontalBox::Slot()
                    .HAlign(HAlign_Left)
                    [
                        SNew(SButton)
                        .OnClicked(this, &SAddStateDialog::OnCancelButtonClicked)
                        [
                            SNew(STextBlock)
                            .Text(FText::FromString(TEXT("Cancel")))
                        ]
                    ]

                    + SHorizontalBox::Slot()
                    .HAlign(HAlign_Right)
                    [
                        SNew(SButton)
                        .OnClicked(this, &SAddStateDialog::OnAddButtonClicked)
                        .IsEnabled(this, &SAddStateDialog::IsAddButtonEnabled)
                        [
                            SNew(STextBlock)
                            .Text(FText::FromString(TEXT("Add")))
                        ]
                    ]
                ]

            ]

        ]
    ];
}



FReply SAddStateDialog::OnAddButtonClicked() const // When the Add button is pressed, store the entered states in StatePairs variable in CustomClassDialog
{
	if (StatePairsPtr && !StartingState.IsEmpty() && !TargetState.IsEmpty())
	{
		bool bStatePairExists = false;

		for (const FStatePairs& StatePair : *StatePairsPtr)
		{
			if (StatePair.StartingState == StartingState && StatePair.TargetState == TargetState)
			{
				bStatePairExists = true;
				break;
			}
		}

		if (!bStatePairExists)
		{
			StatePairsPtr->Add(FStatePairs{ StartingState, TargetState });

			TSharedPtr<SWindow> ParentWindow = FSlateApplication::Get().FindWidgetWindow(AsShared());
			if (ParentWindow.IsValid())
			{
				ParentWindow->RequestDestroyWindow();
			}
		}
	}

    ParentWidget->UpdateStatePairs(); // Update the states box to display newly entered value

	return FReply::Handled();
}

FReply SAddStateDialog::OnCancelButtonClicked() const // When the Cancel button is pressed, destroy the AddStateDialog screen
{
    TSharedPtr<SWindow> ParentWindow = FSlateApplication::Get().FindWidgetWindow(AsShared());
    if (ParentWindow.IsValid())
    {
        ParentWindow->RequestDestroyWindow();
    }
    
    return FReply::Handled();
}

bool SAddStateDialog::IsAddButtonEnabled() const // Check whether Add button should be enabled or not based on some validation checks
{
    if (StartingState.IsEmpty() || TargetState.IsEmpty() || StartingState.Contains(TEXT(" ")) || TargetState.Contains(TEXT(" "))) // States can't be empty or can't have spaces
    {
        return false;
    }

    else if (!IsAlphabeticStart(StartingState) || !IsAlphabeticStart(TargetState)) // States must start with alphabet
    {
        return false;
    }

    else if (!IsAlphabeticAndNumeric(StartingState) || !IsAlphabeticAndNumeric(TargetState)) // States can't have special symbols
    {
        return false;
    }

    if (StatePairsPtr) // Same state pair can't be entered
    {
        for (const FStatePairs& StatePair : *StatePairsPtr)
        {
            if (StatePair.StartingState == StartingState && StatePair.TargetState == TargetState)
            {
                return false;
            }
        }
    }
    return true;
}


EVisibility SAddStateDialog::GetWarningBoxVisibility() const // Show warning box when it's required
{
    if (!IsAddButtonEnabled())
    {
        return EVisibility::Visible;
    }
    else
    {
        return EVisibility::Hidden;
    }
}


FText SAddStateDialog::GetWarningText() const // Get different warning texts based on different conditions
{
    bool bStatePairExists = false;
    bool bContainsSpace = StartingState.Contains(TEXT(" ")) || TargetState.Contains(TEXT(" "));

    if (bContainsSpace) // States can't have spaces
    {
        return FText::FromString(TEXT("Your state names may not contain a space."));
    }
    
    else if (StartingState.IsEmpty() || TargetState.IsEmpty()) // States can't be empty
    {
        return FText::FromString(TEXT("Please enter both Starting State and Target State."));
    }
    
    else if (!IsAlphabeticStart(StartingState) || !IsAlphabeticStart(TargetState)) // States must start with alphabet
    {
        return FText::FromString(TEXT("State names must start with an alphabet."));
    }
    
    else if (!IsAlphabeticAndNumeric(StartingState) || !IsAlphabeticAndNumeric(TargetState)) // States can't have special symbols
    {
        return FText::FromString(TEXT("No special symbols are allowed in state names."));
    }

    else
    {
        for (const FStatePairs& StatePair : *StatePairsPtr) // Same state pair can't be entered
        {
            if (StatePair.StartingState == StartingState && StatePair.TargetState == TargetState)
            {
                bStatePairExists = true;
                break;
            }
        }
        if (bStatePairExists)
        {
            return FText::FromString(TEXT("The state pair already exists."));
        }
    }

    return FText();
}

bool SAddStateDialog::IsAlphabeticStart(const FString& String) // Check if state starts with alphabet
{
    if (String.IsEmpty())
        return false;

    TCHAR FirstChar = String[0];
    return FChar::IsAlpha(FirstChar);
}

bool SAddStateDialog::IsAlphabeticAndNumeric(const FString& String) // Check if state contains any special symbols
{
    for (TCHAR Char : String)
    {
        if (!FChar::IsAlnum(Char) && Char != TEXT('_'))
        {
            return false;
        }
    }
    return true;
}