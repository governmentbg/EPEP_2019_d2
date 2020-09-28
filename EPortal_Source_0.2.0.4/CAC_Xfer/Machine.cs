using System;
using System.Reflection;

public static class Machine
{
    public static readonly bool HasEntryPoint;
    public static readonly bool Interactive;

    static Machine()
    {
        HasEntryPoint = Assembly.GetEntryAssembly() != null && Assembly.GetEntryAssembly().EntryPoint != null;

        if (HasEntryPoint)
        {
            try
            {
                Interactive = Console.Title != null && (!Console.KeyAvailable || Console.KeyAvailable);
            }
            catch
            {
                Interactive = false;
            }
        }
        else
            Interactive = false;
    }

    public static void Display(string message)
    {
        if (Interactive)
            Console.WriteLine(message);
    }

    public static void Terminate()
    {
        if (HasEntryPoint)
            Environment.Exit(1);
    }

    public delegate bool KeyHandler(ConsoleKey key);

    public static bool HandleKeys(KeyHandler handler)
    {
        if (Interactive)
        {
            while (Console.KeyAvailable)
            {
                ConsoleKey key = Console.ReadKey(true).Key;

                if (!handler(key))
                    return false;
            }
        }

        return true;
    }
}
