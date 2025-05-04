// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFileManager.h"
#include "UObject/NoExportTypes.h"
#include "Misc/DefaultValueHelper.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "FRouteData.h"
#include "FS_Map_Route.h"
#include "F_NPCData.h"

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

    UFUNCTION(BlueprintCallable, Category = "NPC File System", meta = (WorldContext = "WorldContextObject"))
    bool IsValidJsonFile(const UObject* WorldContextObject, FString& Err, const FString& Path, const FString& mapPath);

    UFUNCTION(BlueprintCallable, Category = "NPC Route Management")
    static UNPC_RouteManager* CreateNPC_RouteManager(UObject* Outer);

    UFUNCTION(BlueprintCallable, Category = "NPC File System", meta = (WorldContext = "WorldContextObject"))
    bool ReadAndInitNPCNodeMap(const UObject* WorldContextObject, int32 DefaultNPCValue, const TArray<FS_Map_Route> PermanentRoutes, const FString& path);

    UFUNCTION(BlueprintCallable, Category = "NPC Route Management", meta = (WorldContext = "WorldContextObject"))
    bool LocalHasNPCSpawnedAtRoute(int32 RouteID) const;

    UFUNCTION(BlueprintCallable, Category = "NPC Route Management", meta = (WorldContext = "WorldContextObject"))
    int32 LocalGetRouteNumNPCs(int32 RouteID) const;


    UFUNCTION(BlueprintCallable, Category = "NPC Route Management", meta = (WorldContext = "WorldContextObject"))
    void ServerAddNPCSpawnedAtRoute(const UObject* WorldContextObject, int32 RouteID, FRouteData NPC_Data);

    UFUNCTION(BlueprintCallable, Category = "NPC Route Management", meta = (WorldContext = "WorldContextObject"))
    void ServerRemoveNPCSpawnedAtRoute(const UObject* WorldContextObject, int32 RouteID);
    void LogCurrentRouteIDs();


    UFUNCTION(BlueprintCallable, Category = "NPC Route Management", meta = (WorldContext = "WorldContextObject"))
    float LocalGetWeightByNPCCount(int32 NPCCount);
protected:

    /// <summary>
    /// Record properties of object class that you want to replicate.
    /// </summary>
    /// <param name="OutLifetimeProps"></param>
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    /// <summary>
    /// Informs the replication system that references to this object class are supported for replication
    /// </summary>
    /// <returns></returns>
    virtual bool IsSupportedForNetworking() const override { return true; }

private:
    // Server-sided data
    TMap<int32, int32> NumberOfNPCsAtRouteMap;
    TMap<FRouteKey, int32> MapConnectionToRouteID;
    TMap <int32, FRouteData> MapRouteIDtoInstances;

    bool CheckAndValidateNPCWeight(const TSharedPtr<FJsonObject>& NPCJsonObject, FString& Err);

    // Client-sided data
    UPROPERTY(Replicated)
    TArray<F_NPCWeightData> NPCWeightArray;

    UPROPERTY(Replicated)
    TArray<F_NPCData> NPCDataArray;

    UPROPERTY(Replicated)
    TArray<F_ReplicatedRouteConnection> RouteConnectionArray;

    void UpdateNPCDataArray(int32 RouteID, bool bHasNPCSpawned);
};