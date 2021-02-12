#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    bool res = false;
    for (int i = 0; i < candidate_count; i++) //iterate through every candidate to check if the given name exists
    {
        if (strcmp(name, candidates[i].name) == 0)
        {
            preferences[voter][rank] = i; //update preferences if the imput matchs to the candidate
            res = true;
            break;
        }
    }
    printf("%i \n", preferences[voter][rank]);
    return res;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    int cant_filas = voter_count;
    int cant_columnas = candidate_count;

    printf("%i, %i\n", cant_filas, cant_columnas);
    for (int i = 0; i < cant_filas; i++)        //iterate throught filas (matriz)
    {
        for (int j = 0; j < cant_columnas; j++) //iterate trhought columnas (matriz)
        {
            int candidates_index = preferences[i][j]; //determine the candidate

            if (!candidates[candidates_index].eliminated) //check if the candidate is eliminated
            {
                candidates[candidates_index].votes += 1; //if is not eliminated, increase the vote of the candidate by 1 eevery time is selected
                break;
            }
        }
    }
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    bool rs = false;

    int needed_votes = (voter_count / 2) + 1;

    for (int i = 0; i < candidate_count; i++) //iterate through every candidate to check every one
    {
        if (candidates[i].votes >= needed_votes) //ckeck if that candidate has a mojority of votes
        {
            printf("%s\n", candidates[i].name); //print winners name
            rs = true;
        }
    }
    return rs;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    int min = 100;
    for (int i = 0; i < candidate_count; i++)
    {
        if ((!candidates[i].eliminated) && (candidates[i].votes < min)) //check if the candidate is eliminated and if is smaller than min
        {
            min = candidates[i].votes;
        }
    }
    return min;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    int tie = 0;
    int num_of_actual_cand = 0;
    bool rs = false;
    for (int i = 0; i < candidate_count; i++)
    {
        if (!candidates[i].eliminated)    //check if the candidate is not eliminated
        {
            num_of_actual_cand += 1; //increase by 1 to know the number of candidates we are working with

            if (candidates[i].votes == min) //check if the candidate has the min votes
            {
                tie += 1; //increase tie by 1
            }
        }
    }

    if (tie == num_of_actual_cand) //if tie is greather than 1 it means theres is a tie, so it will be true
    {
        rs = true;
    }
    return rs;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    bool candidates_status = false;

    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == min) //check if the candidate has the min votes
        {
            candidates[i].eliminated = true; //update candidates value to true
        }
    }
}