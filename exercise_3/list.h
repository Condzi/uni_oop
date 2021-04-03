#pragma once

class List final {
public:
	List();
	List( int sz );
	List( List const& copy );
	~List();
	
	bool IsEmpty() const;
	// Returns `false` on memory allocation failure.
	bool AddElement();
	void PrintList() const;

private:
	int size;
	double* begin;
};