#include "header.h"
#include <iostream>
#include <cstdint>
using namespace std;


//работа с ПАЛИТРОЙ
Palette::Palette(int InitialCapacity, const std::string& paletteName) : capacity(InitialCapacity), name(paletteName) {
	paletteCount++;
	pixels = new Pixel[capacity];
}

Palette::~Palette() {
	paletteCount--;
	delete[] pixels;
}
void Palette::setName(string& paletteName) {
	name = paletteName;
}
string Palette::getName(){
	return name;
}

int Palette::GetCapacity() {
	return capacity;
}

void Palette::Clear() {
	capacity = 0;
}
Pixel* Palette::GetPixelByPointer(int index) {
	if (index >= 0 && index < capacity) {
		return &pixels[index];
	}
	else {
		return nullptr;
	}
}
// Дружественная функция для печати палитры
void printPalette(Palette& palette) {
	std::cout << "Palette contains " << palette.capacity << " pixels:" << std::endl;
	for (int i = 0; i < palette.capacity; ++i) {
		std::cout << "Pixel " << i << ": ";
		palette.pixels[i].print();
	}
}

Pixel& Palette:: GetPixelByReference(int index) {
	if (index >= 0 && index < capacity) {
		return pixels[index];
	}
	else {
		return pixels[0];
	}
}
Palette& Palette::operator+=(Pixel newPixel) {
	pixels[capacity++] = newPixel;
	return *this;
}
void Palette::print(){
	std::cout << "Palette contains " << capacity << " pixels:" << std::endl;
	for (int i = 0; i < capacity; ++i) {
		std::cout << "Pixel " << i << ": ";
		pixels[i].print();
	}
}

Palette::Palette(Palette& other) : capacity(other.capacity) {
	pixels = new Pixel[capacity];
	for (int i = 0; i < capacity; ++i) {
		pixels[i] = other.pixels[i];
	}
}
//работа с ИЗОБРАЖЕНИЕМ
Image::Image(int w, int h) : width(w), height(h) {
	pixels = new Pixel* [height];
	for (int i = 0; i < height; i++) {
		pixels[i] = new Pixel[width];
	}
}
Image::~Image() {
	for (int i = 0; i < height; i++) {
		delete[] pixels[i];
	}
	delete[] pixels;
}

int Image::GetHeight() {
	return height;
}

int Image::GetWidth() {
	return width;
}

Pixel Image::GetPixel(int x, int y) {
	if (x < 0 || x >= width || y < 0 || y >= height) {
		throw std::out_of_range("Pixel coordinates out of bounds");
	}
	return pixels[y][x];
}
void Image::SetPixel(int x, int y, Pixel pixel) {
	if (x >= 0 && x < width && y >= 0 && y < height) {
		pixels[y][x] = pixel;
	}
}

Image::Image(Image& other) : width(other.width), height(other.height) {
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			pixels[i][j] = other.pixels[i][j];
		}
	}
}



void Image::Resize(int newWidth, int newHeight) {
	Pixel** newPixels = new Pixel * [newHeight];
	for (int i = 0; i < newHeight; i++) {
		newPixels[i] = new Pixel[newWidth];
	}

	int minWidth = (newWidth < width) ? newWidth : width;
	int minHeight = (newHeight < height) ? newHeight : height;

	for (int y = 0; y < minHeight; y++) {
		for (int x = 0; x < minWidth; x++) {
			newPixels[y][x] = pixels[y][x];
		}
	}

	for (int i = 0; i < height; i++) {
		delete[] pixels[i];
	}
	delete[] pixels;

	pixels = newPixels;
	width = newWidth;
	height = newHeight;
}

//Работа с классом Pixel

int8_t  Pixel::GetR() { return R; }
int8_t  Pixel::GetG() { return G; }
int8_t  Pixel::GetB() { return B; }

void Pixel::SetR(int8_t r) { R = r; }
void Pixel::SetG(int8_t g) { G = g; }
void Pixel::SetB(int8_t b) { B = b; }

int8_t Pixel::GetGray() {
	return (R + G + B) / 3;
}
void Pixel::print() {
	std::cout << "Pixel(R: " << static_cast<int>(R)
		<< ", G: " << static_cast<int>(G)
		<< ", B: " << static_cast<int>(B) << ")" << std::endl;
}
Pixel Pixel:: operator+(Pixel& other) {
	return Pixel(R + other.R, G + other.G, B + other.B);
}
bool Pixel:: operator==(Pixel& other){
	return R == other.R && G == other.G && B == other.B;
}

//работа с фильтрами
Image BlackAndWhiteFilter::Apply(Image image) {
	for (int y = 0; y < image.GetHeight(); y++) {
		for (int x = 0; x < image.GetWidth(); x++) {
			Pixel pixel = image.GetPixel(x, y);
			int gray = pixel.GetGray();
			Pixel grayPixel(gray, gray, gray);
			image.SetPixel(x, y, grayPixel);
		}
	}
	return image;
}


//методы класса PhotoEditor
void PhotoEditor::ShowImageInfo() {
	std::cout << "Image width: " << image.GetWidth() << ", height: " << image.GetHeight() << std::endl;
}


//методы класса Gradient
Gradient::Gradient(Pixel start, Pixel end) :StartColor(start), EndColor(end) {}
Image Gradient::Apply(Image image) {
		int width = image.GetWidth();
		int height = image.GetHeight();
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				// Вычисляем отношение текущей позиции x к ширине изображения
				float ratio = static_cast<float>(x) / (width - 1);

				// Вычисляем цвета пикселей на основе отношения
				int red = StartColor.GetR() + ratio * (EndColor.GetR() - StartColor.GetR());
				int green = StartColor.GetG() + ratio * (EndColor.GetG() - StartColor.GetG());
				int blue = StartColor.GetB() + ratio * (EndColor.GetB() - StartColor.GetB());
				Pixel newpixel(red, green, blue);

				image.SetPixel(x, y, newpixel);
			}
		}
		return image;
}
int Palette::paletteCount = 0;



int main()
{
	// Создаем изображение
	Image image(3, 3);
	image.SetPixel(0, 0, Pixel(100, 150, 200));
	image.SetPixel(1, 0, Pixel(120, 170, 220));
	image.SetPixel(2, 0, Pixel(140, 190, 240));

	// Используем базовый класс
	Filter baseFilter;
	baseFilter.baseApply(image);
	// Используем дочерний класс BrightnessFilter через базовый указатель
	Filter* brightnessFilter = new BrightnessFilter(50);
	brightnessFilter->baseApply(image);  // Вызов через базовый класс
	delete brightnessFilter;

	// Пример работы с не виртуальным методом (если apply не виртуальный)
	Filter* simpleFilter = new Filter();
	simpleFilter->apply(image); // Всегда вызов базового метода
	delete simpleFilter;

	return 0;
}
