using System;

namespace HT9045.Core
{
    // Domain event args raised by the orchestrator / domain services.
    // (BCB6 used direct callbacks + WM_COPYDATA; the C# port surfaces these
    // as .NET events so adapters and views can subscribe without coupling.)

    public class ConfigChangedEventArgs : EventArgs
    {
        // The recipe / setup file that became active (PROD_INFO_ST.cFileName).
        public string CFileName;

        public ConfigChangedEventArgs(string cFileName)
        {
            CFileName = cFileName;
        }
    }

    public class TaskCompletedEventArgs : EventArgs
    {
        // Name of the cycle / task step that finished (e.g. "RunOneCycle").
        public string TaskName;
        public bool Success;

        public TaskCompletedEventArgs(string taskName, bool success)
        {
            TaskName = taskName;
            Success = success;
        }
    }

    public class AlarmOccuredEventArgs : EventArgs
    {
        // Mirrors the C++ alarm code convention (e.g. WAR / JAM numeric codes).
        public int AlarmCode;
        public string Message;

        public AlarmOccuredEventArgs(int alarmCode, string message)
        {
            AlarmCode = alarmCode;
            Message = message;
        }
    }
}
