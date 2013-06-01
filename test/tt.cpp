#include <iostream>
int add(int x, int y) {return x+y;}
int hh()
{
	typedef int (*add_t) (int, int);
	static add_t a = add;
	return a(1,2);
}

int main()
{
	std::cout << hh() << std::endl;
	return 0;
}