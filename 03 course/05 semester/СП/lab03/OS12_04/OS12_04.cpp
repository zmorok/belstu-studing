#include <iostream>
#include <string>
#include "OS12_Lib.h"

int main() {

	try {
		OS12HANDLE h1 = OS12::Init();
		OS12HANDLE h2 = OS12::Init();

		std::cout << "OS12::Adder::Add(h1,2,3) = " << OS12::Adder::Add(h1, 2, 3) << std::endl;
		std::cout << "OS12::Adder::Add(h2,2,3) = " << OS12::Adder::Add(h2, 2, 3) << std::endl;

		std::cout << "OS12::Adder::Sub(h1,2,3) = " << OS12::Adder::Sub(h1, 2, 3) << std::endl;
		std::cout << "OS12::Adder::Sub(h2,2,3) = " << OS12::Adder::Sub(h2, 2, 3) << std::endl;

		std::cout << "OS12::Multiplier::Mul(h1,2,3) = " << OS12::Multiplier::Mul(h1, 2, 3) << std::endl;
		std::cout << "OS12::Multiplier::Mul(h2,2,3) = " << OS12::Multiplier::Mul(h2, 2, 3) << std::endl;

		std::cout << "OS12::Multiplier::Div(h1,2,3) = " << OS12::Multiplier::Div(h1, 2, 3) << std::endl;
		std::cout << "OS12::Multiplier::Div(h2,2,3) = " << OS12::Multiplier::Div(h2, 2, 3) << std::endl;

		OS12::Dispose(h1);
		OS12::Dispose(h2);

	}
	catch (const std::exception& e) {
		std::cerr << "OS12: error: " << e.what() << std::endl;
	}

	return 0;
}