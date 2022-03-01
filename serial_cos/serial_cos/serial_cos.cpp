#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <time.h>
#include <cstdio>

#define PI 3.14159265358979323846  /* pi */
using namespace std;
long double factorial(int num);

int main()
{
	int k = 0;
	long double sum = 0;
	float x = 0;
	long double num, fraction;

	printf("Enter k and x (in degree)\n");
	fflush(stdout);
	scanf_s("%d %fl", &k, &x);
	clock_t time_start = clock();
	x = (x * PI) / 180.0;
	for (int i = 0; i < k; i++)
	{
		fraction = (long double)(pow(-1, i) * pow(x, 2 * i)) / factorial(2 * i);
		sum += fraction;
	}

	cout << std::fixed << setprecision(10) << sum << "\n";
	printf("The execution time is %.6fs\n", (double)(clock() - time_start) / CLOCKS_PER_SEC);

	cout << "Press any key to exist (dont press enter)";
	cin >> x;
	return 0;
}

long double factorial(int num)
{
	long double f = 1.0;

	for (int i = 1; i <= num; i++) {
		f = f * i;
	}
	return f;
}