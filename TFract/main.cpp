#include <iostream>
#include "TNatural.h"
#include "TInteger.h"
#include "TFract.h"

int main() {
	TFract<10> frac1("-5", "10");

	std::cout << frac1*2 << std::endl;

	return 0;
}
