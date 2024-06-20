// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskObject.h"

UTaskObject::UTaskObject() : DelayDuration(0.0f) {}

void UTaskObject::Init(UExperimentHUDWidget* Widget, float DelaySeconds, int RouteIndex, EReasonForReport Enum)
{
	DelayDuration = DelaySeconds;
	RouteIndex_ = RouteIndex;
	Enum_ = Enum;
	if (Widget) {
		HUDWidget = Widget;
	}
	else {
		// HANDLE THIS
	}
}

void UTaskObject::StartTimer()
{
	if (!HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject)) {
		// If we need to invalidate or update the TimerHandle, make this into a member variable!
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UTaskObject::DelayedFunction, DelayDuration, false);
	}
}

void UTaskObject::DelayedFunction()
{
	UExperimentHUDWidget* ValidWidget = HUDWidget.Get();
	if (ValidWidget) {
		ValidWidget->MakeReport(RouteIndex_, Enum_, DelayDuration);
	}
}