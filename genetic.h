#ifndef GENETIC_H
#define GENETIC_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define LENGTH 8 /* Number of Cities */
#define SIZE   10 /* Population Size */
#define MUTATION_RATE 1.0f /* Mutation Rate */
#define MAX_GENERATIONS (SIZE / 2) /* Maximum Generations */
#define STAGNATION_LIMIT 3

#define INITIAL_CAPACITY SIZE /* INITIAL CAPACITY FOR THE POPULATION */

typedef enum {
    INITIALIZE,
    SELECT_PARENTS,
    CROSSOVER,
    MUTATION,
    REPLACE,
    TERMINATE,
} States;

typedef struct City {
    float x;
    float y;
} City;

typedef struct Genome {
    unsigned int *path;  /* tracking city order array */
    unsigned int length; /* num cities */
    float fitness; 
} Genome;

typedef struct Genomes {
    Genome *items;
    unsigned int count;
    unsigned int capacity;
} Genomes;

typedef struct Permutation {
    unsigned int **array;
    unsigned int count;
} Permutation;

void swap(void *a, void *b, size_t size);
unsigned int factorial(unsigned int n);

void generate_permutations(unsigned int *a, unsigned int size, unsigned int **result, unsigned int *index, unsigned int length);
Permutation *generate_permutations_of_indices(unsigned int *indices, unsigned int length);
void print_permutations(Permutation *permutations);
void free_permutations(Permutation *permutations);

unsigned int *deep_copy_array(const unsigned int *source, const unsigned int length);
unsigned int *select_random_permutation_of_unique_indices(unsigned int length);
void print_indices(unsigned int *array, unsigned int length);

float random_float();
City generate_random_city();
float euclidean_distance(City a, City b);
void initialize_cities(unsigned int length);

float calculate_fitness(Genome *g);
Genome generate_random_genome(unsigned int length);
Genome deep_copy_genome(const Genome *source);
void mutate_genome(Genome *g, float mutation_rate);
void print_genome(FILE *stream, const Genome *g, const char *title);
void free_genome(Genome *g);

void initialize_population(Genomes *population, unsigned int size, unsigned int length);
Genomes *select_parents(const Genomes *gs);
Genome crossover(const Genomes *parents);
void append_to_population(Genomes *population, Genome *member);
void pop_from_population(Genomes *population, Genome *member);
Genome *find_weakest(Genomes *population);
Genome *find_best(Genomes *population);
void replace_worst(Genomes *population, Genome *new_member);
void print_population(FILE *stream, const Genomes *gs, const char *title);

#define PRINT_POPULATION(stream, population) print_population(stream, population, #population) /*Special Print Function*/

void free_population(Genomes *gs);

#endif  /*GENETIC_H*/