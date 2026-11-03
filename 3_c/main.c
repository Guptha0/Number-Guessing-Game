#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MIN_RANGE 1
#define MAX_RANGE 100
#define MAX_ATTEMPTS 7

/* Drain any trailing or malformed characters from standard input stream buffer */
static void flush_stdin_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* Discard input byte */
    }
}

/* Safely acquire integer with bounds checking and buffer sanitation */
static int read_valid_integer(int min_val, int max_val) {
    int value;
    int items_read;

    while (1) {
        printf("Enter guess (%d - %d): ", min_val, max_val);
        items_read = scanf("%d", &value);

        if (items_read == EOF) {
            printf("\n[EOF reached. Exiting program]\n");
            exit(0);
        }

        if (items_read != 1) {
            printf("[Buffer Error] Invalid numeric input encountered.\n");
            flush_stdin_buffer();
            continue;
        }

        // Clean any leftover characters on the line (e.g. trailing newline or chars)
        flush_stdin_buffer();

        if (value < min_val || value > max_val) {
            printf("[Range Error] Guess outside defined limits.\n");
            continue;
        }

        return value;
    }
}

int main(void) {
    int secret_number;
    int attempts_used;
    int current_guess;
    int total_score = 0;
    char play_again = 'y';

    // Seed PRNG with epoch timestamp
    srand((unsigned int)time(NULL));

    printf("==================================================\n");
    printf("  Tier 3 (C Procedural / Explicit Memory & Buffer)\n");
    printf("==================================================\n");

    while (play_again == 'y' || play_again == 'Y') {
        // Linear Congruential PRNG range mapping
        secret_number = (rand() % (MAX_RANGE - MIN_RANGE + 1)) + MIN_RANGE;
        attempts_used = 0;

        printf("\n[Engine] Target locked between %d and %d. Max attempts: %d\n", 
               MIN_RANGE, MAX_RANGE, MAX_ATTEMPTS);

        while (attempts_used < MAX_ATTEMPTS) {
            current_guess = read_valid_integer(MIN_RANGE, MAX_RANGE);
            attempts_used++;

            if (current_guess == secret_number) {
                int round_score = (MAX_ATTEMPTS - attempts_used + 1) * 100;
                total_score += round_score;
                printf("[SUCCESS] Match found in %d attempts! Earned: %d pts.\n", 
                       attempts_used, round_score);
                break;
            } else if (current_guess < secret_number) {
                printf(">> HIGHER! [%d attempts remaining]\n", MAX_ATTEMPTS - attempts_used);
            } else {
                printf(">> LOWER!  [%d attempts remaining]\n", MAX_ATTEMPTS - attempts_used);
            }

            if (attempts_used == MAX_ATTEMPTS) {
                printf("[DEFEAT] Reached attempt limit. Secret number was: %d\n", secret_number);
            }
        }

        printf("Cumulative Score: %d\n", total_score);
        printf("Play again? (y/n): ");
        if (scanf(" %c", &play_again) != 1) {
            break;
        }
        flush_stdin_buffer();
    }

    printf("Exiting procedural session.\n");
    return 0;
}
