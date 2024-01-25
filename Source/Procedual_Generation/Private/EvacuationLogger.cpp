// Fill out your copyright notice in the Description page of Project Settings.


#include "EvacuationLogger.h"

// Sets default values for this component's properties
UEvacuationLogger::UEvacuationLogger()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEvacuationLogger::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEvacuationLogger::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
void UEvacuationLogger::WriteExperimentalSetupDetails(const FString& FilePath, const TArray<FString>& PlayerNames,
	const TArray<FString>& SettingLabels, const TArray<FString>& SettingValues)
{
	// Create a FString to be written to the CSV file
	FString CSVContent;

	AppendParticipantIndices(CSVContent, PlayerNames);

	// Extra line to separate the two sections
	CSVContent += TEXT("\n");

	// Add the setting configuration to the CSV file
	AppendSettingConfig(CSVContent, SettingLabels, SettingValues);

	WriteFile(FilePath, CSVContent);
}
void UEvacuationLogger::WriteNavigationHistory(const FString& FilePath, const TArray<FNavigationHistoryT>& NavigationData)
{
	FString CSVContent;

	// Header
	CSVContent += TEXT("ParticipantIndex,Time,CurrentWaypoint,UpdatedRoute\n");

	for (const FNavigationHistoryT& NavHistory : NavigationData)
	{
		for (const FRouteSnapshotT& Snapshot : NavHistory.RouteSnapshots)
		{
			// Format each row
			FString Row = NavHistory.ParticipantIndex + TEXT(",");
			Row += FString::SanitizeFloat(Snapshot.Time) + TEXT(",");
			Row += FString::FromInt(Snapshot.CurrentWaypoint) + TEXT(",");
            
			// Convert UpdatedRoute array to string
			for (int RoutePoint : Snapshot.UpdatedRoute)
			{
				Row += FString::FromInt(RoutePoint) + TEXT(" ");
			}
			FString _ = Row.TrimEnd();

			// Add row to CSV content
			CSVContent += Row + TEXT("\n");
		}
	}

	// Write to file
	WriteFile(FilePath, CSVContent);
}

void UEvacuationLogger::AppendParticipantIndices(FString& CSVContent, const TArray<FString>& PlayerNames)
{
	// Create a FString in the format of "Names, Players..." to be written to the CSV file
	CSVContent += TEXT("Names,") + ConvertToCSVFormat(PlayerNames) + TEXT("\n");
}

void UEvacuationLogger::AppendSettingConfig(FString& CSVContent, const TArray<FString>& SettingLabels, const TArray<FString>& SettingValues)
{
	// Create a FString in the format of "SettingLabels, SettingValues..." to be written to the CSV file
	CSVContent += TEXT("SettingLabels,") + ConvertToCSVFormat(SettingLabels) + TEXT("\n");
	CSVContent += TEXT("SettingValues,") + ConvertToCSVFormat(SettingValues) + TEXT("\n");
}
FString UEvacuationLogger::ConvertToCSVFormat(const TArray<FString>& StringArray)
{
	FString Result;
	for (const FString& Value : StringArray)
	{
		Result += Value + TEXT(",");
	}
	Result.RemoveFromEnd(TEXT(","));
	return Result;
}

void UEvacuationLogger::WriteFile(const FString& FilePath, const FString& CSVContent)
{
	// Add the relative path to FilePath
	const FString AbsoluteFilePath = FPaths::ProjectDir() + FilePath;

	// If File already exists, delete it
	if (FPaths::FileExists(AbsoluteFilePath))
	{
		IFileManager::Get().Delete(*AbsoluteFilePath);
	}
	
	FFileHelper::SaveStringToFile(CSVContent, *AbsoluteFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
}

