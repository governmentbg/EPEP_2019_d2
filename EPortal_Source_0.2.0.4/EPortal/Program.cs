using System;
using System.ServiceProcess;
using System.Threading;

class Program
{
    static int WaitSeconds = 0;

    static bool HandleKeys(ConsoleKey key)
    {
        if (key == ConsoleKey.P)
        {
            Transfer.SendData();
            WaitSeconds = 1;
        }
        else if (key == ConsoleKey.Q)
        {
            Log.Info("quit");
            return false;
        }

        return true;
    }

    public static void MainLoop()
    {
        try
        {
            Config.Init();
            Const.Init();
            Log.Info(Machine.Interactive ? "initialized" : "(initialized)");

            Machine.KeyHandler handler = new Machine.KeyHandler(HandleKeys);

            while (Machine.HandleKeys(handler))
            {
                if (Config.Work_Time.Paused())
                    WaitSeconds = -1;
                else
                    Thread.Sleep(1000);

                if (WaitSeconds++ % Config.Wake_Interval == 0)
                    Transfer.SendData();
            }
        }
        catch (ThreadAbortException)
        {
            Log.Info("stopped");
        }
        catch (Exception ex)
        {
            Log.Final(ex);
        }
    }

    static void Main(string[] args)
    {
        if (Machine.Interactive)
        {
            Console.CancelKeyPress += delegate { Log.Info("cancelled"); };
            MainLoop();
        }
        else
        {
            ServiceBase[] ServicesToRun;
            ServicesToRun = new ServiceBase[] { new EPortalService() };
            ServiceBase.Run(ServicesToRun);
        }
    }
}