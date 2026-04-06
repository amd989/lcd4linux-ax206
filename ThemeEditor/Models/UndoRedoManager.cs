using System.Collections.Generic;

namespace ThemeEditor.Models
{
    public interface IUndoableAction
    {
        void Undo();
        void Redo();
        bool TryMerge(IUndoableAction next);
    }

    public class UndoRedoManager
    {
        private readonly Stack<IUndoableAction> _undoStack = new();
        private readonly Stack<IUndoableAction> _redoStack = new();

        public bool IsUndoRedoing { get; private set; }
        public bool CanUndo => _undoStack.Count > 0;
        public bool CanRedo => _redoStack.Count > 0;

        public void Push(IUndoableAction action)
        {
            if (IsUndoRedoing) return;

            if (_undoStack.Count > 0 && _undoStack.Peek().TryMerge(action))
                return;

            _undoStack.Push(action);
            _redoStack.Clear();
        }

        public void Undo()
        {
            if (!CanUndo) return;
            IsUndoRedoing = true;
            try
            {
                var action = _undoStack.Pop();
                action.Undo();
                _redoStack.Push(action);
            }
            finally { IsUndoRedoing = false; }
        }

        public void Redo()
        {
            if (!CanRedo) return;
            IsUndoRedoing = true;
            try
            {
                var action = _redoStack.Pop();
                action.Redo();
                _undoStack.Push(action);
            }
            finally { IsUndoRedoing = false; }
        }

        public void Clear()
        {
            _undoStack.Clear();
            _redoStack.Clear();
        }
    }
}
