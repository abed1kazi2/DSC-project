# 🗳️ Voting System Simulator

A terminal-based election management system written in C. It lets you register candidates, manage a voting session, cast votes with duplicate-prevention, and declare a winner with a visual results breakdown.

---

## Features

- **Candidate Registration** — Add up to 10 candidates (case-insensitive duplicate detection)
- **Voting Session Control** — Open or close the voting window at any time
- **Ballot Casting** — Voters identified by unique Voter IDs; each ID can vote only once
- **Election Results** — Visual bar chart, vote percentages, and winner declaration (tie handling included)
- **Voter Roll** — View a full list of all Voter IDs that have participated
- **Election Reset** — Wipe all data and start a fresh election (with confirmation prompt)

---

## Requirements

- A C compiler: `gcc`, `clang`, or compatible
- C standard: **C99** or later
- No external libraries required

---

## Building

```bash
gcc -std=c99 -Wall -o voting_system voting_system.c
```

---

## Running

```bash
./voting_system
```

---

## Usage

On launch, you are presented with an interactive menu:

```
╔══════════════════════════════════════╗
║        VOTING SYSTEM SIMULATOR       ║
║  Status: Voting CLOSED | Candidates: 0 ║
╠══════════════════════════════════════╣
║  1. Register Candidate               ║
║  2. View Candidates                  ║
║  3. Open / Close Voting              ║
║  4. Cast Vote                        ║
║  5. Declare Winner                   ║
║  6. View Voter Roll                  ║
║  7. Reset Election                   ║
║  8. Exit                             ║
╚══════════════════════════════════════╝
```

### Typical Election Flow

1. **Register at least 2 candidates** (Option 1)
2. **Open voting** (Option 3)
3. **Cast votes** — each voter enters their unique Voter ID and picks a candidate (Option 4)
4. **Close voting** (Option 3)
5. **Declare the winner** to see results with a bar chart (Option 5)

---

## Constraints

| Parameter        | Limit |
|------------------|-------|
| Max Candidates   | 10    |
| Max Voters       | 100   |
| Max Name Length  | 50 chars |
| Max Voter ID Length | 20 chars |

---

## Project Structure
 
```
Configuration, Headers, & Data Structures    # Macros, structs, and global declarations
void register candidate                      # Candidate registration logic
Void caste vote and disply cndidate          # Vote casting and candidate display logic
declare winner , show voter roll & toggle voting  # Winner declaration, voter roll, voting toggle
display candidates & reset election          # Display helpers and election reset logic
README.md                                    # This file
```


### Key Components

| Component | Description |
|-----------|-------------|
| `Candidate` struct | Stores name, vote count, and ID |
| `Voter` struct | Stores Voter ID and voted status |
| `register_candidate()` | Adds a candidate with duplicate check |
| `cast_vote()` | Validates voter, records ballot |
| `declare_winner()` | Computes results, handles ties |
| `reset_election()` | Clears all state after confirmation |

---

## Notes

- Candidate names are compared **case-insensitively** to prevent duplicates like `Alice` and `ALICE`.
- Voting cannot be opened with fewer than 2 registered candidates.
- New candidates cannot be registered while voting is open.
- The winner declaration is blocked while voting is still open.
- On a tie, all tied candidates are listed and a run-off is suggested.

---

## License

This project is released for educational use. No license restrictions apply.
