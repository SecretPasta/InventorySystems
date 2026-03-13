// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inv_InteractComponent.generated.h"


class UInv_InventoryComponent;
class UInv_HUDWidget;
class UInputMappingContext;
class UInputAction;


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class INVENTORY_API UInv_InteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInv_InteractComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ToggleInventory();	

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent();
	
private:
	void PrimaryInteract();
	void CreateHUDWidget();
	void TraceForItem();
	
	TWeakObjectPtr<UInv_InventoryComponent> InventoryComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TObjectPtr<UInputMappingContext> DefaultIMC;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TObjectPtr<UInputAction> PrimaryInteractAction;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TObjectPtr<UInputAction> ToggleInventoryAction;

	//assign the class for our HUD Widget in the editor
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UInv_HUDWidget> HUDWidgetClass;

	UPROPERTY()
	TObjectPtr<UInv_HUDWidget> HUDWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	double TraceLength;

	// Cannot expose an ENum directly for changing in the editor with UPROPERTY. It muSt be wrapped in a
	// TEnumAsByte<> for it to appear as a list.
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TEnumAsByte<ECollisionChannel> ItemTraceChannel;

	TWeakObjectPtr<AActor> ThisActor; //actor hit in this frame
	TWeakObjectPtr<AActor> LastActor; //actor hit last frame
};
