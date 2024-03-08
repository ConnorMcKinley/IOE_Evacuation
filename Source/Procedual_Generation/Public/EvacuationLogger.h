// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EvacuationLogger.generated.h"

UENUM(BlueprintType)
enum EReasonForSnapshot
{
	Initial,
	WaypointReached,
	RoadblockReport,
	Finished
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
		: Time(0.0f), CurrentWaypoint(0), Reason(EReasonForSnapshot::Initial)
	{
	}

	FRouteSnapshotT(float InTime, int InCurrentWaypoint, TArray<int> InUpdatedRoute, EReasonForSnapshot InReason)
		: Time(InTime), CurrentWaypoint(InCurrentWaypoint), UpdatedRoute(InUpdatedRoute), Reason(InReason)
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

	// The waypoints that this roadblock is in between
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	int NodeA;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	int NodeB;
};

UENUM(BlueprintType)
enum EReasonForReport
{
	Default
};
USTRUCT(BlueprintType)
struct FReportData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	FString ParticipantIndex;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	int RoadblockIndex;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	FDateTime TimeOpened;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	FDateTime TimeClosed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	FDateTime TimeSent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	bool WasReportSent;

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
	int CurrentWaypoint;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	int ScoreGuide;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	int ScoreOtherPlayer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	FDateTime TimeAppeared;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	FDateTime TimeSent;
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
	void WriteExperimentalSetupDetails(const FString& FilePath, const TArray<FString>& PlayerNames,
											  const TArray<FString>& SettingLabels, const TArray<FString>& SettingValues);
	UFUNCTION(BlueprintCallable, Category = "CSV")
	void WriteNavigationHistory(const FString& FilePath, const TArray<FNavigationHistoryT>& NavigationData);

	UFUNCTION(BlueprintCallable, Category = "CSV")
	void WriteReportHistory(const FString& FilePath, const TArray<FRoadBlockData>& RoadBlockData, const TArray<FReportData>& ReportData);

	UFUNCTION(BlueprintCallable, Category = "CSV")
	void WriteTrustHistory(const FString& FilePath, const TArray<FTrustData>& TrustData);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	static void AppendParticipantIndices(FString& CSVContent, const TArray<FString>& PlayerNames);
	static void AppendSettingConfig(FString& CSVContent, const TArray<FString>& SettingLabels, const TArray<FString>& SettingValues);
	static FString ConvertToCSVFormat(const TArray<FString>& StringArray);
	void WriteFile(const FString& FilePath, const FString& CSVContent) const;
	// Function to parse a FDateTime into a string in the format HH:MM:SS:MS
	static FString ParseDateTimeIntoHmsms(FDateTime Time);
		
};
