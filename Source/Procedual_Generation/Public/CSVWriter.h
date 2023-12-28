// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CSVWriter.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PROCEDUAL_GENERATION_API UCSVWriter : public UActorComponent
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintCallable, Category = "CSV")
	static void WriteExperimentalSetupDetails(const FString& FilePath, const TArray<FString>& PlayerNames,
	                                          const TArray<FString>& SettingLabels, const TArray<FString>& SettingValues);

private:
	static void AppendParticipantIndices(FString& CSVContent, const TArray<FString>& PlayerNames);
	static void AppendSettingConfig(FString& CSVContent, const TArray<FString>& SettingLabels, TArray<FString>& SettingValues);
	
    static FString ConvertToCSVFormat(const TArray<FString>& StringArray);
};
