#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;


double calculateNElem(long long n) {
	return pow(-1, n) / (2 * n + 1);
}


double calculatePi(long long n) {
	double pi_num = 0.0;
	for (int i = 0; i < n; i++) {
		pi_num += calculateNElem(i);
	}
	return 4.0 * pi_num;
}


int main() {
	long long n = 2147455400;
	cout << setprecision(15) << calculatePi(n) << endl;
	return 0;
}