// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EvacuationLogger.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	static void AppendParticipantIndices(FString& CSVContent, const TArray<FString>& PlayerNames);
	static void AppendSettingConfig(FString& CSVContent, const TArray<FString>& SettingLabels, TArray<FString>& SettingValues);
	
	static FString ConvertToCSVFormat(const TArray<FString>& StringArray);
		
};
