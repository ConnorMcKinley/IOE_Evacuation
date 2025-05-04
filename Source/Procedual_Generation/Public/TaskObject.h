// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExperimentHUDWidget.h"
#include "EvacuationLogger.h"
#include "UObject/NoExportTypes.h"
#include "TaskObject.generated.h"


// Broadcast an event to all bound liste
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTaskCompletedSignature);

/**
 *
 */
UCLASS(BlueprintType)
class PROCEDUAL_GENERATION_API UTaskObject : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FTaskCompletedSignature OnTaskCompleted;

    // Constructor
    UTaskObject();

    UFUNCTION(BlueprintCallable)
    void Init(UExperimentHUDWidget* Widget, float DelaySeconds, int RouteIndex, EReasonForReport Enum);

    UFUNCTION(BlueprintCallable)
    void StartTimer();

protected:
    float DelayDuration;
    int RouteIndex_;
    EReasonForReport Enum_;

    UFUNCTION()
    void DelayedFunction();
private:
    TWeakObjectPtr<UExperimentHUDWidget> HUDWidget;
};
