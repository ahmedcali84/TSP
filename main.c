/* TSP Project */

#include "genetic.h"
#include "alog.h"

int main(void) {
    srand(time(NULL));

    // Dumping Output File
    const char *Output_Stream = "output.txt";
    FILE *fw = fopen(Output_Stream, "w");

    // Initialize Cities
    initialize_cities(LENGTH);

    unsigned int size = 5; // Population Size

    // Evolution Start
    States state = INITIALIZE;
    Genomes *Population = NULL;
    Genomes *Parents = NULL;
    Genome offspring = {0};
    unsigned int generations = 0;

    float best_fitness = 0.0f;
    unsigned int stagnation = 0.0;

    // evolution loop
    while (state != TERMINATE) {
        switch (state) {
            case INITIALIZE:
                // Allocate Memory For Population
                Population = (Genomes *)malloc(sizeof(Genomes));
                assert(Population != NULL && "Memory Allocation for Population Failed");

                Population->items = (Genome *)malloc(sizeof(Genome) * size);
                assert(Population->items != NULL && "Memory Allocation for Population Genome Array Failed");

                initialize_population(Population, size, LENGTH);
                state = SELECT_PARENTS;
                break;

            case SELECT_PARENTS:
                if (Parents) free_population(Parents);
                Parents = select_parents(Population);
                state = CROSSOVER;
                break;

            case CROSSOVER:
                offspring = crossover(Parents);
                if (Parents) free_population(Parents);
                Parents = NULL;
                state = MUTATION;
                break;

            case MUTATION:
                mutate_genome(&offspring, MUTATION_RATE);
                state = REPLACE;
                break;
            
            case REPLACE:
                replace_worst(Population, &offspring);
                generations++;
                
                print_genome(fw, &offspring , "Child");
                free_genome(&offspring);
                offspring.path = NULL;

                Genome *current_best = find_best(Population);
                if (current_best && current_best->fitness > best_fitness) {
                    best_fitness = current_best->fitness;
                    stagnation = 0;
                } else {
                    stagnation++;
                }

                Log_Out(DEBUG, "Generation %3d | Best Fitness: %.4f | Stagnation: %2d\n",
                    generations, best_fitness, stagnation);

                if (generations >= MAX_GENERATIONS || stagnation >= STAGNATION_LIMIT) {
                    PRINT_POPULATION(fw , Population); // print population
                    if (Parents) PRINT_POPULATION(fw , Parents); // print parents
                    fclose(fw); // Close file
                    state = TERMINATE;
                } else {
                    state = SELECT_PARENTS;
                }
                break;

            case TERMINATE:
                break;
        }
    }
    print_genome(stdout, find_best(Population), "Best Route");

    // Free Memory
    if (Parents) free_population(Parents);
    if (Population) free_population(Population);
    return 0;
}


/* ******************* This Main Function Was Used For Debugging Purposes when this project was being developped ************ */
// int main2(void) {
//     srand(time(NULL));

//     // Dumping Output File
//     const char *output_stream = "output.txt";
//     FILE *fw = fopen(output_stream, "w");

//     // Initialize Cities
//     initialize_cities(LENGTH);

//     // Allocate Memory For Population
//     unsigned int size = 5; // Population Size
//     Genomes *Population = (Genomes *)malloc(sizeof(Genomes));
//     assert(Population != NULL && "Memory Allocation for Population Failed");

//     Population->items = (Genome *)malloc(sizeof(Genome) * size);
//     assert(Population != NULL && "Memory Allocation for Genime Array Failed");

//     // Initialize Population
//     initialize_population(Population, size, LENGTH);
//     PRINT_POPULATION(fw , Population); // print population

//     // Select Parents
//     Genomes *Parents = select_parents(Population);
//     PRINT_POPULATION(fw , Parents); // print parents

//     // Child CrossOver
//     Genome child = crossover(&Parents->items[0], &Parents->items[1]);
//     mutate_genome(&child , 0.90);
//     print_genome(fw , &child, "child");

//     printf("Successfully dumped output to %s\n", output_stream);
//     fclose(fw); // Close file

//     // Free Memory
//     free_genome(&child);
//     free_population(Parents);
//     free_population(Population);
//     return 0;
// }
