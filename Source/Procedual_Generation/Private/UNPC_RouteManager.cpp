#include "UNPC_RouteManager.h"

TMap<int32, bool> UNPC_RouteManager::RouteIDMap;
TMap<int32, int32> UNPC_RouteManager::NumberOfNPCsAtRouteMap;

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

bool UNPC_RouteManager::ReadAndInitNPCNodeMap(const UObject* WorldContextObject, int32 num_routes, int32 DefaultNPCValue)
{
    for (int32 i = 0; i < num_routes; i++) {
        NumberOfNPCsAtRouteMap.Add(i, 0);
    }

    FString FilePath = FPaths::ProjectDir() / TEXT("NPC.json");
    FString JsonString;
    if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load JSON file: %s"), *FilePath);
        return false;
    }

    // Parse .json file
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

    if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON file: %s"), *FilePath);
        return false;
    }
    // Read object from JSON
    const TSharedPtr<FJsonObject>* RoutesObject;
    if (JsonObject->TryGetObjectField(TEXT("routes"), RoutesObject))
    {
        // Iterate over each route in the object
        for (auto& RoutePair : (*RoutesObject)->Values)
        {
            int32 RouteID = FCString::Atoi(*RoutePair.Key);
            int32 NPCCount = DefaultNPCValue;

            // Check if the route has a specific NPC count
            if (RoutePair.Value.IsValid() && RoutePair.Value->Type == EJson::Number)
            {
                NPCCount = RoutePair.Value->AsNumber();
            }

            // Update the SpawnNPCMap with the NPC count for the route
            if (int32* MapValue = NumberOfNPCsAtRouteMap.Find(RouteID)) {
                *MapValue = NPCCount;
                UE_LOG(LogTemp, Log, TEXT("Route: %d has %d NPCs"), RouteID, NPCCount);
            }
            else {
                UE_LOG(LogTemp, Error, TEXT("Invalid route ID in JSON file: %s"), *FilePath);
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

int32 UNPC_RouteManager::GetRouteNumNPCs(int32 route_id)
{
    return NumberOfNPCsAtRouteMap.Contains(route_id) ? NumberOfNPCsAtRouteMap[route_id] : -1 ;
}

void UNPC_RouteManager::ResetStaticMaps()
{
    RouteIDMap.Empty(); // Clear RouteIDMap
}
