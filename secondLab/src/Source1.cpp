#include <iostream>
#include <cmath>
#include <iomanip>
#include <string>
using namespace std;


double calculateNElem(long long n) {
	return pow(-1, n) / (2 * n + 1);
}


double calculatePi(long long n) {
	double pi_num = 0.0;
	for (long long i = 0; i < n; i++) {
		pi_num += calculateNElem(i);
	}
	return 4.0 * pi_num;
}


int main(int argc, char* argv[]) {
	size_t n = stoull(argv[1]);
	cout << setprecision(15) << calculatePi(n) << endl;
	return 0;
}