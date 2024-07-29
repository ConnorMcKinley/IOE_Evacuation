// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "BP_BoundaryBox.h"
#include "BP_NPC.h"
#include "FRouteData.generated.h"

/**
 * Stores All References to Instances for NPC Spawning. 
 * Example: One Connection Road will have 2 Boundary Boxes and 10 NPCs. A Map/Array with this Data Structure will hold all references to these Actors/Characters
 */
USTRUCT(BlueprintType)
struct FRouteData
{
    GENERATED_BODY()

    // Blueprint instances or references for visual representations
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<ABP_BoundaryBox*> BoxInstances;

    // NPCs instances
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<ABP_NPC*> NPCInstances;
};