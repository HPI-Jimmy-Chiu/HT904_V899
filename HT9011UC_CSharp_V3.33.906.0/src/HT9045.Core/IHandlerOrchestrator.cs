using System;

namespace HT9045.Core
{
    // The handler's top-level cycle driver. In the BCB6 source this logic was
    // spread across cKernel / main.cpp scan loops; the C# port consolidates a
    // single offline-able cycle entry point behind this interface.
    public interface IHandlerOrchestrator
    {
        // Runs one representative pick -> test -> bin cycle. Returns true on a
        // clean cycle, false if a gate (license / alarm) stopped it.
        bool RunOneCycle();

        event EventHandler<ConfigChangedEventArgs> ConfigChanged;
        event EventHandler<TaskCompletedEventArgs> TaskCompleted;
        event EventHandler<AlarmOccuredEventArgs> AlarmOccured;
    }
}
