using System.IO;
using System.Threading;

public class Watcher
{
    public Watcher(string path, string filter)
    {
        watch = new FileSystemWatcher(path, filter);
        watch.NotifyFilter = NotifyFilters.CreationTime | NotifyFilters.LastWrite | NotifyFilters.FileName;
        watch.Created += delegate { changedFile = true; };
        watch.Changed += delegate { changedFile = true; };
        watch.Renamed += delegate { changedFile = true; };
        watch.EnableRaisingEvents = true;
    }

    public bool Changed()
    {
        Thread.Sleep(changedFile ? 500 : 1000);

        if (changedFile)
        {
            Thread.Sleep(500);  // to settle
            changedFile = false;
            return true;
        }

        return false;
    }

    FileSystemWatcher watch;
    bool changedFile = false;
}
