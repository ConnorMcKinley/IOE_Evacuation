// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Misc/DefaultValueHelper.h"
#include "RandomReportValidation.generated.h"

/**
 * 
 */
UCLASS()
class PROCEDUAL_GENERATION_API URandomReportValidation : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category="Utilities|Validation")
	static bool URandomReportValidateMinMax(int MinValue, const FString& MaxValue, FString& OutErrorMessage);
	
	UFUNCTION(BlueprintCallable, Category="Utilities|Validation")
	static bool UTimeLagValidateMinMax(float MinValue, float MaxValue, FString& OutErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "Utilities|Validation")
	static bool UPenaltyTimeValidate(float PenaltyValue, FString& OutErrorMessage);
};
