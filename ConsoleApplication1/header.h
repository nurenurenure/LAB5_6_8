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
	void display() {
		std::cout << "(" << R << ", " << G << ", " << B << ")";
	}

};

class Image {
private:
	int width;
	int height;
	Pixel** pixels;
public:
	Image(int width, int height);
	~Image();
	int GetHeight();
	int GetWidth();
	Pixel GetPixel(int x, int y);
	void Resize(int NewW, int NewH);
	void SetPixel(int x, int y, Pixel pixel);
	// Поверхностное копирование
	Image(Image& other) : width(other.width), height(other.height), pixels(other.pixels) {
		// Здесь копируется только указатель на массив, без создания нового массива
	}

	/**************Глубокое копирование
	Image(Image& other) : width(other.width), height(other.height) {
		pixels = new Pixel * [width];
		for (int i = 0; i < width; ++i) {
			pixels[i] = new Pixel[height];
			for (int j = 0; j < height; ++j) {
				pixels[i][j] = other.pixels[i][j];
			}
		}
	}
	*******/
	void display() {
		for (int i = 0; i < width; ++i) {
			for (int j = 0; j < height; ++j) {
				pixels[i][j].display();
				std::cout << " ";
			}
			std::cout << std::endl;
		}
	}
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
	virtual void apply() const {
		std::cout << "Base Filter applied\n";
	}
	virtual ~Filter() = default;
};

class BlackAndWhiteFilter: public Filter {
};

class BrightnessFilter : public Filter {
private:
	int level;

public:
	BrightnessFilter(int lvl) : level(lvl) {}
	void apply(){
		std::cout << "Brightness Filter applied with level " << level << "\n";
	}
	int getLevel(){
		return level;
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

