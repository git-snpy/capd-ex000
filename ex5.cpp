/*
 * ex5.cpp
 * Numerical verification of periodic orbit
 * by shooting method
 */
#include <iostream>
#include <capd/capdlib.h>

using namespace capd;
using namespace std;

int main(void)
{
	cout.precision(15);
	try {
		int order = 20;	// order of Taylor expansion
		// vector field for "scaled" Lorenz system
		IMap vectorField("var:x,y,z,T;par:r,s,b;"
					"fun:T*s*(y-x),T*(-y+x*(r-z)),T*(x*y-b*z),0;");

		// set parameter values for the Lorenz system
		vectorField.setParameter("r", interval(18.0));
		vectorField.setParameter("s", interval(10.0));
		vectorField.setParameter("b", interval(8.0) / 3.0);

		// solver for Lorenz system
		// the argument "0.1" is the initial step size
		// but it does not mean
		// because the step size is automatically controlled
		ITaylor solver(vectorField, order, 0.1);

		ITimeMap timeMap(solver);

		// initial point close to a periodic orbit
		IVector x(4);
		x[0] = interval("11.0265756760119",
						"11.0265756760119");
		x[1] = interval("14.5162669781979",
						"14.5162669781979");
		x[2] = interval(17.0);
		x[3] = interval("1.0357509658742212",
						"1.0357509658742212");

		// compute the image of the mid point
		C0Rect2Set s0(x);
		IVector F = timeMap(1.0, s0);
		// f <- (phi(x) - x, x_2 - (r-1) = 0)
		F -= x;
		F[3] = x[2] - 17.0;

		// an interval enclosing the fixed point (we expect)
		IVector X(4);
		X = x + interval("-1.0E-10", "1.0E-10");

		// preparation for computing Jacovian
		IEuclLNorm Nr;
		C1Rect2Set s1(X, Nr);	// doubleton representation of x
		IMatrix M(4,4);
		// M will be overwritten by the monodromy Matrix
		timeMap(1.0, s1, M);
		M -= IMatrix::Identity(4);
		M[3][0] = 0;
		M[3][1] = 0;
		M[3][2] = 1;
		M[3][3] = 0;

		// interval Newton operator
		IVector N = x - matrixAlgorithms::gauss(M, F);

		// Test if N is in X
		if(subset(N, X)) {
			cout << "A periodic orbit exists" << endl;
		} else {
			cout << "Verification failed" << endl;
		}

		cout << "X = " << X << endl;
		cout << "N = " << N << endl;

	} catch(exception &e) {
		cout << "\n\nException caught!\n" << e.what() << endl << endl;
	}
	return 0;
}



