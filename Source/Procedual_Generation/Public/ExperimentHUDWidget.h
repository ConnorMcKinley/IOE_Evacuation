// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EvacuationLogger.h"
#include "Blueprint/UserWidget.h"
#include "ExperimentHUDWidget.generated.h"


/**
 * 
 */
UCLASS()
class PROCEDUAL_GENERATION_API UExperimentHUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    // This is the function that Blueprints can override to implement their own functionality
    UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
    void MakeReport(int RouteIndex, EReasonForReport ReportReason, float DelayDuration);
};
