#include "UNPC_RouteManager.h"

UNPC_RouteManager::UNPC_RouteManager()
{

}

void UNPC_RouteManager::LogCurrentRouteIDs()
{
    UE_LOG(LogTemp, Log, TEXT("Logging All RouteIDs"));
    // Log all RouteIDs in MapRouteIDtoInstances
    for (const auto& Entry : MapRouteIDtoInstances)
    {
        UE_LOG(LogTemp, Log, TEXT("MapRouteIDtoInstances: Current RouteID: %d exists in MapRouteIDtoInstances"), Entry.Key);
    }

    // Log NPC data in NPCDataArray
    for (const F_NPCData& Data : NPCDataArray)
    {
        // Convert boolean to string
        FString SpawnedStatus = Data.bHasNPCsSpawned ? TEXT("Yes") : TEXT("No");
        UE_LOG(LogTemp, Log, TEXT("NPCDataArray: Current RouteID: %d - Number of NPCs: %d. Spawned? %s"), Data.RouteID, Data.NumberOfNPCs, *SpawnedStatus);
    }
}

void UNPC_RouteManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UNPC_RouteManager, NPCDataArray);
    DOREPLIFETIME(UNPC_RouteManager, RouteConnectionArray);
}

void UNPC_RouteManager::ServerAddNPCSpawnedAtRoute(const UObject* WorldContextObject, int32 RouteID, FRouteData NPC_Data)
{
    
    MapRouteIDtoInstances.Add(RouteID, NPC_Data);
    UE_LOG(LogTemp, Log, TEXT("Added RouteID: %d to MapRouteIDtoInstances"), RouteID);
    UpdateNPCDataArray(RouteID, true);
}

void UNPC_RouteManager::ServerRemoveNPCSpawnedAtRoute(const UObject* WorldContextObject, int32 RouteID)
{
    UE_LOG(LogTemp, Log, TEXT("RemoveNPCSpawnedAtRoute called with RouteID: %d"), RouteID);

    LogCurrentRouteIDs();

    // Check if the RouteID exists in the map
    if (FRouteData* RouteData = MapRouteIDtoInstances.Find(RouteID))
    {
        UE_LOG(LogTemp, Log, TEXT("Deleting %d in the MapRouteIDtoInstances"), RouteID);
        // Destroy all BoxInstances
        for (ABP_BoundaryBox* BoxInstance : RouteData->BoxInstances)
        {
            if (BoxInstance)
            {
                if (BoxInstance->IsValidLowLevel())
                {
                    UE_LOG(LogTemp, Log, TEXT("Deleting BoxInstance: %s"), *BoxInstance->GetName());
                    BoxInstance->Destroy();
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("BoxInstance is not valid: %s"), *BoxInstance->GetName());
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("BoxInstance pointer is null."));
            }
        }

        // Destroy all NPCInstances
        for (ABP_NPC* NPCInstance : RouteData->NPCInstances)
        {
            if (NPCInstance)
            {
                if (NPCInstance->IsValidLowLevel())
                {
                    UE_LOG(LogTemp, Log, TEXT("Deleting NPCInstance: %s"), *NPCInstance->GetName());
                    NPCInstance->Destroy();
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("NPCInstance is not valid: %s"), *NPCInstance->GetName());
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("NPCInstance pointer is null."));
            }
        }

        // Remove the RouteID from the map
        MapRouteIDtoInstances.Remove(RouteID);
        UpdateNPCDataArray(RouteID, false);

    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("RouteID: %d not found in the MapRouteIDtoInstances"), RouteID);
    }
}

/// <summary>
/// Update the client's array on whether the RouteID has spawned in its NPCs or not
/// </summary>
/// <param name="RouteID"></param>
/// <param name="bHasNPCSpawned"></param>
void UNPC_RouteManager::UpdateNPCDataArray(int32 RouteID, bool bHasNPCSpawned)
{

    for (F_NPCData& Data : NPCDataArray)
    {
        if (Data.RouteID == RouteID)
        {
            Data.bHasNPCsSpawned = bHasNPCSpawned;
            break;
        }
    }
}

bool UNPC_RouteManager::IsValidJsonFile(const UObject* WorldContextObject, FString& Err)
{
    FString NPCFilePath = FPaths::ProjectDir() / TEXT("NPC.json");
    if (!FPaths::FileExists(NPCFilePath)) {
        Err = "NPC.json Does Not Exist!";
        return false;
    }

    FString NPCFileContent;
    if (!FFileHelper::LoadFileToString(NPCFileContent, *NPCFilePath)) {
        Err = "Failed to read NPC.json!";
        return false;
    }

    TSharedPtr<FJsonObject> NPCJsonObject;
    TSharedRef<TJsonReader<>> NPCReader = TJsonReaderFactory<>::Create(NPCFileContent);
    if (!FJsonSerializer::Deserialize(NPCReader, NPCJsonObject) || !NPCJsonObject.IsValid()) {
        Err = "Failed to parse NPC.json!";
        return false;
    }

    // Load and parse map.json
    FString MapFilePath = FPaths::ProjectDir() / TEXT("map.json");
    if (!FPaths::FileExists(MapFilePath)) {
        Err = "map.json Does Not Exist!";
        return false;
    }

    FString MapFileContent;
    if (!FFileHelper::LoadFileToString(MapFileContent, *MapFilePath)) {
        Err = "Failed to read map.json!";
        return false;
    }

    TSharedPtr<FJsonObject> MapJsonObject;
    TSharedRef<TJsonReader<>> MapReader = TJsonReaderFactory<>::Create(MapFileContent);
    if (!FJsonSerializer::Deserialize(MapReader, MapJsonObject) || !MapJsonObject.IsValid()) {
        Err = "Failed to parse map.json!";
        return false;
    }

    // Check if 'connections' field exists and is a JSON array in map.json
    const TArray<TSharedPtr<FJsonValue>>* MapConnectionsArray;
    if (!MapJsonObject->TryGetArrayField(TEXT("connections"), MapConnectionsArray))
    {
        Err = "map.json does not contain a valid 'connections' array!";
        return false;
    }

    TSet<TPair<int32, int32>> ValidConnectionsSet;
    for (const TSharedPtr<FJsonValue>& Value : *MapConnectionsArray)
    {
        if (Value->Type == EJson::Object)
        {
            int32 NodeIndex1, NodeIndex2;
            if (Value->AsObject()->TryGetNumberField(TEXT("nodeIndex1"), NodeIndex1) &&
                Value->AsObject()->TryGetNumberField(TEXT("nodeIndex2"), NodeIndex2))
            {
                int32 LowerNode = FMath::Min(NodeIndex1, NodeIndex2);
                int32 HigherNode = FMath::Max(NodeIndex1, NodeIndex2);
                ValidConnectionsSet.Add(TPair<int32, int32>(LowerNode, HigherNode));
            }
        }
    }

    // Check if 'connections' field exists and is a JSON object in NPC.json
    const TSharedPtr<FJsonObject>* NPCConnectionsObject;
    if (!NPCJsonObject->TryGetObjectField(TEXT("connections"), NPCConnectionsObject))
    {
        Err = "NPC.json does not contain a valid 'connections' object!";
        return false;
    }

    TMap<FString, int32> MapConnectionCounter;
    // Validate the format and content of the 'connections' object
    for (const auto& Connection : (*NPCConnectionsObject)->Values)
    {
        FString KeyString = Connection.Key;

        int32 HyphenIdx;
        if (!KeyString.FindChar('-', HyphenIdx))
        {
            Err = FString::Printf(TEXT("Invalid connection key format: %s"), *KeyString);
            return false;
        }

        int32 NodeA, NodeB;
        bool bIsValidNodeA = FDefaultValueHelper::ParseInt(KeyString.Left(HyphenIdx), NodeA);
        bool bIsValidNodeB = FDefaultValueHelper::ParseInt(KeyString.Mid(HyphenIdx + 1), NodeB);

        if (!bIsValidNodeA || !bIsValidNodeB)
        {
            Err = FString::Printf(TEXT("Invalid nodes in connection key: %s"), *KeyString);
            return false;
        }

        if (!(Connection.Value.IsValid() &&
            (Connection.Value->Type == EJson::Number || Connection.Value->Type == EJson::Null)))
        {
            Err = FString::Printf(TEXT("Invalid value type for connection key: %s"), *KeyString);
            return false;
        }

        int32 LowerNode = FMath::Min(NodeA, NodeB);
        int32 HigherNode = FMath::Max(NodeA, NodeB);
        FString NewKey = FString::FromInt(LowerNode) + '-' + FString::FromInt(HigherNode);
        UE_LOG(LogTemp, Log, TEXT("Current Key is: %s"), *NewKey);

        MapConnectionCounter.FindOrAdd(NewKey)++;

        if (MapConnectionCounter[NewKey] > 1)
        {
            Err = FString::Printf(TEXT("Multiple entries exist for connection %i-%i"), NodeA, NodeB);
            return false;
        }

        if (!ValidConnectionsSet.Contains(TPair<int32, int32>(LowerNode, HigherNode)))
        {
            Err = FString::Printf(TEXT("Connection %i-%i in NPC.json does not exist in map.json"), LowerNode, HigherNode);
            return false;
        }
    }

    return true;
}

UNPC_RouteManager* UNPC_RouteManager::CreateNPC_RouteManager(UObject* Outer)
{
    UNPC_RouteManager* NewRouteManager = NewObject<UNPC_RouteManager>(Outer);
    return NewRouteManager;
}

bool UNPC_RouteManager::ReadAndInitNPCNodeMap(const UObject* WorldContextObject, int32 DefaultNPCValue, const TArray<FS_Map_Route> PermanentRoutes)
{
    // server DS
    MapConnectionToRouteID.Empty();

    // local arrays start empty
    NPCDataArray.Empty();
    RouteConnectionArray.Empty();
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

                        // Fill the replicated NPC data array
                        F_NPCData NPCData;
                        NPCData.RouteID = *RouteID;
                        NPCData.NumberOfNPCs = NPCCount;
                        NPCData.bHasNPCsSpawned = false;
                        NPCDataArray.Add(NPCData);
                    }
                    else
                    {
                        UE_LOG(LogTemp, Error, TEXT("Route ID not found for key: %s"), *KeyString);
                        return false;
                    }

                    // Fill the replicated route connection array
                    F_ReplicatedRouteConnection RouteConnection;
                    RouteConnection.RouteKey = Key;
                    RouteConnection.RouteID = *RouteID;
                    RouteConnectionArray.Add(RouteConnection);
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
    LogCurrentRouteIDs();
    return true;
}

bool UNPC_RouteManager::LocalHasNPCSpawnedAtRoute(int32 RouteID) const
{
    for (const F_NPCData& Data : NPCDataArray) {
        if (Data.RouteID == RouteID) {
            return Data.bHasNPCsSpawned;
        }
    }
    return false;
}

int32 UNPC_RouteManager::LocalGetRouteNumNPCs(int32 RouteID) const
{
    for (const F_NPCData& Data : NPCDataArray) {
        if (Data.RouteID == RouteID) {
            return Data.NumberOfNPCs;
        }
    }
    return -1;
}
