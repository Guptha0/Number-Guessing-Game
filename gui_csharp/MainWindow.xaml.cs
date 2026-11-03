using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using System.Windows.Threading;

namespace AdvancedNumberGuessing
{
    public enum GameMode { Classic, TimeAttack, Endurance }

    public partial class MainWindow : Window
    {
        private GameMode _currentMode = GameMode.Classic;
        private int _minBound = 1;
        private int _maxBound = 100;
        private int _maxAttempts = 7;
        private int _attemptsRemaining;
        private int _secretTarget;
        private int _score = 0;
        private int _winStreak = 0;
        private bool _isRoundActive = false;
        private bool _isInitialized = false;

        // High-Precision Dispatcher Timer for Time Attack
        private DispatcherTimer? _gameTimer;
        private double _timeRemainingSeconds;
        private const double InitialTimeLimit = 30.0;

        private readonly Random _rand = new();

        public MainWindow()
        {
            InitializeComponent();
            _isInitialized = true;
            InitializeTimer();
            StartNewRound();
        }

        private void InitializeTimer()
        {
            _gameTimer = new DispatcherTimer(DispatcherPriority.Render)
            {
                Interval = TimeSpan.FromMilliseconds(50)
            };
            _gameTimer.Tick += GameTimer_Tick;
        }

        private void StartNewRound()
        {
            if (!_isInitialized) return;
            if (_currentMode == GameMode.Endurance && _winStreak > 0)
            {
                // Dynamic Range Expansion: 1-100 -> 1-150 -> 1-225
                _maxBound = (int)(100 * Math.Pow(1.5, _winStreak));
                _maxAttempts = 7 + (_winStreak * 2);
            }
            else
            {
                _minBound = 1;
                _maxBound = 100;
                _maxAttempts = 7;
            }

            _secretTarget = _rand.Next(_minBound, _maxBound + 1);
            _attemptsRemaining = _maxAttempts;
            _isRoundActive = true;

            TxtRangeIndicator.Text = $"TARGET RANGE: [{_minBound} — {_maxBound}]";
            TxtAttemptsLeft.Text = _attemptsRemaining.ToString();
            TxtFeedback.Text = "Awaiting first input...";
            FeedbackBadge.Background = new SolidColorBrush(Color.FromRgb(33, 38, 45));
            HeatBar.Value = 0;
            TxtHeatValue.Text = "0%";
            ListAttempts.Items.Clear();
            ParticleCanvas.Children.Clear();
            InputGuess.Text = string.Empty;
            InputGuess.Focus();

            if (_currentMode == GameMode.TimeAttack)
            {
                _timeRemainingSeconds = InitialTimeLimit;
                TxtTimer.Visibility = Visibility.Visible;
                TxtTimer.Text = $"TIME: {_timeRemainingSeconds:F1}s";
                _gameTimer?.Start();
            }
            else
            {
                TxtTimer.Visibility = Visibility.Collapsed;
                _gameTimer?.Stop();
            }
        }

        private void GameTimer_Tick(object? sender, EventArgs e)
        {
            _timeRemainingSeconds -= 0.05;
            if (_timeRemainingSeconds <= 0)
            {
                _timeRemainingSeconds = 0;
                _gameTimer?.Stop();
                RoundLost("Time expired! Defeat.");
            }
            TxtTimer.Text = $"TIME: {_timeRemainingSeconds:F1}s";
        }

        private void SubmitGuess_Click(object sender, RoutedEventArgs e) => ProcessGuess();

        private void InputGuess_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter) ProcessGuess();
        }

        private void ProcessGuess()
        {
            if (!_isRoundActive) return;

            if (!int.TryParse(InputGuess.Text.Trim(), out int guess) || guess < _minBound || guess > _maxBound)
            {
                TriggerScreenShake();
                TxtFeedback.Text = $"[Error] Please enter an integer between {_minBound} and {_maxBound}.";
                return;
            }

            _attemptsRemaining--;
            TxtAttemptsLeft.Text = _attemptsRemaining.ToString();

            // Calculate Heat Radar Proximity Metric [0.0 - 1.0]
            int distance = Math.Abs(guess - _secretTarget);
            int maxDistance = Math.Max(_secretTarget - _minBound, _maxBound - _secretTarget);
            double proximity = 1.0 - ((double)distance / maxDistance);
            proximity = Math.Clamp(proximity, 0.0, 1.0);
            
            HeatBar.Value = proximity * 100;
            TxtHeatValue.Text = $"{HeatBar.Value:F0}%";

            if (guess == _secretTarget)
            {
                RoundWon();
                return;
            }

            // Wrong Guess: Log attempt & apply feedback
            TriggerScreenShake();
            string direction = guess < _secretTarget ? "HIGHER ▲" : "LOWER ▼";
            ListAttempts.Items.Insert(0, $"#{_maxAttempts - _attemptsRemaining:D2} | Guess: {guess:D3} -> {direction} ({proximity * 100:F0}% Heat)");

            TxtFeedback.Text = $"Target is {direction}!";
            FeedbackBadge.Background = new SolidColorBrush(Color.FromRgb(56, 139, 253));

            if (_attemptsRemaining <= 0)
            {
                RoundLost($"Exceeded attempts! Target was {_secretTarget}.");
            }

            InputGuess.SelectAll();
        }

        private void RoundWon()
        {
            _isRoundActive = false;
            _gameTimer?.Stop();
            _winStreak++;

            // Exponential Multiplier based on speed, mode, and remaining attempts
            int baseScore = (_attemptsRemaining + 1) * 200;
            int modeBonus = _currentMode == GameMode.TimeAttack ? (int)(_timeRemainingSeconds * 20) : 100;
            int streakMultiplier = Math.Max(1, _winStreak);
            int totalRoundPoints = (baseScore + modeBonus) * streakMultiplier;

            _score += totalRoundPoints;
            TxtScore.Text = _score.ToString();
            TxtStreak.Text = $"{_winStreak}x";

            TxtFeedback.Text = $"TARGET ACQUIRED! +{totalRoundPoints} PTS!";
            FeedbackBadge.Background = new SolidColorBrush(Color.FromRgb(35, 134, 54));

            SpawnConfettiStorm();
        }

        private void RoundLost(string reason)
        {
            _isRoundActive = false;
            _gameTimer?.Stop();
            _winStreak = 0;
            TxtStreak.Text = "0x";

            TriggerScreenShake();
            TxtFeedback.Text = reason;
            FeedbackBadge.Background = new SolidColorBrush(Color.FromRgb(218, 54, 51));
        }

        private void TriggerScreenShake()
        {
            if (FindResource("ShakeAnimation") is Storyboard sb)
            {
                sb.Begin(this);
            }
        }

        private void SpawnConfettiStorm()
        {
            Color[] colors = { Color.FromRgb(0, 210, 255), Color.FromRgb(255, 42, 84), Color.FromRgb(255, 184, 0), Color.FromRgb(126, 231, 135) };
            for (int i = 0; i < 70; i++)
            {
                var rect = new Rectangle
                {
                    Width = _rand.Next(6, 12),
                    Height = _rand.Next(6, 12),
                    Fill = new SolidColorBrush(colors[_rand.Next(colors.Length)]),
                    RenderTransform = new RotateTransform(_rand.Next(360))
                };

                double startX = _rand.Next(100, 800);
                Canvas.SetLeft(rect, startX);
                Canvas.SetTop(rect, -20);
                ParticleCanvas.Children.Add(rect);

                var fallAnim = new DoubleAnimation
                {
                    From = -20,
                    To = 700 + _rand.Next(100),
                    Duration = TimeSpan.FromSeconds(1.2 + _rand.NextDouble() * 1.5),
                    EasingFunction = new QuadraticEase { EasingMode = EasingMode.EaseIn }
                };

                var driftAnim = new DoubleAnimation
                {
                    From = startX,
                    To = startX + _rand.Next(-150, 150),
                    Duration = fallAnim.Duration
                };

                rect.BeginAnimation(Canvas.TopProperty, fallAnim);
                rect.BeginAnimation(Canvas.LeftProperty, driftAnim);
            }
        }

        private void ModeChanged(object sender, RoutedEventArgs e)
        {
            if (!_isInitialized) return;

            if (RbClassic?.IsChecked == true) _currentMode = GameMode.Classic;
            else if (RbTimeAttack?.IsChecked == true) _currentMode = GameMode.TimeAttack;
            else if (RbEndurance?.IsChecked == true) _currentMode = GameMode.Endurance;

            _winStreak = 0;
            StartNewRound();
        }

        private void NewRound_Click(object sender, RoutedEventArgs e) => StartNewRound();
    }
}
