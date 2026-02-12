#include <cmath>
#include <iostream>

#include "drawer.h"
#include "saver.h"

int main()
{
	Image img({ 100, 50 }, 0x000000);

	// Снеговик снизу вверх (белый цвет)
	FillCircle(img, { 15, 38 }, 8, 0xFFFFFF);  // Белый снег
	FillCircle(img, { 15, 27 }, 6, 0xFFFFFF);  // Белый снег
	FillCircle(img, { 15, 18 }, 4, 0xFFFFFF);  // Белый снег

	// Шляпа (черный цвет)
	DrawLine(img, { 12, 14 }, { 18, 14 }, 0x000000);  // Черный
	DrawLine(img, { 13, 12 }, { 13, 14 }, 0x000000);  // Черный
	DrawLine(img, { 17, 12 }, { 17, 14 }, 0x000000);  // Черный
	DrawLine(img, { 13, 12 }, { 17, 12 }, 0x000000);  // Черный

	//солнце (желтый цвет)
	DrawCircle(img, { 70, 6 }, 4, 0xFFFF00);  // Желтый

	// снежинки (белый цвет)
	img.SetPixel({ 20, 3 }, 0xFFFFFF);   // Белый
	img.SetPixel({ 50, 2 }, 0xFFFFFF);   // Белый
	img.SetPixel({ 72, 25 }, 0xFFFFFF);  // Белый
	img.SetPixel({ 75, 15 }, 0xFFFFFF);  // Белый
	img.SetPixel({ 35, 20 }, 0xFFFFFF);  // Белый
	img.SetPixel({ 45, 15 }, 0xFFFFFF);  // Белый
	img.SetPixel({ 50, 10 }, 0xFFFFFF);  // Белый

	// Ствол (коричневый цвет)
	DrawLine(img, { 55, 41 }, { 55, 46 }, 0x8B4513);  // Коричневый

	// Ярусы ели (зеленый цвет)
	DrawLine(img, { 39, 38 }, { 71, 38 }, 0x00FF00);  // Зеленый
	DrawLine(img, { 42, 35 }, { 68, 35 }, 0x00FF00);  // Зеленый
	DrawLine(img, { 45, 32 }, { 65, 32 }, 0x00FF00);  // Зеленый
	DrawLine(img, { 48, 29 }, { 62, 29 }, 0x00FF00);  // Зеленый
	DrawLine(img, { 51, 26 }, { 59, 26 }, 0x00FF00);  // Зеленый
	DrawLine(img, { 54, 23 }, { 56, 23 }, 0x00FF00);  // Зеленый

	// Макушка (желтая звезда)
	FillCircle(img, { 55, 19 }, 1, 0xFFFF00);  // Желтый

	// Земля (светло-голубой снег)
	DrawLine(img, { 0, 47 }, { 76, 47 }, 0x87CEEB);  // Светло-голубой

	SaveImageToPPM(img, "image");
}
