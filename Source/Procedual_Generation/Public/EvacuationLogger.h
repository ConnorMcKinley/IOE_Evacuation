// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EvacuationLogger.generated.h"


USTRUCT(BlueprintType)
struct FRouteSnapshotT
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	float Time;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	int CurrentWaypoint;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	TArray<int> UpdatedRoute;

	FRouteSnapshotT()
		: Time(0.0f), CurrentWaypoint(0)
	{
	}

	FRouteSnapshotT(float InTime, int InCurrentWaypoint, TArray<int> InUpdatedRoute)
		: Time(InTime), CurrentWaypoint(InCurrentWaypoint), UpdatedRoute(InUpdatedRoute)
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

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROCEDUAL_GENERATION_API UEvacuationLogger : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEvacuationLogger();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable, Category = "CSV")
	static void WriteExperimentalSetupDetails(const FString& FilePath, const TArray<FString>& PlayerNames,
											  const TArray<FString>& SettingLabels, const TArray<FString>& SettingValues);
	UFUNCTION(BlueprintCallable, Category = "CSV")
	static void WriteNavigationHistory(const FString& FilePath, const TArray<FNavigationHistoryT>& NavigationData);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	static void AppendParticipantIndices(FString& CSVContent, const TArray<FString>& PlayerNames);
	static void AppendSettingConfig(FString& CSVContent, const TArray<FString>& SettingLabels, const TArray<FString>& SettingValues);
	static FString ConvertToCSVFormat(const TArray<FString>& StringArray);
	static void WriteFile(const FString& FilePath, const FString& CSVContent);
		
};
