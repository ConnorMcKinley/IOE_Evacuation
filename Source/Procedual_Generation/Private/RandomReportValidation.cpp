// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomReportValidation.h"

bool URandomReportValidation::URandomReportValidateMinMax(int MinValue, const FString& MaxValue, FString& OutErrorMessage)
{
    if (MinValue <= 0) {
        OutErrorMessage = TEXT("Minimum value must be greater than 0.");
        return false;
    }

    if (!MaxValue.Equals("inf", ESearchCase::IgnoreCase)) {
        float MaxValueFloat;
        if (!FDefaultValueHelper::ParseFloat(MaxValue, MaxValueFloat) || MinValue > MaxValueFloat) {
            OutErrorMessage = TEXT("Max value must be a valid! (inf is default)");
            return false;
        }
    }

    return true;
}

bool URandomReportValidation::UTimeLagValidateMinMax(float MinValue, float MaxValue, FString& OutErrorMessage)
{
    if (MinValue < 0 || MinValue > MaxValue) {
        OutErrorMessage = TEXT("Time Lag Values are Invalid!");
        return false;
    }
    return true;
}

bool URandomReportValidation::UPenaltyTimeValidate(float PenaltyValue, FString& OutErrorMessage)
{
    if (PenaltyValue <= 0) {
        OutErrorMessage = TEXT("Penalty Time must be greater than 0!");
        return false;
    }
    return true;
}
