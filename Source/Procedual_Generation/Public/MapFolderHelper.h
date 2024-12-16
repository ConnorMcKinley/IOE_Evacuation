// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/TextureRenderTarget2D.h"
#include "ImageUtils.h" 
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"

#include "MapFolderHelper.generated.h"

/**
 * 
 */
UCLASS()
class PROCEDUAL_GENERATION_API UMapFolderHelper : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category="File Checker") 
	static void GetSubfoldersInDirectory(const FString& DirectoryPath, TArray<FString>& Subfolders, FString& ErrorMessage);

    UFUNCTION(BlueprintCallable, Category = "Rendering")
    static bool SaveRenderTargetToDisk(UTextureRenderTarget2D* RenderTarget, const FString& FileName, const FString& Path);

	UFUNCTION(BlueprintCallable, Category ="File Checker")
	static bool GetMapJsonInformation(const FString& FilePath, FString& Err, int& NumNodes, int& NumConnections);
};
