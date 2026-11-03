using System;

namespace NumberGuessingGame
{
    public class GuessResult
    {
        public bool IsCorrect { get; set; }
        public string Feedback { get; set; } = string.Empty;
        public int AttemptsUsed { get; set; }
    }

    public class GameManager
    {
        private readonly int _minRange;
        private readonly int _maxRange;
        private readonly int _maxAttempts;
        private int _secretNumber;
        private int _currentAttempts;
        private bool _isGameOver;

        public int Score { get; private set; }

        public GameManager(int minRange = 1, int maxRange = 100, int maxAttempts = 7)
        {
            _minRange = minRange;
            _maxRange = maxRange;
            _maxAttempts = maxAttempts;
            _secretNumber = 0;
            _currentAttempts = 0;
            _isGameOver = false;
            Score = 0;
        }

        public void StartNewGame()
        {
            _secretNumber = Random.Shared.Next(_minRange, _maxRange + 1);
            _currentAttempts = 0;
            _isGameOver = false;
        }

        public GuessResult EvaluateGuess(int guess)
        {
            if (_isGameOver)
            {
                throw new InvalidOperationException("Game is already over. Start a new game first.");
            }

            _currentAttempts++;

            if (guess == _secretNumber)
            {
                _isGameOver = true;
                int roundScore = Math.Max(10, (_maxAttempts - _currentAttempts + 1) * 100);
                Score += roundScore;

                return new GuessResult
                {
                    IsCorrect = true,
                    Feedback = $"Spot on! You discovered {_secretNumber} in {_currentAttempts} attempts.",
                    AttemptsUsed = _currentAttempts
                };
            }

            if (_currentAttempts >= _maxAttempts)
            {
                _isGameOver = true;
                return new GuessResult
                {
                    IsCorrect = false,
                    Feedback = $"Out of attempts! The hidden number was {_secretNumber}.",
                    AttemptsUsed = _currentAttempts
                };
            }

            string hint = guess < _secretNumber ? "HIGHER" : "LOWER";
            return new GuessResult
            {
                IsCorrect = false,
                Feedback = $"Incorrect! Go {hint}. [Remaining attempts: {_maxAttempts - _currentAttempts}]",
                AttemptsUsed = _currentAttempts
            };
        }

        public bool IsGameOver => _isGameOver;
        public int MinRange => _minRange;
        public int MaxRange => _maxRange;
        public int MaxAttempts => _maxAttempts;
    }

    public static class Program
    {
        public static void Main(string[] args)
        {
            Console.Title = "Tier 1: High-Level C# Number Guessing Game";
            Console.WriteLine("==================================================");
            Console.WriteLine("  Tier 1 (C# Managed Runtime / High-Level OOP)    ");
            Console.WriteLine("==================================================");

            var manager = new GameManager(minRange: 1, maxRange: 100, maxAttempts: 7);
            bool playAgain = true;

            while (playAgain)
            {
                manager.StartNewGame();
                Console.WriteLine($"\n[System] A secret number has been chosen between {manager.MinRange} and {manager.MaxRange}.");
                Console.WriteLine($"[Rules] You have {manager.MaxAttempts} attempts.");

                while (!manager.IsGameOver)
                {
                    Console.Write("Enter your guess: ");
                    string? input = Console.ReadLine();

                    if (input == null)
                    {
                        Console.WriteLine("\n[EOF detected]. Exiting.");
                        return;
                    }

                    if (!int.TryParse(input, out int guess))
                    {
                        Console.ForegroundColor = ConsoleColor.Yellow;
                        Console.WriteLine("[Invalid Input] Please enter a valid integer.");
                        Console.ResetColor();
                        continue;
                    }

                    if (guess < manager.MinRange || guess > manager.MaxRange)
                    {
                        Console.ForegroundColor = ConsoleColor.Yellow;
                        Console.WriteLine($"[Out of Range] Guess must be between {manager.MinRange} and {manager.MaxRange}.");
                        Console.ResetColor();
                        continue;
                    }

                    var result = manager.EvaluateGuess(guess);

                    if (result.IsCorrect)
                    {
                        Console.ForegroundColor = ConsoleColor.Green;
                        Console.WriteLine($"[SUCCESS] {result.Feedback}");
                        Console.ResetColor();
                    }
                    else
                    {
                        Console.ForegroundColor = ConsoleColor.Cyan;
                        Console.WriteLine($"[HINT] {result.Feedback}");
                        Console.ResetColor();
                    }
                }

                Console.WriteLine($"Total Score: {manager.Score}");
                Console.Write("\nPlay another round? (y/n): ");
                string? response = Console.ReadLine();
                playAgain = response?.Trim().Equals("y", StringComparison.OrdinalIgnoreCase) ?? false;
            }

            Console.WriteLine("Thank you for playing!");
        }
    }
}
