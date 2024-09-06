// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "MyPlayerState.h"
#include "GameFramework/PlayerState.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ALearnGASCharacter

AMyCharacter::AMyCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AMyCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// 客户端初始化 ASC：因为 ASC 是挂载在 PlayerState 上的，所以客户端要初始化 ASC，必须等 PlayerState 同步下来
	if(UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->InitAbilityActorInfo(GetPlayerState(), this);
	}
}

void AMyCharacter::OnAnyGameplayTagRemovedOrNewed(FGameplayTag GameplayTag, int Count)
{
	if(Count == 0)
	{
		
	}
}

void AMyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 服务器初始化 ASC
	// todo：暂时不知道为什么服务器推荐在这里初始化 ASC
	if(UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->InitAbilityActorInfo(GetPlayerState(), this);

		for(TSubclassOf<UGameplayAbility> GAClass : AbilitiesToGive)
		{
			FGameplayAbilitySpec AbilitySpec = ASC->BuildAbilitySpecFromClass(GAClass);
			ASC->GiveAbility(AbilitySpec);
		}

		// 监听一些 GAS 相关的事件，我认为只需要服务器监听即可
		ASC->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &AMyCharacter::OnActiveGameplayEffectAddedDelegateToSelf);

		FGameplayTag Enhance_1_Tag;
		FOnGameplayEffectTagCountChanged Delegate = ASC->RegisterGameplayTagEvent(Enhance_1_Tag, EGameplayTagEventType::NewOrRemoved);
		Delegate.AddUObject(this, &AMyCharacter::OnAnyGameplayTagRemovedOrNewed);

		// 抛送 ASC 初始化完成的事件到蓝图
		OnFinishAbilitySystemComponentInitialization();
	}
}

UAbilitySystemComponent* AMyCharacter::GetAbilitySystemComponent() const
{
	if(IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(GetPlayerState()))
	{
		return ASI->GetAbilitySystemComponent();
	}

	return GetComponentByClass<UAbilitySystemComponent>();
}

void AMyCharacter::OnFinishAbilitySystemComponentInitialization_Implementation()
{
}

void AMyCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	HasAuthority()
}

void AMyCharacter::OnActiveGameplayEffectAddedDelegateToSelf(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle GameplayEffectHandle)
{
	UE_LOG(LogTemp, Display, TEXT("qtc Add Active Gameplay: %s"), *GameplayEffectSpec.Def->GetName());

	FActiveGameplayEffect* ActiveGE = const_cast<FActiveGameplayEffect*>(TargetASC->GetActiveGameplayEffect(GameplayEffectHandle));
	if(ActiveGE)
	{
		ActiveGE->EventSet.OnInhibitionChanged.AddUObject(this, &AMyCharacter::OnInhibitionChanged);
	}
}

void AMyCharacter::OnInhibitionChanged(FActiveGameplayEffectHandle Handle, bool bInhibited)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	auto ActiveGE = ASC->GetActiveGameplayEffect(Handle)->Spec.Def;
	UE_LOG(LogTemp, Display, TEXT("qtc OnActiveGameplayEffectInhibitionChanged: %s, %s"), *ActiveGE->GetName(), bInhibited ? TEXT("true") : TEXT("false"));
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AMyCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMyCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}