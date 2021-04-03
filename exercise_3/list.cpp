#include <algorithm>
#include <iostream>

#include "list.h"

List::List() :size(0), begin( nullptr ) 
{}

List::List( int sz ) {
	size = sz;
	begin = new double[sz];

	std::fill( begin, begin+size, 1.0 );
}

List::List( List const &copy ) {
	if( copy.IsEmpty() ) {
		size = 0;
		begin = nullptr;
		return;
	}

	size = copy.size;
	begin = new double[size];

	std::copy( copy.begin, copy.begin + size, begin );
}

List::~List() {
	delete[] begin;
}

bool List::IsEmpty() const {
	return size == 0;
}

bool List::AddElement() {
	// Try to expand our array.
	try {
		auto* new_memory = new double[size+1];
		std::copy( begin, begin + size, new_memory );
		new_memory[size] = 1.0;
		size++;
		delete[] begin;
		begin = new_memory;
	} catch( std::bad_alloc const& ex ) {
		std::cout << "!!! Error: Bad alloc!\n";
		return false;
	}

	return true;
}

void List::PrintList() const {
	if( IsEmpty() ) {
		std::cout << "The List is empty.\n";
	} else {
		for( int i = 0; i < size; i++ ) {
			std::cout << "[" << i << "]: " << begin[i] << std::endl;
		}
	}
}


