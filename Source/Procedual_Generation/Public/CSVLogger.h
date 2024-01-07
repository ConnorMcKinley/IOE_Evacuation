// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CSVLogger.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROCEDUAL_GENERATION_API UCSVLogger : public UActorComponent
{
	GENERATED_BODY()

public:	

protected:
	UFUNCTION(BlueprintCallable, Category = "CSV")
	static void WriteExperimentalSetupDetails(const FString& FilePath, const TArray<FString>& PlayerNames,
											  const TArray<FString>& SettingLabels, const TArray<FString>& SettingValues);

private:
	static void AppendParticipantIndices(FString& CSVContent, const TArray<FString>& PlayerNames);
	static void AppendSettingConfig(FString& CSVContent, const TArray<FString>& SettingLabels, TArray<FString>& SettingValues);
	
	static FString ConvertToCSVFormat(const TArray<FString>& StringArray);
		
};
