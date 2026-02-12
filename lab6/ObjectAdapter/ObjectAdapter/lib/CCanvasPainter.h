#pragma once
#include "../app/ModernGraphicsAdapter.h"
#include "./graphics_lib.h"
#include "./modern_graphics_lib.h"
#include "./shape_drawing_lib.h"

// Пространство имен приложения (доступно для модификации)
namespace app
{

	void PaintPicture(shape_drawing_lib::CCanvasPainter& painter)
	{
		using namespace shape_drawing_lib;
		CTriangle triangle({ 10, 15 }, { 100, 200 }, { 150, 250 });
		CRectangle rectangle({ 30, 40 }, 18, 24);

		painter.Draw(rectangle);
		painter.Draw(triangle);
	}

	void PaintPictureOnCanvas()
	{
		graphics_lib::CCanvas simpleCanvas;
		shape_drawing_lib::CCanvasPainter painter(simpleCanvas);
		PaintPicture(painter);
	}

	void PaintPictureOnModernGraphicsRenderer()
	{
		modern_graphics_lib::CModernGraphicsRenderer renderer(std::cout);
		ModernGraphicsAdapter modernCanvas(renderer);
		shape_drawing_lib::CCanvasPainter painter(modernCanvas);

		modernCanvas.BeginDraw();
		PaintPicture(painter);
		modernCanvas.EndDraw();
	}
}