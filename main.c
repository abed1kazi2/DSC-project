/*
 * ╔══════════════════════════════════════════════════════════╗
 *        VOTING SYSTEM SIMULATOR — Written in C
 *   Features: Register candidates, cast votes, prevent
 *             double voting, declare winner with stats.
 * ╚══════════════════════════════════════════════════════════╝
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/* ─── Constants ─────────────────────────────────────────── */
#define MAX_CANDIDATES  10
#define MAX_VOTERS      100
#define MAX_NAME_LEN    50
#define MAX_ID_LEN      20

/* ─── Data Structures ───────────────────────────────────── */
typedef struct {
    char name[MAX_NAME_LEN];
    int  votes;
    int  id;
} Candidate;

typedef struct {
    char voter_id[MAX_ID_LEN];
    int  has_voted;
} Voter;

/* ─── Global State ──────────────────────────────────────── */
Candidate candidates[MAX_CANDIDATES];
Voter     voters[MAX_VOTERS];
int       candidate_count = 0;
int       voter_count     = 0;
int       voting_open     = 0;

/* ─── Utility ───────────────────────────────────────────── */
void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void print_divider(void) {
    printf("──────────────────────────────────────────────────\n");
}

void to_upper(char *str) {
    for (int i = 0; str[i]; i++)
        str[i] = toupper((unsigned char)str[i]);
}

/* ─── Register Candidate ────────────────────────────────── */
void register_candidate(void) {
    if (voting_open) {
        printf("  [!] Cannot register candidates while voting is open.\n");
        return;
    }
    if (candidate_count >= MAX_CANDIDATES) {
        printf("  [!] Maximum candidate limit (%d) reached.\n", MAX_CANDIDATES);
        return;
    }

    char name[MAX_NAME_LEN];
    printf("  Enter candidate name : ");
    fgets(name, MAX_NAME_LEN, stdin);
    name[strcspn(name, "\n")] = '\0';

    if (strlen(name) == 0) {
        printf("  [!] Name cannot be empty.\n");
        return;
    }

    char upper_input[MAX_NAME_LEN];
    strcpy(upper_input, name);
    to_upper(upper_input);

    for (int i = 0; i < candidate_count; i++) {
        char upper_existing[MAX_NAME_LEN];
        strcpy(upper_existing, candidates[i].name);
        to_upper(upper_existing);
        if (strcmp(upper_input, upper_existing) == 0) {
            printf("  [!] Candidate '%s' already registered.\n", name);
            return;
        }
    }

    candidates[candidate_count].id    = candidate_count + 1;
    candidates[candidate_count].votes = 0;
    strcpy(candidates[candidate_count].name, name);
    candidate_count++;

    printf("  [✓] '%s' registered as Candidate #%d.\n", name, candidate_count);
}

/* ─── Open / Close Voting ───────────────────────────────── */
void toggle_voting(void) {
    if (!voting_open) {
        if (candidate_count < 2) {
            printf("  [!] At least 2 candidates required to open voting.\n");
            return;
        }
        voting_open = 1;
        printf("  [✓] Voting is now OPEN.\n");
    } else {
        voting_open = 0;
        printf("  [✓] Voting is now CLOSED.\n");
    }
}

/* ─── Display Candidates ────────────────────────────────── */
void display_candidates(void) {
    if (candidate_count == 0) {
        printf("  [!] No candidates registered yet.\n");
        return;
    }
    print_divider();
    printf("  %-4s  %-30s  %s\n", "ID", "Candidate Name", "Votes");
    print_divider();
    for (int i = 0; i < candidate_count; i++) {
        printf("  %-4d  %-30s  %d\n",
               candidates[i].id,
               candidates[i].name,
               candidates[i].votes);
    }
    print_divider();
}

/* ─── Cast Vote ─────────────────────────────────────────── */
void cast_vote(void) {
    if (!voting_open) {
        printf("  [!] Voting is currently closed.\n");
        return;
    }
    if (voter_count >= MAX_VOTERS) {
        printf("  [!] Maximum voter limit (%d) reached.\n", MAX_VOTERS);
        return;
    }

    char voter_id[MAX_ID_LEN];
    printf("  Enter your Voter ID  : ");
    fgets(voter_id, MAX_ID_LEN, stdin);
    voter_id[strcspn(voter_id, "\n")] = '\0';

    if (strlen(voter_id) == 0) {
        printf("  [!] Voter ID cannot be empty.\n");
        return;
    }

    /* Double-voting prevention */
    for (int i = 0; i < voter_count; i++) {
        if (strcmp(voters[i].voter_id, voter_id) == 0) {
            printf("  [✗] Voter ID '%s' has already cast a vote!\n", voter_id);
            return;
        }
    }

    display_candidates();
    int choice;
    printf("  Enter Candidate ID to vote for : ");
    if (scanf("%d", &choice) != 1) {
        printf("  [!] Invalid input.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    if (choice < 1 || choice > candidate_count) {
        printf("  [!] Invalid Candidate ID '%d'.\n", choice);
        return;
    }

    strcpy(voters[voter_count].voter_id, voter_id);
    voters[voter_count].has_voted = 1;
    voter_count++;

    candidates[choice - 1].votes++;
    printf("  [✓] Vote cast for '%s' by Voter '%s'.\n",
           candidates[choice - 1].name, voter_id);
}

/* ─── Declare Winner ────────────────────────────────────── */
void declare_winner(void) {
    if (candidate_count == 0) {
        printf("  [!] No candidates registered.\n");
        return;
    }
    if (voting_open) {
        printf("  [!] Close the voting session before declaring a winner.\n");
        return;
    }

    int total_votes = 0;
    for (int i = 0; i < candidate_count; i++)
        total_votes += candidates[i].votes;

    if (total_votes == 0) {
        printf("  [!] No votes have been cast yet.\n");
        return;
    }

    int max_votes = 0;
    for (int i = 0; i < candidate_count; i++)
        if (candidates[i].votes > max_votes)
            max_votes = candidates[i].votes;

    int winner_ids[MAX_CANDIDATES];
    int winner_count = 0;
    for (int i = 0; i < candidate_count; i++)
        if (candidates[i].votes == max_votes)
            winner_ids[winner_count++] = i;

    print_divider();
    printf("\n         ★  ELECTION RESULTS  ★\n\n");
    printf("  %-28s  %6s  %6s  %s\n", "Candidate", "Votes", "%", "Bar");
    print_divider();

    for (int i = 0; i < candidate_count; i++) {
        double pct = candidates[i].votes * 100.0 / total_votes;
        int bars = (int)(pct / 5);
        printf("  %-28s  %6d  %5.1f%%  ",
               candidates[i].name, candidates[i].votes, pct);
        for (int b = 0; b < bars; b++) printf("█");
        printf("\n");
    }

    print_divider();
    printf("  Total Votes: %d  |  Voters: %d\n", total_votes, voter_count);
    print_divider();

    if (winner_count == 1) {
        printf("\n  🏆  WINNER : %s  (%d votes, %.1f%%)\n\n",
               candidates[winner_ids[0]].name,
               max_votes,
               max_votes * 100.0 / total_votes);
    } else {
        printf("\n  ⚖  TIE between:\n");
        for (int i = 0; i < winner_count; i++)
            printf("     • %s (%d votes)\n",
                   candidates[winner_ids[i]].name, max_votes);
        printf("  A run-off election may be required.\n\n");
    }
}

/* ─── Voter Roll ────────────────────────────────────────── */
void show_voter_roll(void) {
    if (voter_count == 0) {
        printf("  [!] No votes cast yet.\n");
        return;
    }
    print_divider();
    printf("  %-5s  %s\n", "No.", "Voter ID");
    print_divider();
    for (int i = 0; i < voter_count; i++)
        printf("  %-5d  %s\n", i + 1, voters[i].voter_id);
    print_divider();
    printf("  Total voters : %d\n", voter_count);
    print_divider();
}

/* ─── Reset Election ────────────────────────────────────── */
void reset_election(void) {
    char confirm[4];
    printf("  [!] This will erase ALL data. Type YES to confirm: ");
    fgets(confirm, sizeof(confirm), stdin);
    confirm[strcspn(confirm, "\n")] = '\0';

    if (strcmp(confirm, "YES") == 0) {
        candidate_count = 0;
        voter_count     = 0;
        voting_open     = 0;
        memset(candidates, 0, sizeof(candidates));
        memset(voters,     0, sizeof(voters));
        printf("  [✓] Election data has been reset.\n");
    } else {
        printf("  [–] Reset cancelled.\n");
    }
}

/* ─── Main Menu ─────────────────────────────────────────── */
void print_menu(void) {
    printf("\n╔══════════════════════════════════════╗\n");
    printf("║       VOTING SYSTEM SIMULATOR        ║\n");
    printf("║  Status: Voting %-6s | Candidates: %-2d║\n",
           voting_open ? "OPEN" : "CLOSED", candidate_count);
    printf("╠══════════════════════════════════════╣\n");
    printf("║  1. Register Candidate               ║\n");
    printf("║  2. View Candidates                  ║\n");
    printf("║  3. Open / Close Voting              ║\n");
    printf("║  4. Cast Vote                        ║\n");
    printf("║  5. Declare Winner                   ║\n");
    printf("║  6. View Voter Roll                  ║\n");
    printf("║  7. Reset Election                   ║\n");
    printf("║  8. Exit                             ║\n");
    printf("╚══════════════════════════════════════╝\n");
    printf("  Choice: ");
}

int main(void) {
    int choice;
    printf("\n  Welcome to the Voting System Simulator!\n");

    while (1) {
        print_menu();

        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            printf("  [!] Please enter a number from 1–8.\n");
            continue;
        }
        clear_input_buffer();
        printf("\n");

        switch (choice) {
            case 1: register_candidate(); break;
            case 2: display_candidates(); break;
            case 3: toggle_voting();      break;
            case 4: cast_vote();          break;
            case 5: declare_winner();     break;
            case 6: show_voter_roll();    break;
            case 7: reset_election();     break;
            case 8:
                printf("  Goodbye! Election concluded.\n\n");
                return 0;
            default:
                printf("  [!] Invalid option. Choose between 1–8.\n");
        }
    }
}












