using EPortal.Properties;

static class Config
{
    public static readonly int Wake_Interval;
    public static readonly WorkTime Work_Time = new WorkTime("Work_Start", "Work_Final");

    public static void Init() { }  // Invoke ctor

    static Config()
    {
        Log.Init("EPortal", Settings.Default.EPortal_Temp);
        Work_Time.GetStart(Settings.Default.Work_Start);
        Work_Time.GetFinal(Settings.Default.Work_Final);
        Wake_Interval = Range.Get("Wake_Interval", 1, 60, Settings.Default.Wake_Interval) * 60;
        Work_Time.CheckMinimal(10);
    }
}
