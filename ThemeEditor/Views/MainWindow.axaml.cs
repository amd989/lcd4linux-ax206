using System;
using System.Collections.Generic;
using System.Linq;
using Avalonia;
using Avalonia.Controls;
using Avalonia.Input;
using Avalonia.Interactivity;
using Avalonia.Platform.Storage;
using Avalonia.VisualTree;
using ThemeEditor.ViewModels;

namespace ThemeEditor.Views
{
    public partial class MainWindow : Window
    {
        private bool _isDraggingWidgets = false;
        private bool _isDrawingSelection = false;
        private Point _dragStartPoint;
        private List<(WidgetViewModel Widget, int X, int Y)>? _dragStartPositions;

        public MainWindow()
        {
            InitializeComponent();
            AddHandler(KeyDownEvent, OnTunnelKeyDown, RoutingStrategies.Tunnel);
        }

        private void OnTunnelKeyDown(object? sender, KeyEventArgs e)
        {
            if (!e.KeyModifiers.HasFlag(KeyModifiers.Control)) return;

            var vm = DataContext as MainWindowViewModel;
            if (vm == null) return;

            if (e.Key == Key.Z) { vm.UndoManager.Undo(); e.Handled = true; }
            else if (e.Key == Key.Y) { vm.UndoManager.Redo(); e.Handled = true; }
        }

        private async void About_Click(object? sender, RoutedEventArgs e)
        {
            await new AboutDialog().ShowDialog(this);
        }

        private async void LoadTheme_Click(object sender, RoutedEventArgs e)
        {
            var topLevel = TopLevel.GetTopLevel(this);
            if (topLevel == null) return;

            var files = await topLevel.StorageProvider.OpenFilePickerAsync(new FilePickerOpenOptions
            {
                Title = "Open LCD4Linux Config",
                AllowMultiple = false,
                FileTypeFilter = new[] { new FilePickerFileType("Config Files") { Patterns = new[] { "*.conf" } } }
            });

            if (files.Count > 0)
            {
                var vm = (MainWindowViewModel)DataContext;
                vm?.LoadConfig(files[0].Path.LocalPath);
            }
        }

        private void Widget_Tapped(object sender, TappedEventArgs e)
        {
            // Handled mostly by PointerPressed now to support multi-select.
            // Keeping this empty to avoid conflicts if needed.
        }

        private void Canvas_PointerPressed(object sender, PointerPressedEventArgs e)
        {
            var pointer = e.GetCurrentPoint(sender as Control);
            if (!pointer.Properties.IsLeftButtonPressed) return;

            var vm = (MainWindowViewModel)DataContext;
            if (vm == null) return;

            // Take focus to handle arrow keys correctly after using Slider
            (sender as Control)?.Focus();

            if (e.Source is Control control && control.DataContext is WidgetViewModel widget)
            {
                // Clicked on a widget
                bool isMultiSelect = e.KeyModifiers.HasFlag(KeyModifiers.Control) || e.KeyModifiers.HasFlag(KeyModifiers.Shift);
                
                if (!widget.IsSelected)
                {
                    if (!isMultiSelect)
                    {
                        foreach (var w in vm.Widgets) w.IsSelected = false;
                    }
                    widget.IsSelected = true;
                }
                else if (isMultiSelect)
                {
                    // Deselect if already selected and holding modifier
                    widget.IsSelected = false;
                }

                vm.SelectedWidget = widget.IsSelected ? widget : vm.Widgets.FirstOrDefault(w => w.IsSelected);

                _isDraggingWidgets = true;
                _dragStartPoint = pointer.Position;
                _dragStartPositions = vm.Widgets
                    .Where(w => w.IsSelected)
                    .Select(w => (w, w.X, w.Y))
                    .ToList();
                e.Handled = true;
            }
            else
            {
                // Clicked on the canvas background -> start drawing selection
                foreach (var w in vm.Widgets) w.IsSelected = false;
                vm.SelectedWidget = null;
                
                _isDrawingSelection = true;
                _dragStartPoint = pointer.Position;
                
                SelectionBox.IsVisible = true;
                Canvas.SetLeft(SelectionBox, _dragStartPoint.X);
                Canvas.SetTop(SelectionBox, _dragStartPoint.Y);
                SelectionBox.Width = 0;
                SelectionBox.Height = 0;
            }
        }

        private void Canvas_PointerMoved(object sender, PointerEventArgs e)
        {
            var currentPoint = e.GetCurrentPoint(sender as Control);
            var vm = (MainWindowViewModel)DataContext;
            if (vm == null) return;

            if (_isDraggingWidgets)
            {
                var delta = currentPoint.Position - _dragStartPoint;

                // Move all selected widgets
                foreach (var w in vm.Widgets.Where(x => x.IsSelected))
                {
                    w.Y += (int)delta.X;
                    w.X += (int)delta.Y;
                }

                // Update drag start point by the integer amount we actually moved
                _dragStartPoint = new Point(
                    _dragStartPoint.X + (int)delta.X,
                    _dragStartPoint.Y + (int)delta.Y);
            }
            else if (_isDrawingSelection)
            {
                var pos = currentPoint.Position;
                var x = Math.Min(pos.X, _dragStartPoint.X);
                var y = Math.Min(pos.Y, _dragStartPoint.Y);
                var w = Math.Abs(pos.X - _dragStartPoint.X);
                var h = Math.Abs(pos.Y - _dragStartPoint.Y);

                Canvas.SetLeft(SelectionBox, x);
                Canvas.SetTop(SelectionBox, y);
                SelectionBox.Width = w;
                SelectionBox.Height = h;

                // Select widgets within the rect
                var selectionRect = new Rect(x, y, w, h);
                foreach (var widget in vm.Widgets)
                {
                    if (widget.IsBackground) continue;

                    // Map widget coordinates back to canvas layout
                    double wx = widget.Y;
                    double wy = widget.X;
                    var widgetRect = new Rect(wx, wy, widget.Width, widget.Height);
                    
                    widget.IsSelected = selectionRect.Intersects(widgetRect);
                }
            }
        }

        private void Canvas_PointerReleased(object sender, PointerReleasedEventArgs e)
        {
            if (_isDraggingWidgets && _dragStartPositions != null)
            {
                var vm = DataContext as MainWindowViewModel;
                if (vm != null)
                {
                    var moves = _dragStartPositions
                        .Where(s => s.X != s.Widget.X || s.Y != s.Widget.Y)
                        .Select(s => (s.Widget, s.X, s.Y, s.Widget.X, s.Widget.Y))
                        .ToList();
                    if (moves.Count > 0)
                        vm.UndoManager.Push(new MoveWidgetsAction(moves));
                }
                _dragStartPositions = null;
            }
            _isDraggingWidgets = false;

            if (_isDrawingSelection)
            {
                _isDrawingSelection = false;
                SelectionBox.IsVisible = false;
                
                var vm = (MainWindowViewModel)DataContext;
                if (vm != null)
                {
                    vm.SelectedWidget = vm.Widgets.LastOrDefault(w => w.IsSelected);
                }
            }
        }

        private void Canvas_PointerWheelChanged(object sender, PointerWheelEventArgs e)
        {
            if (e.KeyModifiers.HasFlag(KeyModifiers.Control))
            {
                var vm = (MainWindowViewModel)DataContext;
                if (vm != null)
                {
                    // Zoom in or out by 10% per scroll tick
                    double zoomDelta = e.Delta.Y > 0 ? 0.1 : -0.1;
                    double newZoom = Math.Max(0.5, Math.Min(3.0, vm.Zoom + zoomDelta));
                    vm.Zoom = newZoom;
                    e.Handled = true;
                }
            }
        }

        private void Window_KeyDown(object sender, KeyEventArgs e)
        {
            // Do not move widgets if the user is typing in a TextBox (like the Property Grid)
            if (e.Source is TextBox) return;

            var vm = (MainWindowViewModel)DataContext;
            if (vm == null) return;

            var selectedWidgets = vm.Widgets.Where(w => w.IsSelected).ToList();
            if (!selectedWidgets.Any()) return;

            int dx = 0;
            int dy = 0;

            switch (e.Key)
            {
                case Key.Up: dy = -1; break;
                case Key.Down: dy = 1; break;
                case Key.Left: dx = -1; break;
                case Key.Right: dx = 1; break;
                default: return; // Not an arrow key
            }

            var before = selectedWidgets.Select(w => (w, w.X, w.Y)).ToList();

            foreach (var w in selectedWidgets)
            {
                w.X += dy;
                w.Y += dx;
            }

            var moves = before
                .Select(s => (s.w, s.X, s.Y, s.w.X, s.w.Y))
                .ToList();
            vm.UndoManager.Push(new MoveWidgetsAction(moves));

            e.Handled = true;
        }
    }
}
