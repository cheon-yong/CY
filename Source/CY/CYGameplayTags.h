// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"

/**
 * 
 */
namespace CYGameplayTags
{
	CY_API FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString = false);

	CY_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	CY_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Mouse);
	CY_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Attack_Basic);
	CY_API  UE_DECLARE_GAMEPLAY_TAG_EXTERN(Gameplay_AbilityInputBlocked);
}