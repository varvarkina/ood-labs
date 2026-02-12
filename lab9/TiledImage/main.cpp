#include <cmath>
#include <iostream>

#include "drawer.h"

int main()
{
        Image img({ 100, 50 }, ' ');
        // Снеговик снизу вверх
        FillCircle(img, { 15, 38 }, 8, 'O');
        FillCircle(img, { 15, 27 }, 6, 'O');
        FillCircle(img, { 15, 18 }, 4, 'O');

        // Шляпа
        DrawLine(img, { 12, 14 }, { 18, 14 }, '-');
        DrawLine(img, { 13, 12 }, { 13, 14 }, '|');
        DrawLine(img, { 17, 12 }, { 17, 14 }, '|');
        DrawLine(img, { 13, 12 }, { 17, 12 }, '-');

        //солнце
        FillCircle(img, { 70, 6 }, 4, '#');

        // снежинки 
        img.SetPixel({ 20, 3 }, '*');
        img.SetPixel({ 50, 2 }, '*');
        img.SetPixel({ 72, 25 }, '*');
        img.SetPixel({ 75, 15 }, '*');
        img.SetPixel({ 35, 20 }, '*');
        img.SetPixel({ 45, 15 }, '*');
        img.SetPixel({ 50, 10 }, '*');

        // Ствол
        DrawLine(img, { 55, 41 }, { 55, 46 }, '|');

        // Ярусы ели
        DrawLine(img, { 39, 38 }, { 71, 38 }, '-');
        DrawLine(img, { 42, 35 }, { 68, 35 }, '-');
        DrawLine(img, { 45, 32 }, { 65, 32 }, '-');  
        DrawLine(img, { 48, 29 }, { 62, 29 }, '-');  
        DrawLine(img, { 51, 26 }, { 59, 26 }, '-');  
        DrawLine(img, { 54, 23 }, { 56, 23 }, '-');  
        // Макушка
        FillCircle(img, { 55, 19 }, 1, '*');

        // Земля
        DrawLine(img, { 0, 47 }, { 76, 47 }, '~');

        Print(img, std::cout);
}
