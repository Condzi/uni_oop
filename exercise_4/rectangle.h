#pragma once

#include <iostream>

using std::ostream;
using std::istream;


class Rectangle final {
public:
	Rectangle();
	Rectangle( double width_, double height_ );

	double area() const;

	Rectangle operator+( Rectangle const& other ) const;
	bool operator<( Rectangle const& other ) const;
	bool operator>( Rectangle const& other ) const;

	friend ostream& operator<<( ostream& s, Rectangle const& rect );
	friend istream& operator>>( istream& s, Rectangle& rect );

private:
	double width, height;
};