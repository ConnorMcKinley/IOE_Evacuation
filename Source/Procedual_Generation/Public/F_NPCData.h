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