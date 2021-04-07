#include "rectangle.h"

using std::cout;
using std::cin;

int main() {
	Rectangle a, b;

	cout << "Type width and height of the first rectangle.\n";
	cin >> a;
	cout << "Type width and height of the second rectangle.\n";
	cin >> b;

	if( a > b ) {
		cout << "Area of the first rectangle is bigger.\n";
	} else if ( a < b ) {
		cout << "Area of the second rectangle is bigger.\n";
	} else {
		cout << "Rectangles have identical areas.\n";
	}

	cout << "a + b results in: \n";
	cout << a + b << "\n";
}