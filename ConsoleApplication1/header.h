#include <string.h>
#include <cstdint>
class Pixel {
private:
	int8_t  R;
	int8_t  G;
	int8_t  B;
public:
	Pixel operator+(Pixel& other);
	bool operator==(Pixel& other);
	Pixel(int8_t  r = 0, int8_t  g = 0, int8_t  b = 0);
	int8_t  GetR();
	int8_t  GetG();
	int8_t  GetB();
	void SetR(int8_t  r);
	void SetG(int8_t  g);
	void SetB(int8_t  b);
	int8_t  GetGray();
	void print();
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
	void SetPixel(int x, int y, Pixel& pixel);
};

class Palette {
private:
	Pixel* pixels;
	int capacity;
public:
	Palette(int InitCapaity = 10);
	Palette(Palette& other);
	~Palette();
	Palette& operator+=(Pixel newPixel);
	int GetCapacity();
	void Clear();
	Pixel* GetPixelByPointer(int index);
	Pixel& GetPixelByReference(int index);
	friend void printPalette(Palette& palette);
	void print();

};
// Дружественная функция для печати палитры
void printPalette(Palette& palette);

class Filter {
public:
	Image Apply(Image image);
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
	int brightness;

public:
	BrightnessFilter(int level);
	Image Apply(Image image);
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

