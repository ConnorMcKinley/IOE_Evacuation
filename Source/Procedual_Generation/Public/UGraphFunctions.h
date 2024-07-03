// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UGraphFunctions.generated.h"

USTRUCT(BlueprintType)
struct FNodeConnection
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node Connection")
    int32 NodeIndex1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node Connection")
    int32 NodeIndex2;
};

USTRUCT(BlueprintType)
struct FNodeConnections
{
    GENERATED_BODY()

    // This will hold unique indices of connected nodes
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node Connections")
    TArray<int32> ConnectedNodes;
};
/**
 * 
 */
UCLASS()
class PROCEDUAL_GENERATION_API UUGraphFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category="Graph Functions")
	static void InitializeNodeConnections(const TArray<FNodeConnection>& Connections, TMap<int32, FNodeConnections>& OutNodeConnections);

    UFUNCTION(BlueprintCallable, Category="Graph Functions")
    static void AddNodeConnection(const FNodeConnection& Connection, TMap<int32, FNodeConnections>& OutNodeConnections);

    UFUNCTION(BlueprintPure, Category = "Graph Functions")
    static TMap<int32, FNodeConnections>& GetNodeToRoutes();
private:
    static inline TMap<int32, FNodeConnections> ump_NodeToRoutes;
};
