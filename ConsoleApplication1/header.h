#include <string.h>
#include <iostream>
#include <cstdint>
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;
class Pixel {
private:
	int8_t  R;
	int8_t  G;
	int8_t  B;
public:
	Pixel operator+(Pixel& other);
	bool operator==(Pixel& other);
	Pixel(int r = 0, int g = 0, int b = 0) : R(r), G(g), B(b) {}
	int8_t  GetR();
	int8_t  GetG();
	int8_t  GetB();
	void SetR(int8_t  r);
	void SetG(int8_t  g);
	void SetB(int8_t  b);
	int8_t  GetGray();
	void print();
	static int clamp(int value) {
		return std::max(0, std::min(255, value));
	}

};

class Image {
private:
	int width;
	int height;
	Pixel** pixels;
public:
	Image(int width, int height);
	Image(Image& other);
	~Image();
	int GetHeight();
	int GetWidth();
	Pixel GetPixel(int x, int y);
	void Resize(int NewW, int NewH);
	void SetPixel(int x, int y, Pixel pixel);
};

class Palette {
private:
	Pixel* pixels;
	int capacity;
	string name;
	static int paletteCount;
public:
	Palette(int InitCapaity = 10, const string& paletteName = "Default Palette");
	Palette(Palette& other);
	~Palette();
	void setName(string& paletteName);
	string getName();
	Palette& operator+=(Pixel newPixel);
	int GetCapacity();
	void Clear();
	Pixel* GetPixelByPointer(int index);
	Pixel& GetPixelByReference(int index);
	friend void printPalette(Palette& palette);
	void print();
	static int getPaletteCount() {
		return paletteCount;
	}

};
// Дружественная функция для печати палитры
void printPalette(Palette& palette);

class Filter {
public:
	virtual void apply(Image& image) {
		std::cout << "Applying base filter\n";
	}
	void baseApply(Image& image) {
		apply(image);
	}
};

class BlackAndWhiteFilter: public Filter {
public:
	BlackAndWhiteFilter() : Filter() {
		std::cout << "BlackAndWhiteFilter constructor called." << std::endl;
	}
	Image Apply(Image image);
};

class BrightnessFilter : public Filter {
private:
	int brightnessLevel;

public:
	BrightnessFilter(int level) : brightnessLevel(level) {}

	void apply(Image& image) override {
		std::cout << "Applying brightness filter with level: " << brightnessLevel << "\n";
	}
};


class PhotoEditor {
private:
	Image image;
	Filter** filters;
	int FilterCount;
public:
	void ApplyFilter(Filter* filter);
	void ShowImageInfo();
};

class Gradient {
private:
	Pixel StartColor;
	Pixel EndColor;
public:
	Gradient(Pixel start, Pixel end);
	Image Apply(Image image);

};

