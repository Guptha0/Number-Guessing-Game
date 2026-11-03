#include <iostream>
#include <random>
#include <limits>
#include <string>
#include <cstdlib>

enum class GuessStatus {
    TooLow,
    TooHigh,
    Correct,
    OutOfAttempts
};

struct GuessOutcome {
    GuessStatus status;
    int remainingAttempts;
};

class GuessingGame {
private:
    int m_minRange;
    int m_maxRange;
    int m_maxAttempts;
    int m_secretNumber;
    int m_attemptsUsed;
    int m_score;
    bool m_gameOver;

    std::mt19937 m_rng;

public:
    GuessingGame(int minRange = 1, int maxRange = 100, int maxAttempts = 7)
        : m_minRange(minRange),
          m_maxRange(maxRange),
          m_maxAttempts(maxAttempts),
          m_secretNumber(0),
          m_attemptsUsed(0),
          m_score(0),
          m_gameOver(false) 
    {
        std::random_device rd;
        m_rng.seed(rd());
    }

    void startRound() {
        std::uniform_int_distribution<int> dist(m_minRange, m_maxRange);
        m_secretNumber = dist(m_rng);
        m_attemptsUsed = 0;
        m_gameOver = false;
    }

    GuessOutcome evaluateGuess(int guess) {
        m_attemptsUsed++;

        if (guess == m_secretNumber) {
            m_gameOver = true;
            int points = std::max(10, (m_maxAttempts - m_attemptsUsed + 1) * 100);
            m_score += points;
            return { GuessStatus::Correct, m_maxAttempts - m_attemptsUsed };
        }

        if (m_attemptsUsed >= m_maxAttempts) {
            m_gameOver = true;
            return { GuessStatus::OutOfAttempts, 0 };
        }

        if (guess < m_secretNumber) {
            return { GuessStatus::TooLow, m_maxAttempts - m_attemptsUsed };
        } else {
            return { GuessStatus::TooHigh, m_maxAttempts - m_attemptsUsed };
        }
    }

    bool isGameOver() const { return m_gameOver; }
    int getMinRange() const { return m_minRange; }
    int getMaxRange() const { return m_maxRange; }
    int getMaxAttempts() const { return m_maxAttempts; }
    int getSecretNumber() const { return m_secretNumber; }
    int getScore() const { return m_score; }
};

// Safe integer reader preventing stream corruption
static int getValidIntegerInput(int minBound, int maxBound) {
    int value;
    while (true) {
        std::cout << "Enter your guess [" << minBound << " - " << maxBound << "]: ";
        if (std::cin >> value) {
            if (value >= minBound && value <= maxBound) {
                return value;
            }
            std::cout << "[Error] Input out of bounds. Please try again.\n";
        } else {
            if (std::cin.eof()) {
                std::cout << "\n[Stream closed/EOF]. Exiting.\n";
                std::exit(0);
            }
            // Stream entered fail state (e.g. non-numeric input)
            std::cin.clear(); // Reset error state flags (failbit/badbit)
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Purge stream buffer
            std::cout << "[Error] Invalid input detected. Non-numeric data rejected.\n";
        }
    }
}

int main() {
    std::cout << "==================================================\n";
    std::cout << "  Tier 2 (C++ Systems Layer / RAII & Strong Types)\n";
    std::cout << "==================================================\n";

    GuessingGame game(1, 100, 7);
    char playAgain = 'y';

    while (playAgain == 'y' || playAgain == 'Y') {
        game.startRound();
        std::cout << "\n[System] Secret generated between " << game.getMinRange()
                  << " and " << game.getMaxRange() << ".\n";
        std::cout << "[Rules] Attempts permitted: " << game.getMaxAttempts() << "\n\n";

        while (!game.isGameOver()) {
            int guess = getValidIntegerInput(game.getMinRange(), game.getMaxRange());
            GuessOutcome outcome = game.evaluateGuess(guess);

            switch (outcome.status) {
                case GuessStatus::Correct:
                    std::cout << ">> Correct! You guessed the target!\n";
                    break;
                case GuessStatus::TooLow:
                    std::cout << ">> HIGHER! (Attempts remaining: " << outcome.remainingAttempts << ")\n";
                    break;
                case GuessStatus::TooHigh:
                    std::cout << ">> LOWER! (Attempts remaining: " << outcome.remainingAttempts << ")\n";
                    break;
                case GuessStatus::OutOfAttempts:
                    std::cout << ">> Game Over! Target was: " << game.getSecretNumber() << "\n";
                    break;
            }
        }

        std::cout << "\nCurrent Accumulated Score: " << game.getScore() << "\n";
        std::cout << "Play another round? (y/n): ";
        if (!(std::cin >> playAgain)) {
            break;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::cout << "Exiting application.\n";
    return 0;
}
