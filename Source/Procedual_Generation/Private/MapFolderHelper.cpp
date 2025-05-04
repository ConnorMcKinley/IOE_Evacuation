// Fill out your copyright notice in the Description page of Project Settings.


#include "MapFolderHelper.h"

void UMapFolderHelper::GetSubfoldersInDirectory(const FString& DirectoryPath, TArray<FString>& Subfolders, FString& ErrorMessage)
{
    Subfolders.Empty();

    FString MapsDirectory = FPaths::Combine(DirectoryPath, TEXT("Maps"));

    UE_LOG(LogTemp, Log, TEXT("MapsDirectory: %s"), *MapsDirectory);

    if (!IFileManager::Get().DirectoryExists(*MapsDirectory))
    {
        ErrorMessage = FString::Printf(TEXT("The Maps directory does not exist: %s"), *MapsDirectory);
        UE_LOG(LogTemp, Error, TEXT("%s"), *ErrorMessage);
        return;
    }

    IFileManager& FileManager = IFileManager::Get();
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

    // Iterate through subdirectories
    PlatformFile.IterateDirectory(*MapsDirectory, [&](const TCHAR* Path, bool bIsDirectory) -> bool
        {
            FString FullPath(Path);
            if (bIsDirectory && FullPath != MapsDirectory)
            {
                FString SubfolderName = FPaths::GetCleanFilename(FullPath);
                UE_LOG(LogTemp, Log, TEXT("Adding subfolder: %s"), *SubfolderName);
                Subfolders.Add(SubfolderName);
            }
            return true; // Continue iteration
        });

    if (Subfolders.Num() == 0)
    {
        ErrorMessage = FString::Printf(TEXT("No subfolders found in the Maps directory: %s"), *MapsDirectory);
        UE_LOG(LogTemp, Error, TEXT("%s"), *ErrorMessage);
    }
    else
    {
        ErrorMessage = FString(TEXT("Success"));
        UE_LOG(LogTemp, Log, TEXT("%s: Found %d subfolders"), *ErrorMessage, Subfolders.Num());
    }
}

bool UMapFolderHelper::SaveRenderTargetToDisk(UTextureRenderTarget2D* RenderTarget, const FString& FileName, const FString& Path)
{
    if (!RenderTarget)
    {
        UE_LOG(LogTemp, Error, TEXT("RenderTarget is null"));
        return false;
    }

    FTextureRenderTargetResource* RenderTargetResource = RenderTarget->GameThread_GetRenderTargetResource();
    TArray<FColor> Bitmap;

    FReadSurfaceDataFlags ReadSurfaceDataFlags;
    ReadSurfaceDataFlags.SetLinearToGamma(true);
    RenderTargetResource->ReadPixels(Bitmap, ReadSurfaceDataFlags);

    if (Bitmap.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("Bitmap array is empty. Render target may not have captured correctly."));
    }

    // Ensure the ImageWrapper module is available
    IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
    TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);
    if (!ImageWrapper.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create image wrapper for PNG format"));
        return false;
    }

    ImageWrapper->SetRaw(Bitmap.GetData(), Bitmap.Num() * sizeof(FColor), RenderTarget->GetSurfaceWidth(), RenderTarget->GetSurfaceHeight(), ERGBFormat::BGRA, 8);
    const TArray64<uint8>& CompressedBitmap64 = ImageWrapper->GetCompressed();
    TArray<uint8> CompressedBitmap(CompressedBitmap64);

    FString Directory = FPaths::ProjectDir() / Path;
    FString MapPath = FPaths::ProjectDir() / Path / FileName;
    

    UE_LOG(LogTemp, Log, TEXT("Output PNG location: %s"), *MapPath);

    if (!IFileManager::Get().DirectoryExists(*Directory))
    {
        UE_LOG(LogTemp, Error, TEXT("Directory Does Not Exist!"));
        return false;
    }

    // Save to disk
    bool bSaved = FFileHelper::SaveArrayToFile(CompressedBitmap, *MapPath);
    if (bSaved)
    {
        UE_LOG(LogTemp, Log, TEXT("Saved render target to: %s"), *MapPath);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to save render target to: %s"), *MapPath);
    }

    return bSaved;
}

bool UMapFolderHelper::GetMapJsonInformation(const FString& FilePath, FString& Err, int& NumNodes, int& NumConnections)
{
    FString FullPath = FPaths::ProjectDir() / FilePath;

    // Check if the file exists
    if (!FPaths::FileExists(FullPath))
    {
        Err = FString::Printf(TEXT("File does not exist: %s"), *FullPath);
        UE_LOG(LogTemp, Error, TEXT("%s"), *Err);
        return false;
    }

    // Read the JSON file
    FString FileContent;
    if (!FFileHelper::LoadFileToString(FileContent, *FullPath))
    {
        Err = FString::Printf(TEXT("Failed to read file: %s"), *FullPath);
        UE_LOG(LogTemp, Error, TEXT("%s"), *Err);
        return false;
    }

    // Parse the JSON content
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(FileContent);

    if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
    {
        Err = TEXT("Failed to parse JSON content");
        UE_LOG(LogTemp, Error, TEXT("%s"), *Err);
        return false;
    }

    // Get the "nodes" array
    const TArray<TSharedPtr<FJsonValue>>* NodesArray;
    if (JsonObject->TryGetArrayField(TEXT("nodes"), NodesArray))
    {
        NumNodes = NodesArray->Num();
    }
    else
    {
        Err = TEXT("Failed to find 'nodes' field in JSON");
        UE_LOG(LogTemp, Error, TEXT("%s"), *Err);
        return false;
    }

    // Get the "connections" array
    const TArray<TSharedPtr<FJsonValue>>* ConnectionsArray;
    if (JsonObject->TryGetArrayField(TEXT("connections"), ConnectionsArray))
    {
        NumConnections = ConnectionsArray->Num();
    }
    else
    {
        Err = TEXT("Failed to find 'connections' field in JSON");
        UE_LOG(LogTemp, Error, TEXT("%s"), *Err);
        return false;
    }

    // Log the information
    UE_LOG(LogTemp, Log, TEXT("Number of Nodes: %d, Number of Connections: %d"), NumNodes, NumConnections);

    return true;
}