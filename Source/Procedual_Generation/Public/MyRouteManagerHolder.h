// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UNPC_RouteManager.h"
#include "Components/SceneComponent.h"

#include "MyRouteManagerHolder.generated.h"

UCLASS()
class PROCEDUAL_GENERATION_API AMyRouteManagerHolder : public AActor
{
    GENERATED_BODY()

public:
    AMyRouteManagerHolder();
    ~AMyRouteManagerHolder();

protected:
    virtual void BeginPlay() override;
    virtual void BeginDestroy() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USceneComponent* RootSceneComponent;

    UPROPERTY(Replicated)
    TObjectPtr<UNPC_RouteManager> NPC_RouteManager;

    UFUNCTION(BlueprintCallable, Category = "NPC Route Management")
    UNPC_RouteManager* GetNPCRouteManager() const {
        return NPC_RouteManager.Get();
    }


    void CleanupSubobject();

private:
    void CreateClass();
};