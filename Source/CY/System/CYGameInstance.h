// Fill out your copyright notice in the Description page of Project Settings.

#pragma once



#include "CommonGameInstance.h"
#include "CYGameInstance.generated.h"

class ACYPlayerController;
class UObject;

/**
 * 
 */
UCLASS(Config = Game)
class CY_API UCYGameInstance : public UCommonGameInstance
{
	GENERATED_BODY()
	
public:

//	UCYGameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
//
//	ACYPlayerController* GetPrimaryPlayerController() const;
//
//	virtual bool CanJoinRequestedSession() const override;
//	virtual void HandlerUserInitialized(const UCommonUserInfo* UserInfo, bool bSuccess, FText Error, ECommonUserPrivilege RequestedPrivilege, ECommonUserOnlineContext OnlineContext) override;
//
//	virtual void ReceivedNetworkEncryptionToken(const FString& EncryptionToken, const FOnEncryptionKeyResponse& Delegate) override;
//	virtual void ReceivedNetworkEncryptionAck(const FOnEncryptionKeyResponse& Delegate) override;
//
//protected:
//
//	virtual void Init() override;
//	virtual void Shutdown() override;
//
//	void OnPreClientTravelToSession(FString& URL);
//
//	/** A hard-coded encryption key used to try out the encryption code. This is NOT SECURE, do not use this technique in production! */
//	TArray<uint8> DebugTestEncryptionKey;
};
