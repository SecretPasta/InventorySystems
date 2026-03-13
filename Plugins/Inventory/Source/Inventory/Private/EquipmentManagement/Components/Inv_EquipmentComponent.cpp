// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentManagement/Components/Inv_EquipmentComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "InventoryManagement/Utils/Inv_InventoryStatics.h"


void UInv_EquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwningPlayerController = Cast<APlayerController>(GetOwner());
	if (OwningPlayerController.IsValid())
	{
		if (ACharacter* OwningCharacter = Cast<ACharacter>(OwningPlayerController->GetPawn()) ; IsValid(OwningCharacter))
		{
			OwningSkeletalMesh = OwningCharacter->GetMesh();
		}
		InitInventoryComponent();
	}
}

void UInv_EquipmentComponent::InitInventoryComponent()
{
	InventoryComponent = UInv_InventoryStatics::GetInventoryComponent(OwningPlayerController.Get());
	if (!InventoryComponent.IsValid()) return;
	
	if (!InventoryComponent->OnItemEquipped.IsAlreadyBound(this, & UInv_EquipmentComponent::OnItemEquipped))
	{
		InventoryComponent->OnItemEquipped.AddDynamic(this, & UInv_EquipmentComponent::OnItemEquipped);
	}
	if (!InventoryComponent->OnItemUnequipped.IsAlreadyBound(this, & UInv_EquipmentComponent::OnItemUnequipped))
	{
		InventoryComponent->OnItemUnequipped.AddDynamic(this, & UInv_EquipmentComponent::OnItemUnequipped);
	}
	
}

void UInv_EquipmentComponent::OnItemEquipped(UInv_InventoryItem* EquippedItem)
{
	
}

void UInv_EquipmentComponent::OnItemUnequipped(UInv_InventoryItem* EquippedItem)
{
	
}





