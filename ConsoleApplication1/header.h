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
	// ������������� �����������
	Image(Image& other) : width(other.width), height(other.height), pixels(other.pixels) {
		// ����� ���������� ������ ��������� �� ������, ��� �������� ������ �������
	}

	/**************�������� �����������
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
// ������������� ������� ��� ������ �������
void printPalette(Palette& palette);

class Filter {
protected:
	std::string name;
public:
	// ����������� �������� ������
	Filter(const std::string& filterName) : name(filterName) {
		std::cout << "Filter base class constructor called: " << name << std::endl;
	}
	virtual ~Filter() {
		std::cout << "Base Filter destructor called.\n";
	}
	Filter(const Filter&) = delete;
	Filter& operator=(Filter& other) {
		if (this != &other) {
			name = other.name;
		}
		std::cout << "Base Filter assignment operator called.\n";
		return *this;
	}

	virtual void apply() {
		std::cout << "Applying base filter: " << name << std::endl;
	}
};

class BlackAndWhiteFilter: public Filter {
};

class BrightnessFilter : public Filter {
private:
	int brightnessLevel;

public:
	// ����������� ������������ ������
	BrightnessFilter(const std::string& filterName, int level)
		: Filter(filterName), brightnessLevel(level) {
		std::cout << "BrightnessFilter derived class constructor called with level: "
			<< brightnessLevel << std::endl;
	}

	void apply(){
		std::cout << "Applying brightness filter: " << name
			<< " with level " << brightnessLevel << std::endl;
	}
	BrightnessFilter& operator=(Filter& base) {
		if (this != &base) {
			// ����� ��������� ������������ �������� ������
			Filter::operator=(base);

			// ������ ��� ������������ ������
			brightnessLevel = 0; // �������� ������� �������
		}
		std::cout << "Derived BrightnessFilter assignment operator called.\n";
		return *this;
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

