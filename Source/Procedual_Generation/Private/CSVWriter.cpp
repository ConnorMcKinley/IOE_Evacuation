// Fill out your copyright notice in the Description page of Project Settings.


#include "CSVWriter.h"

#include "VectorTypes.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"

void UCSVWriter::WriteExperimentalSetupDetails(const FString& FilePath, const TArray<FString>& PlayerNames,
    const TArray<FString>& SettingLabels, const TArray<FString>& SettingValues)
{
    // Add the relative path to FilePath
    const FString AbsoluteFilePath = FPaths::ProjectDir() + FilePath;

    // Create a FString to be written to the CSV file
    FString CSVContent;

    AppendParticipantIndices(CSVContent, PlayerNames);

    // Extra line to separate the two sections
    CSVContent += TEXT("\n");

    FFileHelper::SaveStringToFile(CSVContent, *AbsoluteFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
}

void UCSVWriter::AppendParticipantIndices(FString& CSVContent, const TArray<FString>& PlayerNames)
{
    // Create a FString in the format of "Names, Players..." to be written to the CSV file
    CSVContent = TEXT("Names,") + ConvertToCSVFormat(PlayerNames) + TEXT("\n");

    // Add to the FString the player indices "(empty), 0, 1, 2, 3, 4, 5, 6, 7, 8, 9"
    TArray<FString> Indices;
    for (int i = 0; i < PlayerNames.Num(); i++)
    {
        Indices.Add(FString::FromInt(i));
    }
    CSVContent += TEXT(",") + ConvertToCSVFormat(Indices) + TEXT("\n");
}

void UCSVWriter::AppendSettingConfig(FString& CSVContent, const TArray<FString>& SettingLabels, TArray<FString>& SettingValues)
{
    // Create a FString in the format of "SettingLabels, SettingValues..." to be written to the CSV file
    CSVContent += TEXT("SettingLabels,") + ConvertToCSVFormat(SettingLabels) + TEXT("\n");
    CSVContent += TEXT("SettingValues,") + ConvertToCSVFormat(SettingValues) + TEXT("\n");
}


FString UCSVWriter::ConvertToCSVFormat(const TArray<FString>& StringArray)
{
    FString Result;
    for (const FString& Value : StringArray)
    {
        Result += Value + TEXT(",");
    }
    Result.RemoveFromEnd(TEXT(","));
    return Result;
}