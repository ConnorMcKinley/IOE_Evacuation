// Fill out your copyright notice in the Description page of Project Settings.


#include "MapImporter.h"

#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

bool UMapImporter::ReadGraphFromFile(const FString& FilePath, TArray<FGraphNode>& OutNodes, TArray<FGraphConnection>& OutConnections)
{
    const FString AbsoluteFilePath = FPaths::ProjectDir() + FilePath;
    
    FString FileContent;
    if (!FFileHelper::LoadFileToString(FileContent, *AbsoluteFilePath))
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load file: %s"), *AbsoluteFilePath);
        return false;
    }

    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(FileContent);
    if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to parse JSON file: %s"), *AbsoluteFilePath);
        return false;
    }

    // Parse nodes
    TArray<TSharedPtr<FJsonValue>> NodesArray = JsonObject->GetArrayField(TEXT("nodes"));
    for (TSharedPtr<FJsonValue> Value : NodesArray)
    {
        TSharedPtr<FJsonObject> NodeObject = Value->AsObject();
        float X = NodeObject->GetNumberField(TEXT("x"));
        float Y = NodeObject->GetNumberField(TEXT("y"));
        OutNodes.Add(FGraphNode(X, Y));
    }

    // Print the number of nodes
    UE_LOG(LogTemp, Warning, TEXT("Number of nodes: %d"), OutNodes.Num());

    // Parse connections
    TArray<TSharedPtr<FJsonValue>> ConnectionsArray = JsonObject->GetArrayField(TEXT("connections"));
    for (TSharedPtr<FJsonValue> Value : ConnectionsArray)
    {
        TSharedPtr<FJsonObject> ConnectionObject = Value->AsObject();
        int32 NodeIndex1 = ConnectionObject->GetIntegerField(TEXT("nodeIndex1"));
        int32 NodeIndex2 = ConnectionObject->GetIntegerField(TEXT("nodeIndex2"));
        OutConnections.Add(FGraphConnection(NodeIndex1, NodeIndex2));
    }

    return true;
}

bool UMapImporter::ReadStartEndPositionsFromFile(const FString& FilePath, TArray<FStartEndPosition>& OutStartEndPositions)
{
    const FString AbsoluteFilePath = FPaths::ProjectDir() + FilePath;

    FString FileContent;
    if (!FFileHelper::LoadFileToString(FileContent, *AbsoluteFilePath))
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load file: %s"), *AbsoluteFilePath);
        return false;
    }
    
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(FileContent);
    if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to parse JSON file: %s"), *AbsoluteFilePath);
        return false;
    }

    const TArray<TSharedPtr<FJsonValue>>* IndicesArray;
    if (!JsonObject->TryGetArrayField(TEXT("start_end_indices"), IndicesArray))
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to find 'start_end_indices' in JSON file: %s"), *AbsoluteFilePath);
        return false;
    }

    OutStartEndPositions.Empty();

    for (const TSharedPtr<FJsonValue>& Value : *IndicesArray)
    {
        const TArray<TSharedPtr<FJsonValue>>* Pair;
        if (Value->TryGetArray(Pair) && Pair->Num() == 2)
        {
            int32 StartIndex = (*Pair)[0]->AsNumber();
            int32 EndIndex = (*Pair)[1]->AsNumber();
            OutStartEndPositions.Add(FStartEndPosition(StartIndex, EndIndex));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Invalid format in 'start_end_indices', expected pairs of integers in JSON file: %s"), *AbsoluteFilePath);
            // Optionally, return false here if strict validation is required
        }
    }

    return true;
}

bool UMapImporter::ReadRoadblockPositionsFromFile(const FString& FilePath, TArray<int32>& OutRoadblockPositions)
{
    const FString AbsoluteFilePath = FPaths::ProjectDir() + FilePath;

    FString FileContent;
    if (!FFileHelper::LoadFileToString(FileContent, *AbsoluteFilePath))
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load file: %s"), *AbsoluteFilePath);
        return false;
    }

    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(FileContent);
    if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to parse JSON file: %s"), *AbsoluteFilePath);
        return false;
    }

    const TArray<TSharedPtr<FJsonValue>>* IndicesArray;
    if (!JsonObject->TryGetArrayField(TEXT("roadblock_indices"), IndicesArray))
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to find 'roadblock_indices' in JSON file: %s"), *AbsoluteFilePath);
        return false;
    }

    OutRoadblockPositions.Empty();

    for (const TSharedPtr<FJsonValue>& Value : *IndicesArray)
    {
        OutRoadblockPositions.Add(Value->AsNumber());
    }

    return true;
}

