﻿#include "header.h"
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
void Image::SetPixel(int x, int y, Pixel& pixel) {
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
Pixel::Pixel(int8_t  r, int8_t  g, int8_t  b) : R(r), G(g), B(b) {}

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

BrightnessFilter::BrightnessFilter(int level) : brightness(level) {}

Image BrightnessFilter::Apply(Image image) {
	for (int y = 0; y < image.GetHeight(); y++) {
		for (int x = 0; x < image.GetWidth(); x++) {
			Pixel pixel = image.GetPixel(x, y);

			int r = std::max(0, std::min(255, pixel.GetR() + brightness));
			int g = std::max(0, std::min(255, pixel.GetG() + brightness));
			int b = std::max(0, std::min(255, pixel.GetB() + brightness));
			Pixel newpixel(r, g, b);
			image.SetPixel(x, y, newpixel);
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
	Palette palette(5);  
	// указатель
	Pixel* pixelPtr = palette.GetPixelByPointer(2);
	pixelPtr->SetR(5);
	pixelPtr->SetG(5);
	pixelPtr->SetB(5);
	cout << "Pixel by pointer: ";
	pixelPtr->print();
	// ссылка
	Pixel& pixelRef = palette.GetPixelByReference(3);
	pixelRef.SetR(50);
	pixelRef.SetG(75);
	pixelRef.SetB(125);
	cout << "Pixel (REFERENCE): ";
	pixelRef.print();

	Palette palette2(3);  // Создаем палитру из 3 пикселей

	// Изменим значения некоторых пикселей
	Pixel* pixelPtr1 = palette2.GetPixelByPointer(0);
	if (pixelPtr1) {
		pixelPtr1->SetR(50);
		pixelPtr1->SetG(100);
		pixelPtr1->SetB(100);
	}

	Pixel* pixelPtr2 = palette2.GetPixelByPointer(1);
	if (pixelPtr2) {
		pixelPtr2->SetR(14);
		pixelPtr2->SetG(18);
		pixelPtr2->SetB(56);
	}

	// Используем дружественную функцию для вывода всей палитры
	printPalette(palette2);

	//==
	Pixel p1(10, 20, 30);
	Pixel p2(10, 20, 30);
	Pixel p3(40, 50, 60);

	if (p1 == p2) {
		std::cout << "p1 and p2 are equal" << std::endl;
	}
	else {
		std::cout << "p1 and p2 are not equal" << std::endl;
	}

	if (p1 == p3) {
		std::cout << "p1 and p3 are equal" << std::endl;
	}
	else {
		std::cout << "p1 and p3 are not equal" << std::endl;
	}
	//+
	Pixel p4(10, 20, 30);
	Pixel p5(40, 50, 60);
	Pixel p6 = p4 + p5;
	p6.print();
	//+=
	Palette palette3(0);

	palette3 += Pixel(10, 20, 30);
	palette3 += Pixel(40, 50, 60);
	palette3 += Pixel(70, 80, 90);

	palette3.print();

	Palette palette_4(3);

	Palette palette_5(2);

	std::cout << "Current palette count: " << Palette::getPaletteCount() << "\n";

	try {
		Image image10(3, 3); // Создаем изображение 3x3
		Pixel redPixel(255, 0, 0);

		// Установка пикселя в пределах изображения
		image10.SetPixel(1, 1, redPixel);
		std::cout << "Pixel set successfully at (1, 1)\n";

		// Попытка установить пиксель за пределами изображения
		image10.SetPixel(6, 6, redPixel);
	}
	catch (const std::out_of_range& e) {
		std::cout << "Pixel out of size\n";
	}

	//работа со строками
	Palette palette6(5, "My Favorite colors");
	string string1 = palette6.getName();
	//Конкатенация строки
	string text = "hello";
	string1 += text;
	std::cout << "Updated string1: " << string1 << "\n";
	//Выделение подстроки
	std::cout << "Updated string1: " << string1.substr(0, 3) << "\n";

	// Используем std::shared_ptr
	{
		std::shared_ptr<Pixel> sharedPixel1 = std::make_shared<Pixel>(0, 0, 0);
		{
			std::shared_ptr<Pixel> sharedPixel2 = sharedPixel1; // Совместное владение
			std::cout << "Inside nested block:\n";
			sharedPixel2->print();
		} // sharedPixel2 уничтожается, но sharedPixel1 еще владеет объектом
		std::cout << "Outside nested block:\n";
		sharedPixel1->print();
	} // sharedPixel1 уничтожается, объект удаляется

	std::cout << "==========================\n";
	{
		std::unique_ptr<Pixel> uniquePixel = std::make_unique<Pixel>(0, 0, 0);
		uniquePixel->print();

		// Передача владения через std::move
		std::unique_ptr<Pixel> anotherUniquePixel = std::move(uniquePixel);
		if (!uniquePixel) {
			std::cout << "uniquePixel is now null after ownership transfer.\n";
		}
		anotherUniquePixel->print();
	} // anotherUniquePixel уничтожается, объект удаляется
}
