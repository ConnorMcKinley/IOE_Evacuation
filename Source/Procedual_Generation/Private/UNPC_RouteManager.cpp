#include "UNPC_RouteManager.h"

TMap<int32, int32> UNPC_RouteManager::NumberOfNPCsAtRouteMap;
TMap<FRouteKey, int32> UNPC_RouteManager::MapConnectionToRouteID;
TMap<int32, FRouteData> UNPC_RouteManager::MapRouteIDtoInstances;

UNPC_RouteManager::UNPC_RouteManager()
{
    // Initialize default values here if necessary (NonStatic!)
}

bool UNPC_RouteManager::HasNPCSpawnedAtRoute(const UObject* WorldContextObject, int32 RouteID)
{
    return MapRouteIDtoInstances.Contains(RouteID);
}

void UNPC_RouteManager::AddNPCSpawnedAtRoute(const UObject* WorldContextObject, int32 RouteID, FRouteData NPC_Data)
{
    MapRouteIDtoInstances.Add(RouteID, NPC_Data);
}

void UNPC_RouteManager::RemoveNPCSpawnedAtRoute(const UObject* WorldContextObject, int32 RouteID)
{
    // Check if the RouteID exists in the map
    if (FRouteData* RouteData = MapRouteIDtoInstances.Find(RouteID))
    {
        // Destroy all BoxInstances
        for (ABP_BoundaryBox* BoxInstance : RouteData->BoxInstances)
        {
            if (BoxInstance && BoxInstance->IsValidLowLevel())
            {
                UE_LOG(LogTemp, Log, TEXT("Deleting BoxInstance: %s"), *BoxInstance->GetName());
                BoxInstance->Destroy();
            }
        }

        // Destroy all NPCInstances
        for (ABP_NPC* NPCInstance : RouteData->NPCInstances)
        {
            if (NPCInstance && NPCInstance->IsValidLowLevel())
            {
                UE_LOG(LogTemp, Log, TEXT("Deleting NPCInstance: %s"), *NPCInstance->GetName());
                NPCInstance->Destroy();
            }
        }

        // Remove the RouteID from the map
        MapRouteIDtoInstances.Remove(RouteID);
    }
}

bool UNPC_RouteManager::IsValidJsonFile(const UObject* WorldContextObject, FString& Err)
{
    FString FilePath = FPaths::ProjectDir() / TEXT("NPC.json");
    if (!FPaths::FileExists(FilePath)) {
        Err = "NPC.json Does Not Exist!";
        return false;
    }

    FString FileContent;
    if (!FFileHelper::LoadFileToString(FileContent, *FilePath)) {
        Err = "Failed to read NPC.json!";
        return false;
    }

    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(FileContent);

    if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid()) {
        Err = "Failed to parse NPC.json!";
        return false;
    }

    // Check if 'connections' field exists and is a JSON object
    const TSharedPtr<FJsonObject>* ConnectionsObject;
    if (!JsonObject->TryGetObjectField(TEXT("connections"), ConnectionsObject))
    {
        Err = "NPC.json does not contain a valid 'connections' object!";
        return false;
    }

    // Validate the format and content of the 'connections' object
    for (const auto& Connection : (*ConnectionsObject)->Values)
    {
        FString KeyString = Connection.Key;
        int32 HyphenIdx;

        // Check if the key format is 'NodeA-NodeB'
        if (!KeyString.FindChar('-', HyphenIdx))
        {
            Err = FString::Printf(TEXT("Invalid connection key format: %s"), *KeyString);
            return false;
        }

        // Ensure NodeA and NodeB are valid integers
        int32 NodeA, NodeB;
        bool bIsValidNodeA = FDefaultValueHelper::ParseInt(KeyString.Left(HyphenIdx), NodeA);
        bool bIsValidNodeB = FDefaultValueHelper::ParseInt(KeyString.Mid(HyphenIdx + 1), NodeB);

        if (!bIsValidNodeA || !bIsValidNodeB)
        {
            Err = FString::Printf(TEXT("Invalid nodes in connection key: %s"), *KeyString);
            return false;
        }

        // Ensure the value is either a number (for NPC count) or null
        if (!(Connection.Value.IsValid() &&
            (Connection.Value->Type == EJson::Number || Connection.Value->Type == EJson::Null)))
        {
            Err = FString::Printf(TEXT("Invalid value type for connection key: %s"), *KeyString);
            return false;
        }
    }

    return true;
}

bool UNPC_RouteManager::ReadAndInitNPCNodeMap(const UObject* WorldContextObject, int32 DefaultNPCValue, const TArray<FS_Map_Route> PermanentRoutes)
{
    MapConnectionToRouteID.Empty();
    for (const FS_Map_Route& Route : PermanentRoutes)
    {
        NumberOfNPCsAtRouteMap.Add(Route.route_id, 0);
        auto NodeStart = FMath::Min(Route.node_start, Route.node_end);
        auto NodeEnd = FMath::Max(Route.node_start, Route.node_end);
        FRouteKey Key(NodeStart, NodeEnd);
        MapConnectionToRouteID.Add(Key, Route.route_id);
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
    const TSharedPtr<FJsonObject>* ConnectionsObject;
    if (JsonObject->TryGetObjectField(TEXT("connections"), ConnectionsObject))
    {
        // Iterate over each connection
        for (auto& Connection : (*ConnectionsObject)->Values)
        {
            FString KeyString = Connection.Key;
            int32 HyphenIdx;

            if (KeyString.FindChar('-', HyphenIdx)) {
                // Extract NodeA and NodeB from the string
                int32 NodeA = FCString::Atoi(*KeyString.Left(HyphenIdx));
                int32 NodeB = FCString::Atoi(*KeyString.Mid(HyphenIdx + 1));

                // Ensure NodeA is always less than or equal to NodeB
                int32 MinNode = FMath::Min(NodeA, NodeB);
                int32 MaxNode = FMath::Max(NodeA, NodeB);
                const FRouteKey Key(MinNode, MaxNode);
                // Retrieve the RouteID from MapConnectionToRouteID map
                if (int32* RouteID = MapConnectionToRouteID.Find(Key))
                {
                    int32 NPCCount = DefaultNPCValue;

                    // Check if the route has a specific NPC count
                    if (Connection.Value.IsValid() && Connection.Value->Type == EJson::Number)
                    {
                        NPCCount = Connection.Value->AsNumber();
                    }

                    // Update the NumberOfNPCsAtRouteMap with the NPC count for the route
                    if (int32* MapValue = NumberOfNPCsAtRouteMap.Find(*RouteID))
                    {
                        *MapValue = NPCCount;
                        UE_LOG(LogTemp, Log, TEXT("Route: %d has %d NPCs"), *RouteID, NPCCount);
                    }
                    else
                    {
                        UE_LOG(LogTemp, Error, TEXT("Route ID not found for key: %s"), *KeyString);
                        return false;
                    }
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("Connection key %s not found in MapConnectionToRouteID"), *KeyString);
                    return false;
                }
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Invalid connection key format: %s"), *KeyString);
                return false;
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to find 'connections' array in JSON file: %s"), *FilePath);
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
    MapRouteIDtoInstances.Empty();
}
