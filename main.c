/* TSP Project */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <time.h>

#define LENGTH 4

typedef struct Genome {
	int *cities;
	int lenght;
	double fitness;
} Genome;

typedef struct Genomes {
	Genome *items;
	size_t count;
	size_t capacity;
} Genomes;

int *generate_random_permutation_of_cities() {
#define M LENGTH
#define N LENGTH

	unsigned char is_used[N] = { 0 }; /* flags */
	int in, im = 0;

	int *cities = (int *)malloc(sizeof(int)*LENGTH);
	assert(cities != NULL && "Memory Allocation for cities failed");

	for (in = N - M; in < N && im < M; ++in) {
		int r = rand() % N; /* generate a random number 'r' */

		while (is_used[r]) {
			/* we already have 'r' */
			r = rand() % N;
		}

		cities[im++] = r + 1; /* +1 since your range begins from 1 */
		is_used[r] = 1;
	}
	assert(im == M);
	return cities;
}

Genome generate_random_genome() {
	Genome g = {
		.fitness = 0.0,
		.lenght = LENGTH,
		.cities = generate_random_permutation_of_cities(LENGTH)
	};

	return g;
}

void print_genome(Genome *g) {
	printf("lenght: %d\n", g->lenght);
	printf("fitness: %lf\n", g->fitness);
	printf("Citiies: ");
	for (int i = 0; i < g->lenght; ++i) {
		if (i != g->lenght-1) {
			printf(" City_%d ->", g->cities[i]);
		} else {
			printf(" City_%d ", g->cities[i]);
		}
	}
	printf("\n");
}

int main(void) {
	srand(time(NULL));
	Genome g = generate_random_genome(LENGTH);
	print_genome(&g);
	return 0;
}