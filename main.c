/* TSP Project */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define LENGTH 3

typedef struct City{
    float x;
    float y;
} City;

typedef struct Genome {
    unsigned int *path; // tracking city order array
    unsigned int length; // num cities
    float fitness; 
} Genome;

typedef struct Genomes {
    Genome *items;
    unsigned int count;
} Genomes;

typedef struct Permutation {
    unsigned int **array;
    unsigned int count;
} Permutation;

// Global Variables
City cities[LENGTH]; // city array

void swap(void *a, void *b, size_t size) {
    char *temp = malloc(size); // Allocate temporary storage
    memcpy(temp, a, size); // temp = a
    memcpy(a, b, size);    // a = b
    memcpy(b, temp, size); // b = temp
    free(temp); // free temp
}

unsigned int factorial(unsigned int n) {
    if (n < 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

void generate_permuations(unsigned int *a, unsigned int size, unsigned int **result, unsigned int *index, unsigned int length) {
    if (size == 1) {
        result[*index] = (unsigned int *)malloc(sizeof(unsigned int) * length);
        memcpy(result[*index], a , sizeof(unsigned int)*length);
        (*index)++;
    } else {
        for (unsigned int i = 0; i < size; ++i) {
            generate_permuations(a, size - 1, result, index, length);
            if (size % 2 == 0) {
                swap(&a[i], &a[size -  1], sizeof(unsigned int));
            } else {
                swap(&a[0], &a[size - 1], sizeof(unsigned int));
            }
        }
    }
}

Permutation *generate_permuations_of_indices(unsigned int *indices, unsigned int length) {
    unsigned int size = factorial(length);
    Permutation *permuation = (Permutation *)malloc(sizeof(Permutation));
    assert(permuation != NULL && "Memory Allocation for Dynamic Permuation Struct Failed");
    
    permuation->array = (unsigned int **)malloc(sizeof(unsigned int *) *size);
    assert(permuation->array != NULL && "Memory Allocation For Dynamic Permuation Array Failed");

    unsigned int *copy = (unsigned int *)malloc(sizeof(unsigned int) * length);
    memcpy(copy, indices, sizeof(unsigned int) * length);

    unsigned int index = 0;
    generate_permuations(copy, length, permuation->array, &index, length);
    permuation->count = size;

    free(copy);
    return permuation;
}

void print_permutations(Permutation *permutations) {
    for (unsigned int i = 0; i < permutations->count; ++i) {
        for (unsigned int j = 0; j < LENGTH; ++j) {
            printf(" %d ", permutations->array[i][j]);
        }
        printf("\n");
    }

    printf("Count: %d\n", permutations->count);
}

void free_permutations(Permutation *permutations) {
    if (permutations != NULL) {
        for (unsigned int i = 0; i < permutations->count; ++i) {
            free(permutations->array[i]);
        }
        free(permutations->array);
        free(permutations);
    }
}

unsigned int *deep_copy_array(const unsigned int *source, const unsigned int length) {
    unsigned int *destination = (unsigned int *)malloc(sizeof(unsigned int)* length);
    assert(destination != NULL && "Memory Allocation For Destination Array Failed");
    memcpy(destination, source, sizeof(unsigned int)*length);

    return destination;
}

unsigned int *select_random_permutation_of_unique_indices(unsigned int length) {
    unsigned int *temp = (unsigned int *) malloc(sizeof(unsigned int) * length);
    assert(temp != NULL && "Memory Allocation For Temporary Indices Array Failed");

    for (unsigned int i = 0; i < length; ++i) {
        temp[i] = i + 1;
    }
    Permutation *permuations = generate_permuations_of_indices(temp, length);
    unsigned int index = (unsigned int) rand() % permuations->count;
    unsigned int *indices = deep_copy_array(permuations->array[index], length);

    free(temp);
    print_permutations(permuations);
    free_permutations(permuations);
    return indices;
}

void print_indices(unsigned int *array, unsigned int length) {
    for (unsigned int i = 0; i < length; ++i) {
        printf("%d\n", array[i]);
    }
}

float random_float() {
    return (float) rand() / (float) RAND_MAX * 1.0f * 10.0f;
}

City generate_random_city() {
    return (City) {random_float(), random_float()};
}

float euclidean_distance(City a, City b) {
    return sqrtf(((b.x - a.x) * (b.x - a.x)) + ((b.y - a.y) * (b.y - a.y)));
}

void initialize_cities() {
    for (unsigned int i = 0; i < LENGTH; ++i) {
        cities[i] = generate_random_city();
    }
}

float calculate_fitness(Genome *g) {
    float total_distance = 0.0f;
    for (unsigned int i = 0; i < g->length - 1; ++i) {
        City city_1 = cities[g->path[i]];
        City city_2 = cities[g->path[i + 1]];
        total_distance += euclidean_distance(city_1, city_2);
    }

    return 1.0f / total_distance;
}

Genome generate_random_genome(unsigned int length) {
    unsigned int *path = select_random_permutation_of_unique_indices(length);

    Genome g = {
        .path = path,
        .length = length,
        .fitness = calculate_fitness(&g),
    };

    return g;
}

void initialize_population(Genomes *population, unsigned int size, unsigned int length) {
    for (unsigned int i = 0; i < size; ++i) {
        population->items[i] = generate_random_genome(length);
    }
    population->count = size;
}

Genome deep_copy_genome(const Genome *source) {
    Genome destination;
    destination.length = source->length;
    destination.fitness = source->fitness;
    destination.path = (unsigned int *)malloc(sizeof(unsigned int)*destination.length);
    assert(destination.path != NULL && "Memory Allocation For Destination Cities Failed");
    memcpy(destination.path, source->path, sizeof(unsigned int)*destination.length);
    return destination;
}

Genomes *select_parents(const Genomes *gs) {
    /* 
        We are selecting Genomes from the Population to be parents 
        so the Genomes with the highest fitness are chosen
    */

    unsigned int max = 0 , max_2 = 1;

    if (gs->items[max_2].fitness > gs->items[max].fitness) {
        swap(&max, &max_2, sizeof(unsigned int));
    }

    for (unsigned int i = 2; i < gs->count; ++i) {
        if (gs->items[i].fitness > gs->items[max].fitness) {
            max_2 = max;
            max = i;
        } else if (gs->items[i].fitness > gs->items[max_2].fitness) {
            max_2 = i;
        }
    }

    Genomes *parents = (Genomes *)malloc(sizeof(Genomes));
    assert(parents != NULL && "Memory Allocation for Parents Items Failed");

    parents->items = (Genome *)malloc(sizeof(Genome) * 2);
    assert(parents->items != NULL && "Memory Allocation for Parents Items Failed");
    parents->count = 2;

    parents->items[0] = deep_copy_genome(&gs->items[max]);
    parents->items[1] = deep_copy_genome(&gs->items[max_2]);

    return parents;
}

// Genome crossover(const Genome *parent1, const Genome *parent2) {

// }

void print_genome(const Genome *g) {
    printf("lenght: %d\n", g->length);
    printf("fitness: %f\n", g->fitness);
    printf("+-------------------------+\n");
    printf("|Cities:    x   |     y   |\n");
    printf("+-------------------------+\n");
    for (unsigned int i = 0; i < g->length; ++i) {
        printf("|City_%d:  %.2f  |   %.2f  |\n", i+1 , cities[g->path[i]].x , cities[g->path[i]].y);
        printf("+-------------------------+\n");
    }
}

void print_population(const Genomes *gs) {
    for (unsigned int i = 0; i < gs->count; ++i) {
        print_genome(&gs->items[i]);
    }
}

void free_genome(Genome *g) {
    if (g->path != NULL) {
        free(g->path);
        g->path = NULL;
    }
}

void free_population(Genomes *gs) {
    if (gs != NULL) {
        for (unsigned int i = 0; i < gs->count; ++i) {
            free_genome(&gs->items[i]);
        }
        free(gs->items);
        free(gs);
    }
}

int main(void) {
    srand(time(NULL));

    initialize_cities();

    unsigned int size = 100;
    Genomes *gs = (Genomes *)malloc(sizeof(Genomes));
    assert(gs != NULL && "Memory Allocation for Population Failed");

    gs->items = (Genome *)malloc(sizeof(Genome) * size);
    assert(gs != NULL && "Memory Allocation for Genime Array Failed");

    initialize_population(gs, size, LENGTH);
    // print_population(gs);

    Genomes *g = select_parents(gs);
    print_population(g);

    free_population(g);
    free_population(gs);
    return 0;
}
