// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define CCHANNEL_CYACTION							ECC_GameTraceChannel1

#define CY_TraceChannel_Interaction					ECC_GameTraceChannel2

// Trace used by weapons, will hit physics assets instead of capsules
#define CY_TraceChannel_Weapon						ECC_GameTraceChannel3

// Trace used by by weapons, will hit pawn capsules instead of physics assets
#define CY_TraceChannel_Weapon_Capsule				ECC_GameTraceChannel4

// Trace used by by weapons, will trace through multiple pawns rather than stopping on the first hit
#define CY_TraceChannel_Weapon_Multi				ECC_GameTraceChannel5