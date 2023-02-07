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
    //compare inputted candidate to argument using strcmp
    //if similar then return true
    for (int j = 0; j < candidate_count ; j++)
    {
        if (strcmp(name, candidates[j].name) == 0)
        {
            preferences [voter][rank] = j;
            return true;
        }
    }
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    //if canidate not eliminated, increase vote count
    //program counter for tabulated candidate and vote
    for (int a = 0; a < voter_count; a++)
    {
        for (int b = 0; b < candidate_count; b++)
        {
            int j = preferences [a][b];
            if (candidates[j].eliminated == false)
            {
                candidates[j].votes++;
                break;
            }
        }
    }
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    //print name of candidate with maximum number of votes
    for (int j = 0; j < candidate_count; j++)
    {
        if (candidates[j].votes > (voter_count / 2))
        {
            printf("%s\n", candidates[j].name);
            return true;
        }
    }
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    //find candidate with minimum number of votes
    int c = voter_count;
    for (int j = 0; j < candidate_count; j++)
    {
        if (candidates[j].votes < c && candidates[j].eliminated == false)
        {
            c = candidates[j].votes;
        }
    }
    return c;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    //compare votes of the candidates and return true if number of votes are the same
    int x = 0, y = 0;
    for (int j = 0; j < candidate_count; j++)
    {
        if (candidates[j].eliminated == false && candidates[j].votes == min)
        {
            x++;
            y++;
        }
        else if (candidates[j].eliminated == false)
        {
            x++;
        }
    }
    if (x == y)
    {
        return true;
    }
    return false;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    //if candidate has minimum number of votes then eliminate
    for (int j = 0; j < candidate_count; j++)
    {
        if (candidates[j].eliminated == false && candidates[j].votes == min)
        {
            candidates[j].eliminated = true;
        }
    }
}