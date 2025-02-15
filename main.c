/* TSP Project */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define LENGTH 4

typedef struct City{
	float x;
	float y;
} City;

typedef struct Genome {
	City *cities;
	int lenght;
	float fitness;
} Genome;

typedef struct Genomes {
	Genome *items;
	size_t count;
	size_t capacity;
} Genomes;

int *generate_random_permutation_of_unique_indices() {
#define M LENGTH
#define N LENGTH

	unsigned char is_used[N] = { 0 }; /* flags */
	int in, im = 0;

	int *cities_indices = (int *)malloc(sizeof(int)*LENGTH);
	assert(cities_indices != NULL && "Memory Allocation for cities failed");

	for (in = N - M; in < N && im < M; ++in) {
		int r = rand() % N; /* generate a random number 'r' */

		while (is_used[r]) {
			/* we already have 'r' */
			r = rand() % N;
		}

		cities_indices[im++] = r; /* +1 since your range begins from 1 */
		is_used[r] = 1;
	}
	assert(im == M);
	return cities_indices;
}

float random_float() {
	return (float) rand() / (float) RAND_MAX * 2.0f * 10.0f;
}

City generate_random_city() {
	return (City) {random_float(), random_float()};
}

float euclidean_distance(City a, City b) {
	float d = sqrtf(((b.x - a.x) * (b.x - a.x)) + ((b.y - a.y) * (b.y - a.y)));
	return d;
}

Genome generate_random_genome() {
	City *cities = (City *)malloc(sizeof(City) * LENGTH);
	assert(cities != NULL && "Memory Allocation for Cities Failed.");

	memset(cities, 0 , sizeof(*cities));

	int *indices = generate_random_permutation_of_unique_indices();
	for (int i = 0; i < LENGTH; ++i) {
		cities[indices[i]] = generate_random_city();
	}

	float sum = 0;
	for (int i = 0; i < LENGTH - 1; ++i) {
		float d = euclidean_distance(cities[i], cities[i + 1]);
		sum += d;
	}

	Genome g = {
		.fitness = 1.0f / sum,
		.lenght = LENGTH,
		.cities = cities,
	};

	free(indices);

	return g;
}

void print_genome(Genome *g) {
	printf("lenght: %d\n", g->lenght);
	printf("fitness: %f\n", g->fitness);
	printf("Cities:   x   ,   y\n");
	for (int i = 0; i < g->lenght; ++i) {
		printf("City_%d:  %.2f  ,  %.2f  \n", i+1 , g->cities[i].x , g->cities[i].y);
	}
	printf("\n");
}

void free_genome(Genome *g) {
	if (g->cities != NULL) {
		free(g->cities);
		g->cities = NULL;
	}
}

int main(void) {
	srand(time(NULL));
	Genome g = generate_random_genome(LENGTH);
	print_genome(&g);
	free_genome(&g);
	return 0;
}