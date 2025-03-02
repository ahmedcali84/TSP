#include "genetic.h"
#include "alog.h"

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

void generate_permutations(unsigned int *a, unsigned int size, unsigned int **result, unsigned int *index, unsigned int length) {
    if (size == 1) {
        result[*index] = (unsigned int *)malloc(sizeof(unsigned int) * length);
        memcpy(result[*index], a , sizeof(unsigned int)*length);
        (*index)++;
    } else {
        for (unsigned int i = 0; i < size; ++i) {
            generate_permutations(a, size - 1, result, index, length);
            if (size % 2 == 0) {
                swap(&a[i], &a[size -  1], sizeof(unsigned int));
            } else {
                swap(&a[0], &a[size - 1], sizeof(unsigned int));
            }
        }
    }
}

Permutation *generate_permutations_of_indices(unsigned int *indices, unsigned int length) {
    unsigned int size = factorial(length);
    Permutation *permuation = (Permutation *)malloc(sizeof(Permutation));
    assert(permuation != NULL && "Memory Allocation for Dynamic Permuation Struct Failed");
    
    permuation->array = (unsigned int **)malloc(sizeof(unsigned int *) *size);
    assert(permuation->array != NULL && "Memory Allocation For Dynamic Permuation Array Failed");

    unsigned int *copy = (unsigned int *)malloc(sizeof(unsigned int) * length);
    memcpy(copy, indices, sizeof(unsigned int) * length);

    unsigned int index = 0;
    generate_permutations(copy, length, permuation->array, &index, length);
    permuation->count = size;

    free(copy);
    return permuation;
}

void print_permutations(Permutation *permutations) {
    for (unsigned int i = 0; i < permutations->count; ++i) {
        for (unsigned int j = 0; j < LENGTH; ++j) {
            Log_Out(DEBUG, " %d ", permutations->array[i][j]);
        }
        Log_Out(DEBUG, "%s" ,"\n");
    }

    Log_Out(DEBUG, "Count: %d\n", permutations->count);
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
        temp[i] = i;
    }

    Permutation *permuations = generate_permutations_of_indices(temp, length);
    unsigned int index = (unsigned int) rand() % permuations->count;
    unsigned int *indices = deep_copy_array(permuations->array[index], length);

    free(temp);
    // print_permutations(permuations);
    free_permutations(permuations);
    return indices;
}

void print_indices(unsigned int *array, unsigned int length) {
    for (unsigned int i = 0; i < length; ++i) {
        Log_Out(INFO,"%d\n", array[i]);
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

void initialize_cities(unsigned int length) {
    for (unsigned int i = 0; i < length; ++i) {
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
    population->capacity = INITIAL_CAPACITY;
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

Genome crossover(const Genomes *parents) {
    Genome child;
    child.length = parents->items[0].length;
    child.path = (unsigned int *)malloc(sizeof(unsigned int) * parents->items[0].length);
    assert(child.path != NULL && "Memory Allocation For Child Path Array Failed");

    // Randomly select a cross over range
    unsigned int start = rand() % (child.length);
    unsigned int end   = rand() % (child.length);
    if (start > end) swap(&start , &end , sizeof(unsigned int));

    // Copy Segment of cities from parent 1
    bool *in_segment = calloc(child.length , sizeof(bool));
    for (unsigned int i = start; i <= end; ++i) {
        child.path[i] = parents->items[0].path[i];
        in_segment[parents->items[0].path[i]] = true;
    }

    // Fill the Remaining With Parent 2 cities
    // Fill positions before start
    unsigned int parent_index = 0;
    for (unsigned int i = 0; i < start; ++i) {
        while (parent_index < parents->items[1].length) {
            unsigned int city = parents->items[1].path[parent_index];
            if (!in_segment[city]) {
                child.path[i] = city;
                parent_index++;
                break;
            }
            parent_index++;
        }
    }

    // Fill positions after end
    for (unsigned int i = end + 1; i < child.length; ++i) {
        while (parent_index < parents->items[1].length) {
            unsigned int city = parents->items[1].path[parent_index];
            if (!in_segment[city]) {
                child.path[i] = city;
                parent_index++;
                break;
            }
            parent_index++;
        }
    }

    free(in_segment);
    child.fitness = calculate_fitness(&child);
    return child;
}

void mutate_genome(Genome *g, float mutation_rate) {
    if (((float) rand() / (float) RAND_MAX) < mutation_rate) {
        unsigned int i = rand() % g->length;
        unsigned int j = rand() % g->length;

        do {
            i = rand() % g->length;
            j = rand() % g->length;
        } while (i == j);

        swap(&g->path[i] , &g->path[j] , sizeof(unsigned int));
        g->fitness = calculate_fitness(g);
    }
}

void append_to_population(Genomes *population, Genome *member)  {
    if (population->count == population->capacity) {
        // Reallocate if population capped
        population->capacity = (population->capacity == 0) ? INITIAL_CAPACITY : population->capacity * 2;
        population->items = realloc(population->items, population->capacity * sizeof(Genome));
        assert(population->items != NULL && "Memory Reallocation Failed");
    }

    // Allocate Memory for the new Genome
    population->items[population->count] = deep_copy_genome(member);
    population->count++;
}

void pop_from_population(Genomes *population, Genome *member) {
    int index = -1;
    for (unsigned int i = 0; i < population->count; ++i) {
        if (&population->items[i] == member) {
            index = i;
            break;
        }
    }

    if (index == -1) return; // Not Found
    free_genome(member);

    if (index != (int) population->count - 1) {
        swap(&population->items[index], &population->items[population->count - 1], sizeof(Genome));
    }

    // Free Last element after shift
    free_genome(&population->items[population->count - 1]);
    population->count--;
}

Genome *find_weakest(Genomes *population) {
    if (population->count == 0) return NULL; // Return NULL on empty population

    unsigned int min = 0;
    for (unsigned int i = 1; i < population->count; ++i) {
        if (population->items[i].fitness < population->items[min].fitness) {
            min = i;
        }
    }

    return &population->items[min];
}

Genome *find_best(Genomes *population) {
    if (population->count == 0) return NULL;

    unsigned int max = 0;
    for (unsigned int i = 1; i < population->count; ++i) {
        if (population->items[i].fitness > population->items[max].fitness) {
            max = i;
        }
    }

    return &population->items[max];
}

void replace_worst(Genomes *population, Genome *new_member) {
    Genome *worst = find_weakest(population);
    pop_from_population(population, worst);
    append_to_population(population, new_member);
}

void print_genome(FILE *stream, const Genome *g, const char *title) {
    Log_File(stream, INFO, "%s: \n", title);
    Log_File(stream, INFO, "lenght: %d\n", g->length);
    Log_File(stream, INFO, "fitness: %f\n", g->fitness);
    Log_File(stream, INFO, "%s", "+-------------------------+\n");
    Log_File(stream, INFO, "%s", "|Cities:    x   |     y   |\n");
    Log_File(stream, INFO, "%s", "+-------------------------+\n");
    for (unsigned int i = 0; i < g->length; ++i) {
        Log_File(stream, INFO, "|City_%d:  %.2f  |   %.2f  |\n", i+1 , cities[g->path[i]].x , cities[g->path[i]].y);
        Log_File(stream, INFO, "%s" , "+-------------------------+\n");
    }
    Log_File(stream, INFO, "%s" , "----------------------------\n");
}

void print_population(FILE *stream, const Genomes *gs, const char *title) {
    Log_File(stream, INFO, " %s :\n", title);
    for (unsigned int i = 0; i < gs->count; ++i) {
        print_genome(stream, &gs->items[i], "member");
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