#pragma once
#define OS12HANDLE void*

namespace OS12 {
	OS12HANDLE Init();

	namespace Adder {
		double Add(OS12HANDLE h, double x, double y);
		double Sub(OS12HANDLE h, double x, double y);
	}
	namespace Multiplier {
		double Mul(OS12HANDLE h, double x, double y);
		double Div(OS12HANDLE h, double x, double y);
	}

	void Dispose(OS12HANDLE h);
}