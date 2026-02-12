#pragma once
#include <iostream>
using namespace std;

// Пространство имен графической библиотеки (недоступно для изменения)
namespace graphics_lib
{
    // Холст для рисования
    class ICanvas
    {
    public:
        // Ставит "перо" в точку x, y
        virtual void MoveTo(int x, int y) = 0;
        // Рисует линию с текущей позиции, передвигая перо в точку x,y
        virtual void LineTo(int x, int y) = 0;
        virtual ~ICanvas() = default;
    };

    // Реализация холста для рисования
    class CCanvas : public ICanvas
    {
    public:
        void MoveTo(int x, int y) override
        {
            cout << "MoveTo (" << x << ", " << y << ")" << endl;
        }
        void LineTo(int x, int y) override
        {
            cout << "LineTo (" << x << ", " << y << ")" << endl;
        }
    };
}