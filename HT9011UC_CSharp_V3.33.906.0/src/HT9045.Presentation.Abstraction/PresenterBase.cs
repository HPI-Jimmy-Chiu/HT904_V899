namespace HT9045.Presentation.Abstraction
{
    // Minimal MVP presenter base. Generic over the view contract so concrete
    // presenters bind to a specific IView without this assembly needing to
    // reference Core (keeps the Core <-> Presentation dependency one-way).
    public abstract class PresenterBase<TView> where TView : IView
    {
        protected readonly TView View;

        protected PresenterBase(TView view)
        {
            View = view;
        }

        // Called once after construction to wire up / refresh the view.
        public abstract void Initialize();
    }
}
