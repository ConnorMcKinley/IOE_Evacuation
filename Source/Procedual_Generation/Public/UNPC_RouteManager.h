// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFileManager.h"
#include "UObject/NoExportTypes.h"
#include "Misc/DefaultValueHelper.h"
#include "FS_Map_Route.h"
#include "FRouteData.h"
#include "UNPC_RouteManager.generated.h"


/**
 * Utility class for NPC Route Management with static functions.
 */
UCLASS(Blueprintable, BlueprintType)
class PROCEDUAL_GENERATION_API UNPC_RouteManager : public UObject
{
    GENERATED_BODY()

public:
    UNPC_RouteManager();

    UFUNCTION(BlueprintCallable, Category = "NPC Route Management", meta = (WorldContext = "WorldContextObject"))
    static bool HasNPCSpawnedAtRoute(const UObject* WorldContextObject, int32 RouteID);

    UFUNCTION(BlueprintCallable, Category = "NPC Route Management", meta = (WorldContext = "WorldContextObject"))
    static void AddNPCSpawnedAtRoute(const UObject* WorldContextObject, int32 RouteID, FRouteData NPC_Data);

    UFUNCTION(BlueprintCallable, Category = "NPC Route Management", meta = (WorldContext = "WorldContextObject"))
    static void RemoveNPCSpawnedAtRoute(const UObject* WorldContextObject, int32 RouteID);

    UFUNCTION(BlueprintCallable, Category = "NPC File System", meta = (WorldContext = "WorldContextObject"))
    static bool IsValidJsonFile(const UObject* WorldContextObject, FString& Err);

    UFUNCTION(BlueprintCallable, Category = "NPC File System", meta = (WorldContext = "WorldContextObject"))
    static bool ReadAndInitNPCNodeMap(const UObject* WorldContextObject, int32 DefaultNPCValue, const TArray<FS_Map_Route> PermanentRoutes);

    UFUNCTION(BlueprintCallable, Category = "NPC Route Management", meta = (WorldContext = "WorldContextObject"))
    static int32 GetRouteNumNPCs(int32 route_id);

    UFUNCTION(BlueprintCallable, Category = "NPC Route Management")
    static void ResetStaticMaps();
    
private:
    // Using a static map to hold route states; static to make it common across all instances.
    static TMap<int32, int32> NumberOfNPCsAtRouteMap;
    static TMap<FRouteKey, int32> MapConnectionToRouteID;
    static TMap<int32, FRouteData> MapRouteIDtoInstances;
};