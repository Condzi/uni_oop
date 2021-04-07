#include "rectangle.h"

Rectangle::Rectangle() : width{ 0 }, height{ 0 }
{}

Rectangle::Rectangle( double width_, double height_ ) :
	width{ width_ },
	height{ height_ }
{}

double Rectangle::area() const {
	return width*height;
}

Rectangle Rectangle::operator+( Rectangle const &other ) const {
	Rectangle result;
	result.width  = width + other.width;
	result.height = height + other.height;

	return result;
}

bool Rectangle::operator<( Rectangle const &other ) const {
	return area() < other.area();
}

bool Rectangle::operator>( Rectangle const &other ) const {
	return area() > other.area();
}

ostream& operator<<( ostream &s, Rectangle const &rect ) {
	s << "Width  = " << rect.width << std::endl;
	s << "Height = " << rect.height;

	return s;
}

istream& operator>>( istream &s, Rectangle &rect ) {
	s >> rect.width >> rect.height;

	return s;
}
