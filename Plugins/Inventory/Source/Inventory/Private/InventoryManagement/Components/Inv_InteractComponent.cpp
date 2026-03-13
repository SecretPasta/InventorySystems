// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagement/Components/Inv_InteractComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "Interaction/Inv_Highlightable.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/HUD/Inv_HUDWidget.h"


class UEnhancedInputLocalPlayerSubsystem;

UInv_InteractComponent::UInv_InteractComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	TraceLength = 500.0f;
	ItemTraceChannel = ECC_GameTraceChannel1;
}


void UInv_InteractComponent::BeginPlay()
{
	Super::BeginPlay();
	SetupInputComponent();
	CreateHUDWidget();
	InventoryComponent = GetOwner()->FindComponentByClass<UInv_InventoryComponent>();
}

// *********************************************************************************************************
//   Actor Components do not have a SetupInputComponent function that can be overriden.
//   Therefore, this is defined manually and called from BeginPlay.
//   Get the PlayerController this is attached to and then bind actions to the PC's Enhanced Input Component
// *********************************************************************************************************
void UInv_InteractComponent::SetupInputComponent()
{
	if (const APlayerController* PC = Cast<APlayerController>(GetOwner()))
	{
		if (const ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* EISubSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				EISubSystem->AddMappingContext(DefaultIMC, 0);
			}
		}

		// Retrieve the EnhancedInputComponent from the PC
		if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PC->InputComponent))
		{
			// now bind exactly as we did in SetupInputComponent on the PC
			EIC->BindAction(PrimaryInteractAction, ETriggerEvent::Started,this, &ThisClass::PrimaryInteract);
			EIC->BindAction(ToggleInventoryAction, ETriggerEvent::Started,this, &ThisClass::ToggleInventory);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Inv_InteractionComponent: Owning PlayerController has no EnhancedInputComponent!"));
		}
	}
}

void UInv_InteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TraceForItem();
}


void UInv_InteractComponent::PrimaryInteract()
{
	//UE_LOG(LogTemp, Warning, TEXT("PrimaryInteract pressed."));
	if (!ThisActor.IsValid()) return;

	UInv_ItemComponent* ItemComp = ThisActor->FindComponentByClass<UInv_ItemComponent>();
	if (!IsValid(ItemComp) || !InventoryComponent.IsValid()) return;

	InventoryComponent->TryAddItem(ItemComp);
}

void UInv_InteractComponent::CreateHUDWidget()
{
	APlayerController* PC = Cast<APlayerController>(GetOwner());
	if (!PC->IsLocalController()) return; 
	
	HUDWidget = CreateWidget<UInv_HUDWidget>(PC, HUDWidgetClass);
	if (IsValid(HUDWidget))
	{
		HUDWidget->AddToViewport();
	}
}

void UInv_InteractComponent::ToggleInventory()
{
	if (!InventoryComponent.IsValid()) return;
	InventoryComponent->ToggleInventoryMenu();
	if (InventoryComponent->IsMenuOpen())
	{
		HUDWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		HUDWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}


void UInv_InteractComponent::TraceForItem()
{
	if (!IsValid(GEngine) || !IsValid(GEngine->GameViewport)) return;

	const APlayerController* PC = Cast<APlayerController>(GetOwner());
	
	FVector2D ViewportSize;
	FVector TraceStart;
	FVector Forward;
	
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	const FVector2D ViewportCenter = ViewportSize/2;
	
	if (!UGameplayStatics::DeprojectScreenToWorld(PC, ViewportCenter, TraceStart, Forward)) return;

	const FVector TraceEnd = TraceStart + Forward * TraceLength;
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ItemTraceChannel);

	LastActor = ThisActor;
	ThisActor = HitResult.GetActor();

	//if not focusing on a valid Item, then hide the pickup message
	if (!ThisActor.IsValid())
	{
		if (IsValid(HUDWidget)) HUDWidget->HidePickupMessage();
	}

	if (ThisActor.IsValid())
	{
		if (UActorComponent* Highlightable = ThisActor->FindComponentByInterface(UInv_Highlightable::StaticClass()); IsValid(Highlightable))
		{
			IInv_Highlightable::Execute_Highlight(Highlightable);
		}
		
		UInv_ItemComponent* ItemComponent = ThisActor->FindComponentByClass<UInv_ItemComponent>();
		if (!IsValid(ItemComponent)) return;

		if (IsValid(HUDWidget)) HUDWidget->ShowPickupMessage(ItemComponent->GetPickupMessage());		
	}
	
	if (ThisActor==LastActor) return;

	if (ThisActor.IsValid())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Started Tracing: %s"), *ThisActor->GetName());
	}

	if (LastActor.IsValid())
	{
		if (UActorComponent* Highlightable = LastActor->FindComponentByInterface(UInv_Highlightable::StaticClass()); IsValid(Highlightable))
		{
			IInv_Highlightable::Execute_UnHighlight(Highlightable);
		}
		//UE_LOG(LogTemp, Warning, TEXT("Stopped tracing: %s"), *LastActor->GetName());
	}
}

