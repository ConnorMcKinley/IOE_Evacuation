#include "MyRouteManagerHolder.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "Engine/NetConnection.h"

// Sets default values
AMyRouteManagerHolder::AMyRouteManagerHolder()
{
    // Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.

    bReplicates = true;
    NPC_RouteManager = nullptr;
    bReplicateUsingRegisteredSubObjectList = true;
    bAlwaysRelevant = true;

    RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
    RootComponent = RootSceneComponent;

}

AMyRouteManagerHolder::~AMyRouteManagerHolder()
{
    UE_LOG(LogTemp, Warning, TEXT("AMyRouteManagerHolder Destructor Called"));

    CleanupSubobject();
}

void AMyRouteManagerHolder::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        CreateClass();
    }
}

void AMyRouteManagerHolder::BeginDestroy()
{
    Super::BeginDestroy();
    UE_LOG(LogTemp, Warning, TEXT("AMyRouteManagerHolder::BeginDestroy Called"));

    CleanupSubobject();
}

void AMyRouteManagerHolder::CreateClass()
{
    if (IsValid(NPC_RouteManager))
    {
        RemoveReplicatedSubObject(NPC_RouteManager);
    }

    NPC_RouteManager = NewObject<UNPC_RouteManager>(this);

    // MyActorSubobject becomes a replicated subobject here 
    AddReplicatedSubObject(NPC_RouteManager);
}

void AMyRouteManagerHolder::CleanupSubobject()
{
    if (NPC_RouteManager)
    {
        RemoveReplicatedSubObject(NPC_RouteManager);
        NPC_RouteManager->MarkPendingKill(); // Mark the subobject for garbage collection
        NPC_RouteManager = nullptr;
    }
}

void AMyRouteManagerHolder::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AMyRouteManagerHolder, NPC_RouteManager);
}