#include<stdio.h>
#include<mpi.h> 
#include<stdlib.h> 
#include<math.h>

#define PI 3.14159265358979323846  /* pi */

long double factorial(int num);

int main(int argc, char* argv[])
{
	int myRank = 0, commSize = 0;
	int n = 0, k = 0, x = 0;
	int division = 0, local_sum_start = 0, local_sum_end = 0;
	long double local_sum = 0, total_sum = 0;
	float x_rad;
	long double num, fraction;
	double time_start = 0, time_end = 0;

	MPI_Init(&argc, &argv);        // starts MPI
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);  // get current process rank
	MPI_Comm_size(MPI_COMM_WORLD, &commSize);      // get number of processeser

	//take inputs from user
	if (myRank == 0)
	{
		printf("Enter n, k and x (in degree)\n");
		fflush(stdout);
		scanf_s("%d %d %d", &n, &k, &x);
		for (int dest = 1; dest < commSize; dest++)
		{
			MPI_Send(&n, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
			MPI_Send(&k, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
			MPI_Send(&x, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
		}
	}
	else
	{
		MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		MPI_Recv(&k, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		MPI_Recv(&x, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	}

	time_start = MPI_Wtime();
	//x and local_k are the same for all processes
	division = k / commSize;
	x_rad = (x * PI) / 180.0;

	//****** Compute parameters & local sum ******//
	local_sum_start = myRank * division;
	local_sum_end = local_sum_start + division;

	for (int i = local_sum_start; i < local_sum_end; i++)
	{
		num = (long double)(pow(-1, i) * pow(x_rad, 2 * i));
		fraction = num / factorial(2 * i);
		local_sum += fraction;
	}

	//send the local sum from the processes
	if (myRank != 0)
	{
		MPI_Send(&local_sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	}

	//recieve the local sums and collect them into total sum
	else
	{
		total_sum = local_sum;
		for (int source = 1; source < commSize; source++)
		{
			MPI_Recv(&local_sum, 1, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
			total_sum += local_sum;
		}
	}

	//print the total sum for user
	if (myRank == 0)
	{
		printf("The calculation of cos(x) with x = %d, k = %d, n = %d\n", x, k, n);
		printf("The result is %.10lf\n", total_sum);
		
		time_end = MPI_Wtime();
		printf("The execution time is %f\n", time_end - time_start);
	}

	MPI_Finalize();
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