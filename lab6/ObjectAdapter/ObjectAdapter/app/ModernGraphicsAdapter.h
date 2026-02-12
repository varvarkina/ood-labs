#pragma once
#include "../lib/graphics_lib.h"
#include "../lib/modern_graphics_lib.h"

class ModernGraphicsAdapter : public graphics_lib::ICanvas
{
public:
	ModernGraphicsAdapter(
		modern_graphics_lib::CModernGraphicsRenderer& modernRenderer)
		: m_start(0, 0)
		, m_renderer(modernRenderer)
	{
	}

	void MoveTo(int x, int y) override
	{
		m_start.x = x;
		m_start.y = y;
	}

	void LineTo(int x, int y) override
	{
		modern_graphics_lib::CPoint end(x, y);

		m_renderer.DrawLine(m_start, end);

		MoveTo(x, y);
	}

	void BeginDraw()
	{
		m_renderer.BeginDraw();
	}

	void EndDraw()
	{
		m_renderer.EndDraw();
	}

private:
	modern_graphics_lib::CPoint m_start;
	modern_graphics_lib::CModernGraphicsRenderer& m_renderer;
};