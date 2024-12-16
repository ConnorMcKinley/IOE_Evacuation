#pragma once

#include "CoreMinimal.h"
#include "FS_Map_Route.h"
#include "F_NPCData.generated.h"


/**
 * Struct to manage NPC Data
 */
USTRUCT(BlueprintType)
struct F_NPCData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    int32 RouteID;

    UPROPERTY(BlueprintReadWrite)
    int32 NumberOfNPCs;

    UPROPERTY(BlueprintReadWrite)
    bool bHasNPCsSpawned;
};

USTRUCT(BlueprintType)
struct F_ReplicatedRouteConnection
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FRouteKey RouteKey;

    UPROPERTY(BlueprintReadWrite)
    int32 RouteID;
};

USTRUCT(BlueprintType)
struct F_NPCWeightData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 NPCCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Weight;

    F_NPCWeightData() : NPCCount(0), Weight(0.0f) {}
    F_NPCWeightData(int32 InNPCCount, float InWeight) : NPCCount(InNPCCount), Weight(InWeight) {}
};