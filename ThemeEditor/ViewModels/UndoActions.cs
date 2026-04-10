using System.Collections.Generic;
using ThemeEditor.Models;

namespace ThemeEditor.ViewModels
{
    public class PropertyChangeAction : IUndoableAction
    {
        private readonly PropertyViewModel _property;
        private readonly string _oldValue;
        private string _newValue;

        public PropertyChangeAction(PropertyViewModel property, string oldValue, string newValue)
        {
            _property = property;
            _oldValue = oldValue;
            _newValue = newValue;
        }

        public void Undo() => _property.Value = _oldValue;
        public void Redo() => _property.Value = _newValue;

        public bool TryMerge(IUndoableAction next)
        {
            if (next is PropertyChangeAction pca && pca._property == _property)
            {
                _newValue = pca._newValue;
                return true;
            }
            return false;
        }
    }

    public class MoveWidgetsAction : IUndoableAction
    {
        private readonly List<(WidgetViewModel Widget, int OldX, int OldY, int NewX, int NewY)> _moves;

        public MoveWidgetsAction(List<(WidgetViewModel Widget, int OldX, int OldY, int NewX, int NewY)> moves)
        {
            _moves = moves;
        }

        public void Undo()
        {
            foreach (var (widget, oldX, oldY, _, _) in _moves)
            {
                widget.X = oldX;
                widget.Y = oldY;
            }
        }

        public void Redo()
        {
            foreach (var (widget, _, _, newX, newY) in _moves)
            {
                widget.X = newX;
                widget.Y = newY;
            }
        }

        public bool TryMerge(IUndoableAction next) => false;
    }
}
