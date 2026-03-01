// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ItemPopUp/Inv_ItemPopUp.h"

#include "Components/Button.h"
#include "Components/Slider.h"

void UInv_ItemPopUp::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Button_Split->OnClicked.AddDynamic(this, &UInv_ItemPopUp::SplitButtonClicked);
	Button_Drop->OnClicked.AddDynamic(this, &UInv_ItemPopUp::DropButtonClicked);
	Button_Consume->OnClicked.AddDynamic(this, &UInv_ItemPopUp::ConsumeButtonClicked);
	Slider_Split->OnValueChanged.AddDynamic(this, &UInv_ItemPopUp::SliderValueChanged);
}

void UInv_ItemPopUp::SplitButtonClicked()
{
}

void UInv_ItemPopUp::DropButtonClicked()
{
}

void UInv_ItemPopUp::ConsumeButtonClicked()
{
}

void UInv_ItemPopUp::SliderValueChanged(float Value)
{
}
