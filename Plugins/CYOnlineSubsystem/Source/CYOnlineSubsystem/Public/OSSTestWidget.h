// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "OSSTestWidget.generated.h"

class UCYMultiPlayerSubsystem;

/**
 * 
 */
UCLASS()
class CYONLINESUBSYSTEM_API UOSSTestWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void MenuSetup(int32 NumberOfPublicConnections = 4, FString TypeOfMatch = FString(TEXT("FreeForAll")), FString LobbyPath = FString(TEXT("/Game/ThirdPerson/Maps/Lobby")));

protected:
	virtual bool Initialize() override;		// Call After UGameInstance Initialize
	virtual void NativeDestruct() override; // Call when remove from parent

	// ----------------------------------------------------------------------
	// Callbacks for the custom delegate on the MultiPlayerSessionSubsystem
	// ----------------------------------------------------------------------
	UFUNCTION()
	void OnCreateSession(bool bWasSuccessful);
	void OnFindSession(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bwasSuccessful);
	void OnJoinSession(EOnJoinSessionCompleteResult::Type Result);
	UFUNCTION()
	void OnStartSession(bool bWasSuccessful);
	UFUNCTION()
	void OnDestroySession(bool bWasSuccessful);


private:
	UCYMultiPlayerSubsystem* MultiplayerSessionSubsystem; // The Subsystem designed to handle all online session functionality

	int32	NumPublicConnections{ 4 };
	FString MatchType{ TEXT("FreeForAll") };
	FString PathToLevel{ TEXT("/Game/ThirdPerson/Maps/Lobby") };


	UPROPERTY(meta = (BindWidget)) 
	class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	UFUNCTION()
	void HostButtonClicked();

	UFUNCTION()
	void JoinButtonClicked();

	// NativeDestruct()->MenuTearDown()
	void MenuTearDown();
};
