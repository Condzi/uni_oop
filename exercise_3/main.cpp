#include "list.h"

int main() {
	List a{8}, b;
	a.PrintList();
	b.PrintList();

	List c( a );
	c.AddElement();
	c.AddElement();
	c.PrintList();
}