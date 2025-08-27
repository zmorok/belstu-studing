using System.Collections.ObjectModel;

namespace FreelanceApp.Services
{
    public class UndoRedoManager<T>
    {
        private readonly Func<T, T> cloneFunc;
        private readonly Action<T, T> applyFunc;

        private readonly Stack<(string Action, T Before, T? After)> undoStack = new();
        private readonly Stack<(string Action, T Before, T? After)> redoStack = new();

        public UndoRedoManager(Func<T, T> cloneFunc, Action<T, T> applyFunc)
        {
            this.cloneFunc = cloneFunc;
            this.applyFunc = applyFunc;
        }

        public void Push(string action, T before, T? after = default)
        {
            undoStack.Push((action, cloneFunc(before), after != null ? cloneFunc(after) : default));
            redoStack.Clear();
        }

        public void Undo(ObservableCollection<T> collection)
        {
            if (undoStack.Count == 0) return;

            var (action, before, after) = undoStack.Pop();
            redoStack.Push((action, cloneFunc(before), after != null ? cloneFunc(after) : default));

            switch (action)
            {
                case "add":
                    collection.Remove(collection.FirstOrDefault(x => Equals(x, before))!);
                    break;
                case "delete":
                    collection.Add(before);
                    break;
                case "edit":
                    var itemToEdit = collection.FirstOrDefault(x => Equals(x, before));
                    if (itemToEdit != null && after != null)
                        applyFunc(itemToEdit, before);
                    break;
            }
        }

        public void Redo(ObservableCollection<T> collection)
        {
            if (redoStack.Count == 0) return;

            var (action, before, after) = redoStack.Pop();
            undoStack.Push((action, cloneFunc(before), after != null ? cloneFunc(after) : default));

            switch (action)
            {
                case "add":
                    collection.Add(before);
                    break;
                case "delete":
                    collection.Remove(collection.FirstOrDefault(x => Equals(x, before))!);
                    break;
                case "edit":
                    var itemToEdit = collection.FirstOrDefault(x => Equals(x, before));
                    if (itemToEdit != null && after != null)
                        applyFunc(itemToEdit, after);
                    break;
            }
        }
    }

    public class UndoRedoAction<T>
    {
        public string ActionType { get; set; } = "";
        public T Snapshot { get; set; } = default!;
    }
}
