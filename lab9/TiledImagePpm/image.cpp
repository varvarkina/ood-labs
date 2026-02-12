#include "image.h"
#include <cassert>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <vector>

/**
 * Конструирует изображение заданного размера. Если размеры не являются положительными,
 * выбрасывает исключение std::out_of_range.
 */
Image::Image(Size size, uint32_t color)
	: m_size(size)
{
	if (size.width <= 0 || size.height <= 0)
	{
		throw std::out_of_range("Invalid image size");
	}
		
	size_t heightTilesCount = (size.height + Tile::SIZE - 1) / Tile::SIZE;
	size_t widthTilesCount = (size.width + Tile::SIZE - 1) / Tile::SIZE;

	m_tiles = std::vector(heightTilesCount, std::vector(widthTilesCount, CoW(Tile(color))));
	/* Реализуйте конструктор самостоятельно */
}

// Возвращает размер изображения в пикселях.
Size Image::GetSize() const noexcept
{
	/* Реализуйте метод самостоятельно. */
	return m_size;
}

/**
 * Возвращает «цвет» пикселя в указанных координатах.Если координаты выходят за пределы
 * изображения, возвращает «пробел».
 */
uint32_t Image::GetPixel(Point p) const noexcept
{
	/* Реализуйте метод самостоятельно. */
	if (!IsPointInSize(p, m_size))
	{
		return 0;
	}

	int tileX = p.x / Tile::SIZE;
	int tileY = p.y / Tile::SIZE;
	int pixelX = p.x % Tile::SIZE;
	int pixelY = p.y % Tile::SIZE;

	return m_tiles[tileY][tileX]->GetPixel({ pixelX, pixelY });
}

/**
 * Задаёт «цвет» пикселя в указанных координатах. Если координаты выходят за пределы изображения
 * действие игнорируется.
 */
void Image::SetPixel(Point p, uint32_t color)
{
	/* Реализуйте метод самостоятельно. */
	if (IsPointInSize(p, m_size))
	{
		const int tileX = p.x / Tile::SIZE;
		const int tileY = p.y / Tile::SIZE;
		const int pixelX = p.x % Tile::SIZE;
		const int pixelY = p.y % Tile::SIZE;
		m_tiles[tileY][tileX].Write()->SetPixel({ pixelX, pixelY }, color);
	}
}

/**
 * Выводит в поток out изображение в виде символов.
 */
void Print(const Image& img, std::ostream& out)
{
	const auto size = img.GetSize();
	for (int y = 0; y < size.height; ++y)
	{
		for (int x = 0; x < size.width; ++x)
		{
			out.put(img.GetPixel({ x, y }));
		}
		out.put('\n');
	}
}

/**
 * Загружает изображение из pixels. Линии изображения разделяются символами \n.
 * Размеры картинки определяются по количеству переводов строки и самой длинной линии.
 */
Image LoadImage(const std::string& pixels)
{
	std::istringstream s(pixels);
	Size size;
	std::string line;
	while (std::getline(s, line))
	{
		size.width = std::max(size.width, static_cast<int>(line.length()));
		++size.height;
	}

	Image img(size);

	s = std::istringstream(pixels);
	for (int y = 0; y < size.height; ++y)
	{
		if (!std::getline(s, line))
		{ 
			break;
		}

		int x = 0;
		for (char ch : line)
		{
			img.SetPixel({ x++, y }, ch);
		}
	}

	return img;
}
