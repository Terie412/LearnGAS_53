// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "MyCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AMyCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	// 测试用
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayAbility>> AbilitiesToGive;

public:
	UPROPERTY(BlueprintReadWrite)
	TArray<FGameplayEffectSpec> GESpecsToApply;

	AMyCharacter();

	virtual void OnRep_PlayerState() override;

	void OnAnyGameplayTagRemovedOrNewed(FGameplayTag GameplayTag, int Count);

	virtual void PossessedBy(AController* NewController) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintNativeEvent)
	void OnFinishAbilitySystemComponentInitialization();

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	void OnInhibitionChanged(FActiveGameplayEffectHandle Handle, bool bIsInhibited);

	void OnActiveGameplayEffectAddedDelegateToSelf(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle GameplayEffectHandle);

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay();

	struct FPeople
	{
		float FValue;
		int IValue;
		char CValue;
	};

	UFUNCTION(BlueprintCallable)
	void Test(int32 Index)
	{
		FOnGivenActiveGameplayEffectRemoved OnRemoveDelegate = GetAbilitySystemComponent()->OnAnyGameplayEffectRemovedDelegate();
		OnRemoveDelegate.AddUObject(this, &AMyCharacter::OnAnyGameplayEffectRemoved);
	}

	UFUNCTION()
	void OnAnyGameplayEffectRemoved(const FActiveGameplayEffect& RemovedGameplayEffect)
	{
		const FActiveGameplayEffectHandle& RemovedGEHandle = RemovedGameplayEffect.Handle;
		FActiveGameplayEffectHandle GEHandle = GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(GESpecsToApply[0]);
		
	}

	void AddMedineReadyToApply(TSubclassOf<UGameplayEffect> GEClass, int Level)
	{
		FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponent()->MakeEffectContext();
		FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GEClass, Level, EffectContext);

		int32 Index = GESpecsToApply.Add(*SpecHandle.Data);
		SpecHandle.Data->SetByCallerNameMagnitudes.Add("MedicineIndex", Index);
	}
};
