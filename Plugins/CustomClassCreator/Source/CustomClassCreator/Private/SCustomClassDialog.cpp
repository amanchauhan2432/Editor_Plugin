#include "SCustomClassDialog.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Input/SButton.h"
#include "Styling/CoreStyle.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h" 

void SCustomClassDialog::Construct(const FArguments& InArgs)
{
    // Referencing AddStateDialog
    AddStateDialog = SNew(SAddStateDialog)
	.StatePairsPtr(&StatePairs)            // Passing StatePairs variable
	.ParentWidget(SharedThis(this));       // Passing CustomClassDialog Widget


    // Setting FileName
    DefaultFileName = TEXT("MyClass");
    FileName = DefaultFileName;

    // Setting FilePath
    DefaultFilePath = FPaths::ProjectDir();
    DefaultFilePath /= TEXT("Source");
    DefaultFilePath /= FApp::GetProjectName();
    DefaultFilePath /= TEXT("/");
    
    FilePath = DefaultFilePath;
    DefaultProjectPath = DefaultFilePath;


    // Getting all the module names in the project
    TArray<FString> ModuleNameArray = GetModuleNames();
    for (const FString& ModuleName : ModuleNameArray)
    {
        ModuleNames.Add(MakeShareable(new FString(ModuleName)));
    }
    
    SelectedModuleName = LastSelectedModuleName.IsValid() ? LastSelectedModuleName : ModuleNames[0];


    // Creating SScrollBox for storing state names
    StatePairsScrollBox = SNew(SScrollBox);


    // Main UI CODE
    ChildSlot
    [
        SNew(SBox)
        .WidthOverride(930.0f)
        .HeightOverride(500.0f)
        [
            SNew(SScrollBox)
            + SScrollBox::Slot()
            [
                SNew(SVerticalBox)

                + SVerticalBox::Slot() // Warning Box
                .Padding(50.0f, 20.0f)
                [
                    SNew(SBorder)
                    .BorderBackgroundColor(FLinearColor::Red)
                    .Visibility(this, &SCustomClassDialog::GetWarningBoxVisibility)
                    [
                        SNew(STextBlock)
                        .Text(this, &SCustomClassDialog::GetWarningText)
                        .Justification(ETextJustify::Center)
                    ]
                ]

                + SVerticalBox::Slot() // FileName Box
                .Padding(50.f, 10.f)
                [
                    SNew(SHorizontalBox)

                    + SHorizontalBox::Slot()
                    [
                        SNew(STextBlock)
                        .Text(FText::FromString(TEXT("File Name")))
                    ]

                    + SHorizontalBox::Slot()
                    .Padding(FMargin(-700, 0.f, 0.f, 0.f))
                    [
                        SNew(SEditableTextBox)
                        .Text(FText::FromString(FileName))
                        .OnTextChanged_Lambda([this](const FText& InText) {
                            FileName = InText.ToString();
                        })
                    ]
                ]

                + SVerticalBox::Slot() // Module Dropdown Box
                .Padding(50.f, 10.f)
                [
                    SNew(SHorizontalBox)

                    + SHorizontalBox::Slot()
                    [
                        SNew(STextBlock)
                        .Text(FText::FromString(TEXT("Module")))
                    ]

                    + SHorizontalBox::Slot()
                    .Padding(FMargin(-700, 0.f, 0.f, 0.f))
                    [
                        SNew(SComboBox<TSharedPtr<FString>>)
                        .OptionsSource(&ModuleNames)
                        .OnGenerateWidget_Lambda([](TSharedPtr<FString> InItem) {
                            return SNew(STextBlock)
                                .Text(FText::FromString(*InItem.Get()));
                        })
                        .OnSelectionChanged_Lambda([this](TSharedPtr<FString> SelectedItem, ESelectInfo::Type SelectType) {
                            if (SelectedItem.IsValid())
                            {
                                SelectedModuleName = SelectedItem;
                                LastSelectedModuleName = SelectedModuleName;

                                if (SelectedModuleName->Equals(FApp::GetProjectName())) // Updating the path based on module name
                                {
                                    FilePath = DefaultFilePath;
                                    DefaultProjectPath = DefaultFilePath;
                                }
                                else
                                {
                                    FString PluginsFolderPath = FPaths::ProjectPluginsDir();
                                    FString PluginPath = PluginsFolderPath / *SelectedModuleName.Get();
                                    FilePath = PluginPath / TEXT("Source") / *SelectedModuleName.Get() / TEXT("/");
                                    DefaultProjectPath = PluginPath / TEXT("Source") / *SelectedModuleName.Get() / TEXT("/");
                                }

                                if (FilePathTextBox.IsValid())
                                {
                                    FilePathTextBox->SetText(FText::FromString(FilePath));
                                }
                            }
                        })
                        .Content()
                        [
                            SNew(STextBlock)
                            .Text_Lambda([this] {
                                return FText::FromString(SelectedModuleName.IsValid() ? *SelectedModuleName.Get() : TEXT(""));
                            })
                        ]
                    ]
                ]

                + SVerticalBox::Slot() // FilePath Box
                .Padding(50.f, 10.f)
                [
                    SNew(SHorizontalBox)

                    + SHorizontalBox::Slot()
                    [
                        SNew(STextBlock)
                        .Text(FText::FromString(TEXT("Path")))
                    ]

                    + SHorizontalBox::Slot()
                    .Padding(FMargin(-700, 0.f, 0.f, 0.f))
                    [
                        SAssignNew(FilePathTextBox, SEditableTextBox)
                        .Text(FText::FromString(FilePath))
                        .OnTextChanged_Lambda([this](const FText& InText) {
                            FilePath = InText.ToString();
                        })
                    ]
                ]

                + SVerticalBox::Slot() // States Box
                .Padding(50.f, 10.f)
                [
                    SNew(STextBlock)
                    .Text(FText::FromString(TEXT("States")))
                ]

                + SVerticalBox::Slot()
                .Padding(50.f, 10.f)
                .AutoHeight()
                [
                    SNew(SBox)
                    .HeightOverride(235.f)
                    [
                        SNew(SBorder)
                        .BorderBackgroundColor(FLinearColor(0.1f, 0.1f, 0.1f, 1.f))
                        [
                            SNew(SVerticalBox)
                            + SVerticalBox::Slot()
                            .FillHeight(5.f)
                            [
                                SAssignNew(StatePairsScrollBox, SScrollBox)
                            ]
                            + SVerticalBox::Slot()
                            .HAlign(HAlign_Right)
                            .VAlign(VAlign_Bottom)
                            .Padding(FMargin(0.f, 0.f, 10.f, 10.f))
                            [
                                SNew(SButton)
                                .OnClicked(this, &SCustomClassDialog::OnAddButtonClicked)
                                [
                                    SNew(STextBlock)
                                    .Text(FText::FromString(TEXT("+")))
                                ]
                            ]
                        ]
                    ]
                ]

                + SVerticalBox::Slot()
                .Padding(FMargin(50.f, 20.f, 50.f, 0.f))
                [
                    SNew(SHorizontalBox)

                    + SHorizontalBox::Slot()
                    .HAlign(HAlign_Left)
                    [
                        SNew(SButton)
                        .OnClicked(this, &SCustomClassDialog::OnCancelButtonClicked)
                        [
                            SNew(STextBlock)
                            .Text(FText::FromString(TEXT("Cancel")))
                        ]
                    ]

                    + SHorizontalBox::Slot()
                    .HAlign(HAlign_Right)
                    [
                        SNew(SButton)
                        .OnClicked(this, &SCustomClassDialog::OnCreateButtonClicked)
                        .IsEnabled(this, &SCustomClassDialog::IsCreateButtonEnabled)
                        [
                            SNew(STextBlock)
                            .Text(FText::FromString(TEXT("Create")))
                        ]
                    ]
                ]
            ]
        ]
    ];
    if (FileNameTextBox.IsValid())
    {
        FileNameTextBox->SetText(FText::FromString(DefaultFileName));
    }
    if (FilePathTextBox.IsValid())
    {
        FilePathTextBox->SetText(FText::FromString(DefaultFilePath));
    }
}


FReply SCustomClassDialog::OnCreateButtonClicked() const
{
    // Create the header file with the given name and path
    FString HFilePath;
    if (ModuleNames.IsValidIndex(0) && SelectedModuleName == ModuleNames[0])
    {
        HFilePath = FPaths::ProjectDir() / TEXT("Source") / FApp::GetProjectName() / TEXT("/");
    }
    else
    {
        HFilePath = FPaths::ProjectPluginsDir() / *SelectedModuleName.Get() / TEXT("Source") / *SelectedModuleName.Get() / TEXT("Public/");
    }
    FString HeaderFilePath = FPaths::Combine(HFilePath, FileName + TEXT(".h"));

    FString HeaderFileContent;

    // Construct the header file content with Unreal Engine standards
    HeaderFileContent += TEXT("#pragma once\n\n");
    HeaderFileContent += FString::Printf(TEXT("#include \"%s.generated.h\"\n\n"), *FileName);

    for (const FStatePairs& StatePair : StatePairs)
    {
        HeaderFileContent += FString::Printf(TEXT("struct %s\n"), *StatePair.StartingState);
        HeaderFileContent += TEXT("{\n    GENERATED_BODY()\n\n    // Add your code here\n\n};\n\n");

        HeaderFileContent += FString::Printf(TEXT("struct %s\n"), *StatePair.TargetState);
        HeaderFileContent += TEXT("{\n    GENERATED_BODY()\n\n    // Add your code here\n\n};\n\n");
    }

    HeaderFileContent += TEXT("\n\n");
    HeaderFileContent += TEXT("class ");
    HeaderFileContent += (*SelectedModuleName.Get()->ToUpper());
    HeaderFileContent += TEXT("_API ");
    HeaderFileContent += FileName;
    HeaderFileContent += TEXT("\n{\n\n");
    HeaderFileContent += TEXT("};\n");

    // Write the content to the header file
    if (FFileHelper::SaveStringToFile(HeaderFileContent, *HeaderFilePath))
    {
        UE_LOG(LogTemp, Warning, TEXT("Header file created successfully at: %s"), *HeaderFilePath);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create header file at: %s"), *HeaderFilePath);
    }

    // Close the dialog box
    TSharedPtr<SWindow> ParentWindow = FSlateApplication::Get().FindWidgetWindow(AsShared());
    if (ParentWindow.IsValid())
    {
        ParentWindow->RequestDestroyWindow();
    }

    return FReply::Handled();
}


FReply SCustomClassDialog::OnAddButtonClicked() const // When the Add button is pressed, opening the AddStateDialog screen
{
    if (AddStateDialog.IsValid())
    {
        TSharedRef<SWindow> Window = SNew(SWindow)
            .Title(FText::FromString(TEXT("Add State Dialog")))
            .SizingRule(ESizingRule::Autosized)
            .SupportsMaximize(false)
            .SupportsMinimize(false);

        Window->SetContent(AddStateDialog.ToSharedRef());

        FSlateApplication::Get().AddWindow(Window);
    }

    return FReply::Handled();
}

void SCustomClassDialog::OnRemoveButtonClicked(int32 Index) // When the X button is pressed, remove that state value from StatePairs variable
{
    if (Index >= 0 && Index < StatePairs.Num())
    {
        StatePairs.RemoveAt(Index);
        UpdateStatePairs();
    }
}

FReply SCustomClassDialog::OnCancelButtonClicked() const // When the Cancel button is pressed, destroy the CustomClassDialog screen
{
    TSharedPtr<SWindow> ParentWindow = FSlateApplication::Get().FindWidgetWindow(AsShared());
    if (ParentWindow.IsValid())
    {
        ParentWindow->RequestDestroyWindow();
    }
    
    return FReply::Handled();
}

TArray<FString> SCustomClassDialog::GetModuleNames() const // Find all the module names which exist in this project and return their names
{
    TArray<FString> ModuleFields;

    FString ProjectName = FApp::GetProjectName();
    ModuleFields.Add(ProjectName);

    FString PluginsFolderPath = FPaths::ProjectPluginsDir();

    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    if (PlatformFile.DirectoryExists(*PluginsFolderPath))
    {
        PlatformFile.IterateDirectory(*PluginsFolderPath, [&ModuleFields](const TCHAR* FilenameOrDirectory, bool bIsDirectory) {
            if (bIsDirectory)
            {
                FString DirectoryName = FPaths::GetCleanFilename(FilenameOrDirectory);
                ModuleFields.Add(DirectoryName);
            }
            return true;
        });
    }

    return ModuleFields;
}

void SCustomClassDialog::UpdateStatePairs() // When the Add button is pressed of AddStateDialog screen, update the StatePairs variable in this screen
{
    StatePairsScrollBox->ClearChildren();

    for (int32 Index = 0; Index < StatePairs.Num(); ++Index)
    {
        const FStatePairs& StatePair = StatePairs[Index];

        StatePairsScrollBox->AddSlot()
            .Padding(50.f, 10.f)
            [
                SNew(SHorizontalBox)

                + SHorizontalBox::Slot()
                .AutoWidth()
                [
                    SNew(SButton)
                    .OnClicked_Lambda([this, Index]() {
                        OnRemoveButtonClicked(Index);
                        return FReply::Handled();
                    })
                    [
                        SNew(STextBlock)
                        .Text(FText::FromString(TEXT("X")))
                    ]
                ]

                + SHorizontalBox::Slot()
                .Padding(10.f, 0.f)
                [
                    SNew(STextBlock)
                    .Text(FText::FromString(FString::Printf(TEXT("%s -> %s"), *StatePair.StartingState, *StatePair.TargetState)))
                ]
            ];
    }
}


bool SCustomClassDialog::IsCreateButtonEnabled() const // Check whether Create class button should be enabled or not based on some validation checks
{
    FString HFile = FilePath / FileName + TEXT(".h");

    if (FileName.IsEmpty() || FileName.Contains(TEXT(" "))) // FileName can't be empty or can't have spaces
    {
        return false;
    }

    else if (!IsAlphabeticStart(FileName)) // FileName must start with alphabet
    {
        return false;
    }

    else if (!IsAlphabeticAndNumeric(FileName)) // FileName can't have special symbols
    {
        return false;
    }
    
    else if (!FilePath.Contains(DefaultProjectPath)) // Files can only be created inside of project source directory
    {
        return false;
    }
    else // Check if file already exists
    {
        TArray<FString> ProjectDirectories;
        FString ProjectSourcePath = FPaths::ProjectDir();
        IFileManager::Get().FindFilesRecursive(ProjectDirectories, *ProjectSourcePath, *(FileName + TEXT(".h")), true, false);
        if (ProjectDirectories.Num() > 0)
        {
            return false;
        }
    }

    return true;
}

EVisibility SCustomClassDialog::GetWarningBoxVisibility() const // Show warning box when it's required
{
    if (!IsCreateButtonEnabled())
    {
        return EVisibility::Visible;
    }
    else
    {
        return EVisibility::Hidden;
    }
}



FText SCustomClassDialog::GetWarningText() const // Get different warning texts based on different conditions
{
    FString HFile = FilePath / FileName + TEXT(".h");

    if (FileName.IsEmpty()) // FileName can't be empty or cant have spaces
    {
        return FText::FromString(TEXT("You must specify a class name."));
    }

    else if (FileName.Contains(TEXT(" "))) // FileName can't have spaces
    {
        return FText::FromString(TEXT("Your class name may not contain a space."));
    }

    else if (!IsAlphabeticStart(FileName)) // FileName must start with alphabet
    {
        return FText::FromString(TEXT("Class name must start with an alphabet."));
    }

    else if (!IsAlphabeticAndNumeric(FileName)) // FileName can't have special symbols
    {
        return FText::FromString(TEXT("No special symbols are allowed in class name."));
    }

    else if (!FilePath.Contains(DefaultProjectPath)) // Files can only be created inside of project source directory
    {
        return FText::Format(FText::FromString(TEXT("All source code must exist within {0}")), FText::FromString(DefaultProjectPath));
    }
    else // Check if file already exists
    {
        TArray<FString> ProjectDirectories;
        FString ProjectSourcePath = FPaths::ProjectDir();
        IFileManager::Get().FindFilesRecursive(ProjectDirectories, *ProjectSourcePath, *(FileName + TEXT(".h")), true, false);
        if (ProjectDirectories.Num() > 0)
        {
            return FText::FromString(TEXT("File already exist with this name."));
        }
    }

    return FText();
}

bool SCustomClassDialog::IsAlphabeticStart(const FString& String) // Check if name starts with alphabet
{
    if (String.IsEmpty())
        return false;

    TCHAR FirstChar = String[0];
    return FChar::IsAlpha(FirstChar);
}

bool SCustomClassDialog::IsAlphabeticAndNumeric(const FString& String) // Check if name contains any special symbols
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