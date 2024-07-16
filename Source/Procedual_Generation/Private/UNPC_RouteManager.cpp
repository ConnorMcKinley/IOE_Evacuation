#include "UNPC_RouteManager.h"

TMap<int32, bool> UNPC_RouteManager::RouteIDMap;
TMap<int32, bool> UNPC_RouteManager::SpawnNPCMap;

UNPC_RouteManager::UNPC_RouteManager()
{
    // Initialize default values here if necessary (NonStatic!)
}

bool UNPC_RouteManager::HasNPCSpawnedAtRoute(const UObject* WorldContextObject, int32 RouteID)
{
    UE_LOG(LogTemp, Log, TEXT("RouteIDMap Contents:"));
    for (const auto& Pair : RouteIDMap)
    {
        UE_LOG(LogTemp, Log, TEXT("Key: %d, Value: %s"), Pair.Key, Pair.Value ? TEXT("true") : TEXT("false"));
    }
    return RouteIDMap.Contains(RouteID) && RouteIDMap[RouteID];
}

void UNPC_RouteManager::AddNPCSpawnedAtRoute(const UObject* WorldContextObject, int32 RouteID)
{
    RouteIDMap.Add(RouteID, true);
}

void UNPC_RouteManager::RemoveNPCSpawnedAtRoute(const UObject* WorldContextObject, int32 RouteID)
{
    RouteIDMap.Remove(RouteID);
}

bool UNPC_RouteManager::IsValidJsonFile(const UObject* WorldContextObject, FString& Err)
{
    FString FilePath = FPaths::ProjectDir() / TEXT("NPC.json");
    if (FPaths::FileExists(FilePath)) {
        return true;
    }
    else {
        Err = "NPC.json Does Not Exist!";
    }
    return false;
}

bool UNPC_RouteManager::ReadAndInitNPCNodeMap(const UObject* WorldContextObject, int32 num_nodes)
{
    // initialize SpawnNPCMap
    for (int32 i = 0; i < num_nodes; i++) {
        SpawnNPCMap.Add(i, false);
    }
    FString FilePath = FPaths::ProjectDir() / TEXT("NPC.json");
    FString JsonString;
    if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load JSON file: %s"), *FilePath);
        return false;
    }

    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

    if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON file: %s"), *FilePath);
        return false;
    }
    // Read nodes array from JSON
    const TArray<TSharedPtr<FJsonValue>>* NodesArray;
    if (JsonObject->TryGetArrayField("nodes", NodesArray))
    {
        // Iterate over each node in the array
        for (int32 Index = 0; Index < NodesArray->Num(); Index++)
        {
            int32 Node = (*NodesArray)[Index]->AsNumber();
            // Mark node as NPC spawn point in map
            if (SpawnNPCMap.Contains(Node))
            {
                SpawnNPCMap[Node] = true;
            }
            else {
                UE_LOG(LogTemp, Error, TEXT("Invalid node within array in JSON file: %s"), *FilePath);
                return false;
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to find 'nodes' array in JSON file: %s"), *FilePath);
        return false;
    }

    return true;
}

bool UNPC_RouteManager::GetNodeSpawnValue(int32 node_id)
{
    return SpawnNPCMap.Contains(node_id) && SpawnNPCMap[node_id];
}

void UNPC_RouteManager::ResetStaticMaps()
{
    RouteIDMap.Empty(); // Clear RouteIDMap
}
