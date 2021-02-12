#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    bool res = false;
    for (int i = 0; i < candidate_count; i++) //iterate through every candidate to check if the given name exists
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i; //update the ranks array to indicate that the voter has the candidate as their rank preference
            res = true;
            break;
        }
    }
    printf("%i \n", ranks[rank]);
    return res;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    int cant_filas = candidate_count;
    int cant_columnas = candidate_count;

    printf("%i, %i\n", cant_filas, cant_columnas);

    for (int i = 0; i < cant_filas; i++)        // iterate throught pairs
    {
        for (int j = i + 1; j < cant_columnas; j++) // iterate trhought candidates
        {
            // ranks[i] is the voter’s ith preference and so is ranks[j]
            // increment current voter’s preferences on filas and columnas
            preferences[ranks[i]][ranks[j]]++;
            // preferences[i][j] represents the number of voters who prefer candidate i over candidate j
            // estamos trabajando en rangos, es por ello que se individualiza, sino se actualizaria segun el orden en el que incrementa la iteracion, y queremos que aumente seg[un el index del candidato introducido]
        }
    }
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    pair_count = 0;

    for (int i = 0; i < candidate_count; i++)               // iterate throught candidates
    {
        for (int j = 0; j < candidate_count; j++)           // iterate throught candidates ith preferences
        {
            if (preferences[i][j] > preferences[j][i])      // set winner and loser if there is no tie
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;

                pair_count += 1;
            }
        }
    }
}


int margin(pair p)
{
    return preferences[p.winner][p.loser] - preferences[p.loser][p.winner];
}


int _than(int par1, int par2)
{
    int res = false;

    if (par1 > par2)
    {
        res = 1;
    }

    else if (par1 == par2)
    {
        res = 0;
    }

    else
    {
        res = -1;
    }

    return res;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int swap_counter = -1;

    while (swap_counter != 0)
    {
        swap_counter = 0;

        for (int i = 0; i < pair_count; i++)
        {
            if (_than(margin(pairs[i]), margin(pairs[i + 1])) == -1)
            {
                pair temp_swap = pairs[i];
                pairs[i] = pairs[i + 1];
                pairs[i + 1] = temp_swap;

                swap_counter += 1;
            }
        }
    }
}

bool recursive_cycle(int winner, int loser)
{
    /*********************** CASO BASE ************************************************/
    //caso base es a b, b a
    if (locked[loser][winner])
    {
        return true;
    }
    /*********************** CASO BASE ************************************************/

    /*++++++++++++++++++++++++++++ PASO RECURSIVO ++++++++++++++++++++++++++++*/
    //paso recursivo
    //en este paso buscamos RECURSIVAMENTE si hay un ciclo
    for (int i = 0; i < pair_count; i++)
    {
        //si puede haber un ciclo al revisar las conecciones del ganador, entonces quiero recorrer el camino generado por el perdedor
        //para recorrer el camino voy a llamar recursivamente a la funcion
        //si no entro en el if, no entro a la funcion, voy al caso base en el q nunca hubo un ciclo y devuelvo false

        if (locked[i][winner])
        {
            return recursive_cycle(i, loser);
        }
    }

    /*++++++++++++++++++++++++++++ PASO RECURSIVO ++++++++++++++++++++++++++++*/

    /*********************** CASO BASE ************************************************/
    return false;
    /*********************** CASO BASE ************************************************/
}


bool is_cycle(pair given)
{
    bool cycle_exists = false;  //this one is a recursive function

    if (recursive_cycle(given.winner, given.loser))  //chequear si al agregar el par given se genera un ciclo
    {
        cycle_exists = true;
    }

    return cycle_exists;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        pair aux_pair = pairs[i];

        if (is_cycle(aux_pair))
        {
            locked[aux_pair.winner][aux_pair.loser] = false;
        }

        else
        {
            locked[aux_pair.winner][aux_pair.loser] = true;
        }
    }
}

// Print the winner of the election
void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        int loser_times = 0;

        for (int j = 0; j < candidate_count; j++)
        {

            if (locked[j][i])
            {
                loser_times ++;
            }
        }

        if (loser_times == 0)
        {
            printf("%s\n", candidates[i]);
        }
    }
}

