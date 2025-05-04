// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "FS_Map_Route.generated.h"

USTRUCT(BlueprintType)
struct FRouteKey
{
    GENERATED_BODY()
    UPROPERTY()
    int32 NodeA;

    UPROPERTY()
    int32 NodeB;

    FRouteKey() : NodeA(0), NodeB(0) {}

    FRouteKey(int32 InNodeA, int32 InNodeB)
        : NodeA(InNodeA), NodeB(InNodeB)
    {
    }

    bool operator==(const FRouteKey& Other) const
    {
        return NodeA == Other.NodeA && NodeB == Other.NodeB;
    }

    friend uint32 GetTypeHash(const FRouteKey& Key)
    {
        return HashCombine(GetTypeHash(Key.NodeA), GetTypeHash(Key.NodeB));
    }
};

USTRUCT(BlueprintType)
struct FS_Map_Route
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "node_start", MakeStructureDefaultValue = "0"))
    int32 node_start;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "node_end", MakeStructureDefaultValue = "0"))
    int32 node_end;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "route_id", MakeStructureDefaultValue = "0"))
    int32 route_id;

    FS_Map_Route()
        : node_start(0), node_end(0), route_id(0)
    {}
};