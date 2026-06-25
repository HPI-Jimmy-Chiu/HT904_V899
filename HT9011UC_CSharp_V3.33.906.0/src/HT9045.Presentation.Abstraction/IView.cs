namespace HT9045.Presentation.Abstraction
{
    // Base view contract. Stub (headless) and WinForms views both implement it.
    // Kept deliberately tiny so Phase-1 stubs are trivial.
    public interface IView
    {
        // Show / activate the view surface.
        void Show();

        // Tear down the view surface.
        void Close();
    }

    // Main handler screen. Mirrors what fMain exposed in the BCB6 UI: a status
    // line and per-axis motor position readout. Signatures use only primitives.
    public interface IMainView : IView
    {
        void ShowStatus(string msg);

        void UpdateMotorScreen(int axis, int pos);
    }
}
