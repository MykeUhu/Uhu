// Copyright by MykeUhu


#include "UI/Widget/UhuUserWidget.h"

void UUhuUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}