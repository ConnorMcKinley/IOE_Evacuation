// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapImporter.generated.h"

// Structure to represent a node
USTRUCT(BlueprintType)
struct FGraphNode {
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	float X;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	float Y;

	FGraphNode(float InX = 0.0f, float InY = 0.0f) : X(InX), Y(InY) {}
};

// Structure to represent a connection between two nodes
USTRUCT(BlueprintType)
struct FGraphConnection {
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	int32 NodeIndex1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Evacuation Data")
	int32 NodeIndex2;

	FGraphConnection(int32 InNodeIndex1 = 0, int32 InNodeIndex2 = 0)
		: NodeIndex1(InNodeIndex1), NodeIndex2(InNodeIndex2) {}
};

UCLASS(BlueprintType)
// Class to handle reading and storing the graph data
class PROCEDUAL_GENERATION_API UMapImporter : public UObject
{
	GENERATED_BODY()

public:
	// Function to read and parse the graph file
	// @param FilePath: The path to the file to be read
	UFUNCTION(BlueprintCallable)
	static bool ReadGraphFromFile(const FString& FilePath, TArray<FGraphNode>& OutNodes, TArray<FGraphConnection>& OutConnections);
};
