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
                                                      const TArray<FString>& GameSettingLabels, const TArray<FString>& GameSettingValues, const TArray<FString>&
                                                      ExperimenterSettingLabels, const TArray<FString>& ExperimenterSettingValues)
{
	// Create a FString to be written to the CSV file
	FString CSVContent;

	AppendParticipantIndices(CSVContent, PlayerNames);

	// Extra line to separate the two sections
	CSVContent += TEXT("\n");

	// Add game configuration title
	CSVContent += TEXT("Game Configuration\n");

	// Add the game configuration to the CSV file
	AppendSettingConfig(CSVContent, GameSettingLabels, GameSettingValues);

	// Extra line to separate the two sections
	CSVContent += TEXT("\n");

	// Add experimenter configuration title
	CSVContent += TEXT("Experimenter Configuration\n");

	// Add the experimenter configuration to the CSV file
	AppendSettingConfig(CSVContent, ExperimenterSettingLabels, ExperimenterSettingValues);

	WriteFile(FilePath, CSVContent);
}
TMap<int, FString> NavReasonToString = {
	{0, "Initial"},
	{1, "Waypoint Reached"},
	{2, "Roadblock Report"},
	{3, "Finished"}
};
void UEvacuationLogger::WriteNavigationHistory(const FString& FilePath, const TArray<FNavigationHistoryT>& NavigationData)
{
	FString CSVContent;

	// Header
	CSVContent += TEXT("ParticipantIndex,Time,Reason,CurrentWaypoint,UpdatedRoute\n");

	for (const FNavigationHistoryT& NavHistory : NavigationData)
	{
		for (const FRouteSnapshotT& Snapshot : NavHistory.RouteSnapshots)
		{
			// Format each row
			FString Row = NavHistory.ParticipantIndex + TEXT(",");
			Row += ParseDateTimeIntoHmsms(Snapshot.Time) + TEXT(",");
			Row += NavReasonToString[Snapshot.Reason] + TEXT(",");
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
TMap<int, FString> ReportReasonToString = {
	{0, "Default"},
	{1, "Experimenter_ReachedWaypoint"},
	{2, "Experimenter_Random"}
};
void UEvacuationLogger::WriteReportHistory(const FString& FilePath, const TArray<FRoadBlockData>& RoadBlockData,
	const TArray<FReportData>& ReportData)
{
	FString CSVContent;

	// Header
	CSVContent += TEXT("Roadblock Index, Location1, Location2\n");

	for (int i = 0; i < RoadBlockData.Num(); ++i)
	{
		FString Row = FString::FromInt(i) + TEXT(",");
		Row += FString::FromInt(RoadBlockData[i].NodeA) + TEXT(",");
		Row += FString::FromInt(RoadBlockData[i].NodeB);

		CSVContent += Row + TEXT("\n");
	}

	CSVContent += TEXT("\n");

	CSVContent += TEXT("Reports\n");

	CSVContent += TEXT("Participant,Roadblock Index,Time Opened,Time Closed,Time Sent,Was Report Sent?,Reason,Report Message\n");

	for (auto Report : ReportData)
	{
		FString Row = Report.ParticipantIndex + TEXT(",");
		Row += (Report.RoadblockIndex != -1 ? FString::FromInt(Report.RoadblockIndex) : TEXT("")) + TEXT(",");
		Row += ParseDateTimeIntoHmsms(Report.TimeOpened) + TEXT(",");
		Row += ParseDateTimeIntoHmsms(Report.TimeClosed) + TEXT(",");
		Row += Report.WasReportSent ? ParseDateTimeIntoHmsms(Report.TimeSent) + TEXT(",") : TEXT(",");
		Row += Report.WasReportSent ? TEXT("Yes,") : TEXT("No,");
		Row += ReportReasonToString[Report.Reason] + TEXT(",");
		Row += Report.ReportMessage;

		CSVContent += Row + TEXT("\n");
	}

	// Write to file
	WriteFile(FilePath, CSVContent);
}

void UEvacuationLogger::WriteTrustHistory(const FString& FilePath, const TArray<FTrustData>& TrustData)
{
	FString CSVContent;

	// Header
	CSVContent += TEXT("Participant,Current Waypoint,Guide Score,Other Player Score,Overall Score,Time Appeared,Time Sent\n");

	for (auto Trust : TrustData)
	{
		FString Row = Trust.ParticipantIndex + TEXT(",");
		Row += FString::FromInt(Trust.CurrentWaypoint) + TEXT(",");
		Row += FString::FromInt(Trust.ScoreGuide) + TEXT(",");
		Row += FString::FromInt(Trust.ScoreOtherPlayer) + TEXT(",");
		Row += FString::FromInt(Trust.ScoreOverall) + TEXT(",");
		Row += ParseDateTimeIntoHmsms(Trust.TimeAppeared) + TEXT(",");
		Row += ParseDateTimeIntoHmsms(Trust.TimeSent);

		CSVContent += Row + TEXT("\n");
	}

	WriteFile(FilePath, CSVContent);
}

void UEvacuationLogger::WritePositionHistory(const FString& FilePath, const TArray<FPositionData>& PositionData)
{
	FString CSVContent;

	// Header
	CSVContent += TEXT("Participant,Time,Position X,Position Y\n");

	for (auto PositionDatum : PositionData)
	{
		FString Row = PositionDatum.ParticipantIndex + TEXT(",");
		Row += ParseDateTimeIntoHmsms(PositionDatum.Time) + TEXT(",");
		Row += FString::FromInt(PositionDatum.Position.X) + TEXT(",");
		Row += FString::FromInt(PositionDatum.Position.Y);

		CSVContent += Row + TEXT("\n");
	}

	WriteFile(FilePath, CSVContent);
}

void UEvacuationLogger::WriteDecisionHistory(const FString& FilePath, const TArray<FString>& DecisionLabels,
                                             const FDecisionData& DecisionData)
{
	FString CSVContent;

	// Create header row
	for (const FString& Label : DecisionLabels)
	{
		CSVContent += Label;
		CSVContent += TEXT(",");
	}
	// Remove the trailing comma and add a new line
	CSVContent.RemoveAt(CSVContent.Len() - 1);
	CSVContent += TEXT("\n");

	// Determine the number of rows to write based on the length of the arrays (assumed to be equal length)
	int32 NumRows = DecisionData.IsNextNode.Num();

	// Add rows of data
	for (int32 i = 0; i < NumRows; ++i)
	{
		CSVContent += (DecisionData.IsNextNode.IsValidIndex(i) ? (DecisionData.IsNextNode[i] ? TEXT("true") : TEXT("false")) : TEXT(""));
		CSVContent += TEXT(",");

		CSVContent += (DecisionData.IsReportCorrectIfNextAndRoadblock.IsValidIndex(i) ? (DecisionData.IsReportCorrectIfNextAndRoadblock[i] ? TEXT("true") : TEXT("false")) : TEXT(""));
		CSVContent += TEXT(",");

		CSVContent += (DecisionData.IsReportCorrectIfAdjacentAndRoadblock.IsValidIndex(i) ? (DecisionData.IsReportCorrectIfAdjacentAndRoadblock[i] ? TEXT("true") : TEXT("false")) : TEXT(""));
		CSVContent += TEXT(",");

		CSVContent += (DecisionData.IsReportWrongIfNextAndNoRoadblock.IsValidIndex(i) ? (DecisionData.IsReportWrongIfNextAndNoRoadblock[i] ? TEXT("true") : TEXT("false")) : TEXT(""));
		CSVContent += TEXT(",");

		CSVContent += (DecisionData.IsReportWrongIfAdjacentAndNoRoadblock.IsValidIndex(i) ? (DecisionData.IsReportWrongIfAdjacentAndNoRoadblock[i] ? TEXT("true") : TEXT("false")) : TEXT(""));
		CSVContent += TEXT(",");

		CSVContent += (DecisionData.RandomTimeSequence.IsValidIndex(i) ? FString::Printf(TEXT("%f"), DecisionData.RandomTimeSequence[i]) : TEXT(""));
		CSVContent += TEXT(",");
		
		CSVContent += (DecisionData.IsRandomReportCorrect.IsValidIndex(i) ? (DecisionData.IsRandomReportCorrect[i] ? TEXT("true") : TEXT("false")) : TEXT(""));


		// Add a new line at the end of each row
		CSVContent += TEXT("\n");
	}

	// Write the content to the file
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

void UEvacuationLogger::WriteFile(const FString& FilePath, const FString& CSVContent) const
{
	// Add the relative path to FilePath and add the experiment start time
	const FString AbsoluteFilePath = FPaths::ProjectDir() + "/" + ExperimentStartTime.ToString() + "/" + FilePath +
		TEXT(".csv");

	// If File already exists, delete it
	if (FPaths::FileExists(AbsoluteFilePath))
	{
		IFileManager::Get().Delete(*AbsoluteFilePath);
	}
	
	FFileHelper::SaveStringToFile(CSVContent, *AbsoluteFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
}

FString UEvacuationLogger::ParseDateTimeIntoHmsms(FDateTime Time)
{
	return FString::Printf(TEXT("%02d:%02d:%02d.%03d"), Time.GetHour(), Time.GetMinute(), Time.GetSecond(), Time.GetMillisecond());
}