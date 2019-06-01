// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TunnelledInputFunLib.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_DIPLOMSKI_API UTunnelledInputFunLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		static void TestPrintSomething(FString nesto);
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		static void TestReturnVector(FVector& myVec_out);

	UFUNCTION(BlueprintCallable, Category = "My Functions")
		static void ConnectToTunnel(bool& connected);
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		static void GetTunnelledInput(TArray<float>& tracks);

};
