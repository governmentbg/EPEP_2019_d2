using System.ServiceProcess;
using System.Threading;

partial class EPortalService : ServiceBase
{
    public EPortalService()
    {
        InitializeComponent();
    }

    protected override void OnStart(string[] args)
    {
        if (workerThread == null)
        {
            workerThread = new Thread(new ThreadStart(Program.MainLoop));
            workerThread.Start();
        }
    }

    protected override void OnStop()
    {
        if (workerThread != null)
            workerThread.Abort();
    }

    private Thread workerThread;
}
