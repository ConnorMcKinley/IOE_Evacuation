// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EvacuationLogger.generated.h"

UENUM(BlueprintType)
enum EReasonForSnapshot
{
	Initial,
	NewWaypointReached,
	ReturnedFromRoadblock,
	Detour,
	RoadblockReport,
	Finished,
	ServerOpen,
	PlayerJoin
};

USTRUCT(BlueprintType)
struct FRouteSnapshotT
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	FDateTime Time; 

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	int CurrentWaypoint;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	TArray<int> UpdatedRoute;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	TEnumAsByte<EReasonForSnapshot> Reason;

	FRouteSnapshotT()
		: Time(FDateTime::Now()), CurrentWaypoint(0), Reason(EReasonForSnapshot::Initial)
	{
	}

	FRouteSnapshotT(float InTime, int InCurrentWaypoint, TArray<int> InUpdatedRoute, EReasonForSnapshot InReason)
		: Time(FDateTime::Now()), CurrentWaypoint(InCurrentWaypoint), UpdatedRoute(InUpdatedRoute), Reason(InReason)
	{
	}
};

USTRUCT(BlueprintType)
struct FNavigationHistoryT
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	FString ParticipantIndex;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	TArray<FRouteSnapshotT> RouteSnapshots;

	FNavigationHistoryT()
	{
	}

	FNavigationHistoryT(FString InParticipantIndex, TArray<FRouteSnapshotT> InRouteSnapshots)
		: ParticipantIndex(InParticipantIndex), RouteSnapshots(InRouteSnapshots)
	{
	}
};

USTRUCT(BlueprintType)
struct FRoadBlockData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	int NodeA = -1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	int NodeB = -1;
};

UENUM(BlueprintType)
enum EReasonForReport
{
	Default,
	ExperimenterReachedWaypoint,
	ExperimenterRandom,
	InvalidRoute,
	AlreadySubmitted
};

USTRUCT(BlueprintType)
struct FReportData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	FString ParticipantIndex;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	int RoadblockIndex = -1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	FDateTime TimeOpened;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	FDateTime TimeClosed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	FDateTime TimeSent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	bool WasReportSent = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	TEnumAsByte<EReasonForReport> Reason;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	FString ReportMessage;
};

USTRUCT(BlueprintType)
struct FTrustData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	FString ParticipantIndex;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	int CurrentWaypoint = -1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	int ScoreGuide = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	int ScoreOtherPlayer = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	int ScoreOverall = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	FDateTime TimeAppeared;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	FDateTime TimeSent;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Evacuation Data")
	TEnumAsByte<EReasonForSnapshot> Reason;
};

USTRUCT(BlueprintType)
struct FPositionData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	FString ParticipantIndex;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	FDateTime Time;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	FVector Position = FVector::ZeroVector;
};

USTRUCT(BlueprintType)
struct FDecisionData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	TArray<bool> IsNextNode;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	TArray<bool> IsReportCorrectIfNextAndRoadblock;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	TArray<bool> IsReportCorrectIfAdjacentAndRoadblock;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	TArray<bool> IsReportWrongIfNextAndNoRoadblock;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	TArray<bool> IsReportWrongIfAdjacentAndNoRoadblock;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	TArray<bool> IsRandomReportCorrect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	TMap<FString, int> PlayersNextNodeIndices;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	TMap<FString, int> IsReportCorrectIfNextAndRoadblockPlayerIndices;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	TMap<FString, int> IsReportCorrectIfAdjacentAndRoadblockPlayerIndices;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	TMap<FString, int> IsReportWrongIfNextAndNoRoadblockPlayerIndices;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	TMap<FString, int> IsReportWrongIfAdjacentAndNoRoadblockPlayerIndices;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	int IsRandomReportCorrectPlayerIndex = -1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	TMap<FString, int> PlayersLastWaypointID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	TArray<float> RandomTimeSequence;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Evacuation Data")
	int RandomTimeSequenceIndex = -1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Evacuation Data")
	TMap<FString, int> RandomTimeLagPlayerIndices;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Evacuation Data")
	TArray<float> RandomTimeLag;
};

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROCEDUAL_GENERATION_API UEvacuationLogger : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEvacuationLogger();

	// The time when the experiment started, exposed to the blueprint
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	FDateTime ExperimentStartTime;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable, Category = "CSV")
	void WriteExperimentalSetupDetails(const FString& FilePath, const FString& LevelName, const TArray<FString>& PlayerNames,
	                                   const TArray<FString>& GameSettingLabels, const TArray<FString>& GameSettingValues, const TArray<FString>&
	                                   ExperimenterSettingLabels, const TArray<FString>& ExperimenterSettingValues, const TArray<FString>& NPCSettingLabels, const TArray<FString>& NPCSettingValues);
	UFUNCTION(BlueprintCallable, Category = "CSV")
	void WriteNavigationHistory(const FString& FilePath, const TArray<FNavigationHistoryT>& NavigationData);

	UFUNCTION(BlueprintCallable, Category = "CSV")
	void WriteReportHistory(const FString& FilePath, const TArray<FRoadBlockData>& RoadBlockData, const TArray<FReportData>& ReportData);

	UFUNCTION(BlueprintCallable, Category = "CSV")
	void WriteTrustHistory(const FString& FilePath, const TArray<FTrustData>& TrustData);

	UFUNCTION(BlueprintCallable, Category = "CSV")
	void WritePositionHistory(const FString& FilePath, const TArray<FPositionData>& PositionData);

	UFUNCTION(BlueprintCallable, Category = "CSV")
	void WriteDecisionHistory(const FString& FilePath, const TArray<FString>& DecisionLabels, const FDecisionData& DecisionData);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	static void AppendLevelName(FString& CSVContent, const FString& LevelName);
	static void AppendParticipantIndices(FString& CSVContent, const TArray<FString>& PlayerNames);
	static void AppendSettingConfig(FString& CSVContent, const TArray<FString>& SettingLabels, const TArray<FString>& SettingValues);
	static FString ConvertToCSVFormat(const TArray<FString>& StringArray);
	void WriteFile(const FString& FilePath, const FString& CSVContent) const;
	// Function to parse a FDateTime into a string in the format HH:MM:SS:MS
	static FString ParseDateTimeIntoHmsms(FDateTime Time);
		
};
