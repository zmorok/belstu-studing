using System;

public class Game
{
    public delegate void AttackHandler(Player player, int dmg);
    public delegate void HealHandler(Player player, int heal);

    public event AttackHandler Attack;
    public event HealHandler Heal;

    public void OnAttack(Player player, int damage)
    {
        Console.WriteLine($"Атака на {player.Name} с уроном: {damage}");
        Attack?.Invoke(player, damage);
    }

    public void OnHeal(Player player, int healAmount)
    {
        Console.WriteLine($"Лечение {player.Name} на: {healAmount}");
        Heal?.Invoke(player, healAmount);
    }
}

public class Player
{
    public string Name { get; set; }
    public int Health { get; set; } = 100;
    private const int MaxHealth = 500;
    private bool isAlive = true;
    private static Game _game;

    public Player(string name)
    {
        Name = name;

        if (_game != null)
        {
            _game.Attack += OnAttacked;
            _game.Heal += OnHealed;
        }
    }

    public static void SetGame(Game game)
    {
        _game = game;
    }

    public override string ToString() =>
        $"{GetType().Name}: {Name} | {(isAlive ? Health + " HP." : "Мёртв.")}";

    public void OnAttacked(Player player, int damage)
    {
        if (player == this)
        {
            Health -= damage;
            if (Health <= 0)
            {
                Health = 0;
                isAlive = false;
                Console.WriteLine(
                    $"{Name} получил урон: {damage}. Текущий уровень жизни: {Health}. Состояние: мёртв"
                );
            }
            else
            {
                Console.WriteLine(
                    $"{Name} получил урон: {damage}. Текущий уровень жизни: {Health}"
                );
            }
        }
    }

    public void OnHealed(Player player, int healAmount)
    {
        if (player == this)
        {
            if (!isAlive)
                isAlive = true;

            Health += healAmount;

            if (Health > MaxHealth)
                Health = MaxHealth;

            Console.WriteLine(
                $"{Name} был исцелен на: {healAmount}. Текущий уровень жизни: {Health}"
            );
        }
    }
}

public class GameActions
{
    private readonly Game _game;
    private readonly Player[] _players;

    public GameActions(Game game, Player[] players)
    {
        _game = game;
        _players = players;
    }

    public void AttackPlayer()
    {
        Console.Write("Выберите игрока для атаки (1 или 2): ");
        if (
            int.TryParse(Console.ReadLine(), out int attackChoice)
            && attackChoice >= 1
            && attackChoice <= 2
        )
        {
            Console.Write("Введите количество урона: ");
            if (int.TryParse(Console.ReadLine(), out int damage))
            {
                _game.OnAttack(_players[attackChoice - 1], damage);
            }
            else
            {
                Console.WriteLine("Некорректный ввод.");
            }
        }
        else
        {
            Console.WriteLine("Некорректный выбор игрока.");
        }
        Console.WriteLine("Нажмите любую клавишу, чтобы продолжить...");
        Console.ReadKey();
    }

    public void HealPlayer()
    {
        Console.Write("Выберите игрока для лечения (1 или 2): ");
        if (
            int.TryParse(Console.ReadLine(), out int healChoice)
            && healChoice >= 1
            && healChoice <= 2
        )
        {
            Console.Write("Введите количество лечения: ");
            if (int.TryParse(Console.ReadLine(), out int healAmount))
            {
                _game.OnHeal(_players[healChoice - 1], healAmount);
            }
            else
            {
                Console.WriteLine("Некорректный ввод.");
            }
        }
        else
        {
            Console.WriteLine("Некорректный выбор игрока.");
        }
        Console.WriteLine("Нажмите любую клавишу, чтобы продолжить...");
        Console.ReadKey();
    }

    public void PlayersStatus()
    {
        Console.Clear();
        Console.WriteLine($"ИГРОКИ:");
        foreach (var player in _players)
            Console.WriteLine(player);
    }

    public void DisplayPlayerStatus()
    {
        Console.WriteLine($"\nТекущее состояние игроков:");
        foreach (var player in _players)
            Console.WriteLine(player);
        Console.WriteLine("Для закрытия нажмите любую клавишу. . .");
        Console.ReadKey();
    }
}

class Program
{
    static void Main(string[] args)
    {
        Game game = new Game();
        Player.SetGame(game);

        Player player1 = new Player("Игрок 1");
        Player player2 = new Player("Игрок 2");

        Player[] players = { player1, player2 };

        GameActions gameActions = new GameActions(game, players);

        while (true)
        {
            gameActions.PlayersStatus();
            Console.WriteLine("\nМеню:");
            Console.WriteLine("1. Атаковать игрока");
            Console.WriteLine("2. Лечить игрока");
            Console.WriteLine("3. Проверить состояние игроков");
            Console.WriteLine("0. Выход");
            Console.Write("Выберите действие: ");
            string choice = Console.ReadLine();

            switch (choice)
            {
                case "1":
                {
                    gameActions.AttackPlayer();
                    break;
                }

                case "2":
                {
                    gameActions.HealPlayer();
                    break;
                }

                case "3":
                {
                    gameActions.DisplayPlayerStatus();
                    break;
                }

                case "0":
                {
                    Console.WriteLine("Выход из игры.");
                    return;
                }
                default:
                {
                    Console.WriteLine("Некорректный выбор.");
                    Console.WriteLine("Нажмите любую клавишу, чтобы продолжить...");
                    Console.ReadKey();
                    break;
                }
            }
        }
    }
}
