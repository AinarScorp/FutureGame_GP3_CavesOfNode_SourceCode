// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings/GameSettings.h"

UGameSettings::UGameSettings()
{
	Sensitivity = 1.0f;
}

UGameSettings* UGameSettings::GetGameSettings()
{
	return Cast<UGameSettings>(UGameUserSettings::GetGameUserSettings());
}