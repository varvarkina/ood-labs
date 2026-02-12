#define CATCH_CONFIG_MAIN
#include "../../../catch2/catch.hpp"
#include "../TiledImage/tile.h"
#include "../TiledImage/image.cpp"
#include <sstream>

// Tile tests

TEST_CASE("Default create Tile")
{
	Tile tile;
	for (int y = 0; y < Tile::SIZE; ++y)
	{
		for (int x = 0; x < Tile::SIZE; ++x)
		{
			REQUIRE(tile.GetPixel({ x, y }) == ' ');
		}
	}
}

TEST_CASE("Create Tile with color")
{
	Tile tile('X');
	for (int y = 0; y < Tile::SIZE; ++y)
	{
		for (int x = 0; x < Tile::SIZE; ++x)
		{
			REQUIRE(tile.GetPixel({ x, y }) == 'X');
		}
	}
}

TEST_CASE("SetPixel success")
{
	Tile tile;

	tile.SetPixel({ 0, 0 }, 'A');
	tile.SetPixel({ 7, 7 }, 'B');

	REQUIRE(tile.GetPixel({ 0, 0 }) == 'A');
	REQUIRE(tile.GetPixel({ 7, 7 }) == 'B');
}

TEST_CASE("SetPixel ignores out of bounds")
{
	Tile tile('X');

	tile.SetPixel({ -1, 0 }, 'A');
	tile.SetPixel({ 0, -1 }, 'A');
	tile.SetPixel({ 8, 0 }, 'A');
	tile.SetPixel({ 0, 8 }, 'A');
	tile.SetPixel({ -1, -1 }, 'A');
	tile.SetPixel({ 8, 8 }, 'A');

	for (int y = 0; y < Tile::SIZE; ++y)
	{
		for (int x = 0; x < Tile::SIZE; ++x)
		{
			REQUIRE(tile.GetPixel({ x, y }) == 'X');
		}
	}
}

TEST_CASE("Tile::GetPixel returns space for out of bounds")
{
	Tile tile('X');

	REQUIRE(tile.GetPixel({ -1, 0 }) == ' ');
	REQUIRE(tile.GetPixel({ 0, -1 }) == ' ');
	REQUIRE(tile.GetPixel({ 8, 0 }) == ' ');
	REQUIRE(tile.GetPixel({ 0, 8 }) == ' ');
	REQUIRE(tile.GetPixel({ -1, -1 }) == ' ');
	REQUIRE(tile.GetPixel({ 8, 8 }) == ' ');
}

TEST_CASE("Tile Copy GetPixel")
{
	Tile original('O');
	original.SetPixel({ 2, 3 }, 'Z');
	original.SetPixel({ 5, 1 }, 'W');

	Tile copy = original;

	for (int y = 0; y < Tile::SIZE; ++y)
	{
		for (int x = 0; x < Tile::SIZE; ++x)
		{
			REQUIRE(copy.GetPixel({ x, y }) == original.GetPixel({ x, y }));
		}
	}

	copy.SetPixel({ 0, 0 }, 'C');
	REQUIRE(original.GetPixel({ 0, 0 }) == 'O');
	REQUIRE(copy.GetPixel({ 0, 0 }) == 'C');
}

TEST_CASE("Instance counting works")
{
	int initialCount = Tile::GetInstanceCount();

	{
		Tile tile1;
		REQUIRE(Tile::GetInstanceCount() == initialCount + 1);

		Tile tile2('X');
		REQUIRE(Tile::GetInstanceCount() == initialCount + 2);

		Tile tile3 = tile2;
		REQUIRE(Tile::GetInstanceCount() == initialCount + 3);
	}

	REQUIRE(Tile::GetInstanceCount() == initialCount);
}

//Image tests

TEST_CASE("Create Image with size and color")
{
	Image img({ 10, 5 }, 'X');
	REQUIRE(img.GetSize().width == 10);
	REQUIRE(img.GetSize().height == 5);
	REQUIRE(Tile::GetInstanceCount() == 1);

	for (int y = 0; y < 5; ++y)
	{
		for (int x = 0; x < 10; ++x)
		{
			REQUIRE(img.GetPixel({ x, y }) == 'X');
		}
	}
	REQUIRE(img.GetPixel({ 10, 5 }) == ' ');
	REQUIRE(img.GetPixel({ -1, -1 }) == ' ');
}

TEST_CASE("Create Image with exact tile size")
{
	Image img({ Tile::SIZE, Tile::SIZE }, 'X');
	REQUIRE(img.GetSize().width == Tile::SIZE);
	REQUIRE(img.GetSize().height == Tile::SIZE);
	REQUIRE(Tile::GetInstanceCount() == 1);

	for (int y = 0; y < 8; ++y)
	{
		for (int x = 0; x < 8; ++x)
		{
			REQUIRE(img.GetPixel({ x, y }) == 'X');
		}
	}
	REQUIRE(img.GetPixel({ 8, 8 }) == ' ');
	REQUIRE(img.GetPixel({ -1, -1 }) == ' ');
}

TEST_CASE("Constructor throws for invalid size")
{
	REQUIRE_THROWS_AS(Image({ -1, 5 }, 'X'), std::out_of_range);
	REQUIRE_THROWS_AS(Image({ 5, -1 }, 'X'), std::out_of_range);
	REQUIRE_THROWS_AS(Image({ 0, 5 }, 'X'), std::out_of_range);  
	REQUIRE_THROWS_AS(Image({ 5, 0 }, 'X'), std::out_of_range);
}

TEST_CASE("Image::GetPixel returns space for out of bounds")
{
	Image img({ 1, 1 }, 'X');

	REQUIRE(img.GetPixel({ -1, 0 }) == ' ');
	REQUIRE(img.GetPixel({ 0, -1 }) == ' ');
	REQUIRE(img.GetPixel({ 1, 0 }) == ' ');
	REQUIRE(img.GetPixel({ 0, 1 }) == ' ');
	REQUIRE(img.GetPixel({ -1, -1 }) == ' ');
	REQUIRE(img.GetPixel({ 1, 1 }) == ' ');
}

TEST_CASE("SetPixel does nothing for out of bounds")
{
	Image img({ 2, 2 }, 'X');

	img.SetPixel({ -1, 0 }, 'A');
	img.SetPixel({ 0, -1 }, 'A');
	img.SetPixel({ 2, 0 }, 'A');
	img.SetPixel({ 0, 2 }, 'A');
	img.SetPixel({ -1, -1 }, 'A');
	img.SetPixel({ 2, 2 }, 'A');

	for (int y = 0; y < 2; ++y)
	{
		for (int x = 0; x < 2; ++x)
		{
			REQUIRE(img.GetPixel({ x, y }) == 'X');
		}
	}

	REQUIRE(Tile::GetInstanceCount() == 1);
}

// Copy-on-Write 
TEST_CASE("SetPixel in one tile creates copy")
{
	Image img({ 16, 16 }, 'X');
	REQUIRE(Tile::GetInstanceCount() == 1);

	img.SetPixel({ 0, 0 }, 'A');
	img.SetPixel({ 7, 7 }, 'B');
	REQUIRE(img.GetPixel({ 0, 0 }) == 'A');
	REQUIRE(img.GetPixel({ 7, 7 }) == 'B');

	REQUIRE(Tile::GetInstanceCount() == 2);
}

TEST_CASE("SetPixel in two tiles creates two independent copies")
{
	Image img({ 16, 16 }, 'X');
	REQUIRE(Tile::GetInstanceCount() == 1);

	img.SetPixel({ 0, 0 }, 'A');
	img.SetPixel({ 7, 7 }, 'B');
	REQUIRE(Tile::GetInstanceCount() == 2);
	img.SetPixel({ 8, 8 }, 'C');
	img.SetPixel({ 15, 15 }, 'D');
	REQUIRE(Tile::GetInstanceCount() == 3);

	REQUIRE(img.GetPixel({ 0, 0 }) == 'A');
	REQUIRE(img.GetPixel({ 7, 7 }) == 'B');
	REQUIRE(img.GetPixel({ 8, 8 }) == 'C');
	REQUIRE(img.GetPixel({ 15, 15 }) == 'D');
}

TEST_CASE("SetPixel in all tiles: last modifies original tile")
{
	Image img({ 16, 16 }, 'X');
	REQUIRE(Tile::GetInstanceCount() == 1);

	img.SetPixel({ 0, 0 }, 'A');
	img.SetPixel({ 7, 7 }, 'B');
	REQUIRE(Tile::GetInstanceCount() == 2);
	img.SetPixel({ 8, 8 }, 'C');
	img.SetPixel({ 15, 15 }, 'D');
	REQUIRE(Tile::GetInstanceCount() == 3);
	img.SetPixel({ 8, 0 }, 'E');
	img.SetPixel({ 15, 7 }, 'F');
	REQUIRE(Tile::GetInstanceCount() == 4);
	img.SetPixel({ 0, 8 }, 'G');
	img.SetPixel({ 7, 15 }, 'H');
	REQUIRE(Tile::GetInstanceCount() == 4);

	REQUIRE(img.GetPixel({ 0, 0 }) == 'A');
	REQUIRE(img.GetPixel({ 7, 7 }) == 'B');
	REQUIRE(img.GetPixel({ 8, 8 }) == 'C');
	REQUIRE(img.GetPixel({ 15, 15 }) == 'D');
	REQUIRE(img.GetPixel({ 8, 0 }) == 'E');
	REQUIRE(img.GetPixel({ 15, 7 }) == 'F');
	REQUIRE(img.GetPixel({ 0, 8 }) == 'G');
	REQUIRE(img.GetPixel({ 7, 15 }) == 'H');
}

TEST_CASE("Print image")
{
	Image img({ 4, 4 }, 'X');
	img.SetPixel({ 0, 0 }, 'A');
	img.SetPixel({ 1, 1 }, 'B');
	img.SetPixel({ 2, 2 }, 'C');
	img.SetPixel({ 3, 3 }, 'D');

	std::stringstream out;

	Print(img, out);
	REQUIRE(out.str() == "AXXX\nXBXX\nXXCX\nXXXD\n");
}

TEST_CASE("Load image from string with different line lengths")
{
	auto img = LoadImage(
		"AXX\n"
		"XBXT\n");

	REQUIRE(img.GetSize().width == 4);
	REQUIRE(img.GetSize().height == 2);

	REQUIRE(img.GetPixel({ 0, 0 }) == 'A');
	REQUIRE(img.GetPixel({ 1, 0 }) == 'X');
	REQUIRE(img.GetPixel({ 2, 0 }) == 'X');
	REQUIRE(img.GetPixel({ 3, 0 }) == ' ');
	REQUIRE(img.GetPixel({ 0, 1 }) == 'X');
	REQUIRE(img.GetPixel({ 1, 1 }) == 'B');
	REQUIRE(img.GetPixel({ 2, 1 }) == 'X');
	REQUIRE(img.GetPixel({ 3, 1 }) == 'T');
}