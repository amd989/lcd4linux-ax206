using System;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;
using System.Windows.Input;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using ThemeEditor.Models;
using Avalonia.Controls;
using Avalonia.Controls.ApplicationLifetimes;

namespace ThemeEditor.ViewModels
{
    public partial class MainWindowViewModel : ViewModelBase
    {
        private LcdConfig _currentConfig;
        private string _currentFilePath;

        public UndoRedoManager UndoManager { get; } = new();

        public ObservableCollection<WidgetViewModel> Widgets { get; } = new();

        [ObservableProperty]
        private WidgetViewModel _selectedWidget;

        [ObservableProperty]
        private int _displayWidth = 480;

        [ObservableProperty]
        private int _displayHeight = 320;

        [ObservableProperty]
        private double _zoom = 1.0;

        private int _orientation = 0;
        public int Orientation
        {
            get => _orientation;
            set
            {
                SetProperty(ref _orientation, value);
                UpdateDimensions();
            }
        }

        private void UpdateDimensions()
        {
            if (Orientation == 1 || Orientation == 3)
            {
                // Portrait
                DisplayWidth = 320;
                DisplayHeight = 480;
            }
            else
            {
                // Landscape
                DisplayWidth = 480;
                DisplayHeight = 320;
            }
        }

        public ICommand LoadCommand { get; }
        public ICommand SaveCommand { get; }

        public MainWindowViewModel()
        {
            LoadCommand = new RelayCommand<string>(LoadConfig);
            SaveCommand = new RelayCommand(SaveConfig);
        }

        public void LoadConfig(string filePath)
        {
            try
            {
                _currentFilePath = filePath;
                var text = File.ReadAllText(filePath);
                _currentConfig = ConfigParser.Parse(text);

                // Find workspace root. The config is usually in <root>/themes/<name>/xxx.conf
                string workspaceRoot = Path.GetDirectoryName(filePath) ?? Environment.CurrentDirectory;
                while (!string.IsNullOrEmpty(workspaceRoot) && !Directory.Exists(Path.Combine(workspaceRoot, "themes")))
                {
                    workspaceRoot = Path.GetDirectoryName(workspaceRoot) ?? "";
                }
                if (string.IsNullOrEmpty(workspaceRoot)) workspaceRoot = Environment.CurrentDirectory;

                foreach (var w in Widgets)
                    w.Dispose();
                Widgets.Clear();

                // 1. Find all widgets
                var widgetBlocks = _currentConfig.Blocks.Where(b => b.Type == "Widget");
                UndoManager.Clear();
                var parsedWidgets = widgetBlocks.Select(b => new WidgetViewModel(b, workspaceRoot, UndoManager)).ToList();

                // 2. Find orientation
                var displayBlock = _currentConfig.Blocks.FirstOrDefault(b => b.Type == "Display" && b.Properties.Any(p => p.Key == "Orientation"));
                if (displayBlock != null)
                {
                    var orientProp = displayBlock.Properties.FirstOrDefault(p => p.Key == "Orientation");
                    if (orientProp != null && int.TryParse(orientProp.Value, out int o))
                    {
                        Orientation = o;
                    }
                }

                // 3. Find layout to map X,Y coordinates
                var layoutBlock = _currentConfig.Blocks.FirstOrDefault(b => b.Type == "Layout");
                if (layoutBlock != null)
                {
                    foreach (var layer in layoutBlock.NestedBlocks.Where(b => b.Type == "Layer"))
                    {
                        int zIndex = 0;
                        if (!string.IsNullOrEmpty(layer.Name) && int.TryParse(layer.Name, out int parsedLayer))
                        {
                            zIndex = parsedLayer;
                        }

                        foreach (var prop in layer.Properties)
                        {
                            // Property key is like "X16.Y6", Value is like "'CPU_TTF'"
                            var name = prop.Value.Trim('\'', '"');
                            var match = Regex.Match(prop.Key, @"X(\d+)\.Y(\d+)");
                            if (match.Success)
                            {
                                int x = int.Parse(match.Groups[1].Value);
                                int y = int.Parse(match.Groups[2].Value);

                                var widget = parsedWidgets.FirstOrDefault(w => w.Name == name);
                                if (widget != null)
                                {
                                    widget.X = x;
                                    widget.Y = y;
                                    widget.ZIndex = zIndex;
                                }
                            }
                        }
                    }
                }

                foreach (var w in parsedWidgets)
                {
                    Widgets.Add(w);
                }
            }
            catch (Exception ex)
            {
                ShowError($"Error loading config: {ex.Message}");
            }
        }

        public void SaveConfig()
        {
            if (_currentConfig == null || string.IsNullOrEmpty(_currentFilePath))
                return;

            try
            {
                // Update layout X/Y back to config AST
                var layoutBlock = _currentConfig.Blocks.FirstOrDefault(b => b.Type == "Layout");
                if (layoutBlock != null)
                {
                    foreach (var w in Widgets)
                    {
                        bool found = false;
                        foreach (var layer in layoutBlock.NestedBlocks.Where(b => b.Type == "Layer"))
                        {
                            var prop = layer.Properties.FirstOrDefault(p => p.Value.Trim('\'', '"') == w.Name);
                            if (prop != null)
                            {
                                prop.Key = $"X{w.X}.Y{w.Y}";
                                found = true;
                                break;
                            }
                        }
                    }
                }

                var text = ConfigSerializer.Serialize(_currentConfig);
                File.WriteAllText(_currentFilePath, text);
            }
            catch (Exception ex)
            {
                ShowError($"Error saving config: {ex.Message}");
            }
        }

        private async void ShowError(string message)
        {
            if (Avalonia.Application.Current?.ApplicationLifetime is IClassicDesktopStyleApplicationLifetime desktop
                && desktop.MainWindow is Window window)
            {
                var dialog = new Window
                {
                    Title = "Error",
                    Width = 400,
                    Height = 150,
                    WindowStartupLocation = WindowStartupLocation.CenterOwner,
                    Content = new StackPanel
                    {
                        Margin = new Avalonia.Thickness(20),
                        Spacing = 15,
                        Children =
                        {
                            new TextBlock { Text = message, TextWrapping = Avalonia.Media.TextWrapping.Wrap },
                            new Button { Content = "OK", HorizontalAlignment = Avalonia.Layout.HorizontalAlignment.Center }
                        }
                    }
                };
                ((Button)((StackPanel)dialog.Content).Children[1]).Click += (_, _) => dialog.Close();
                await dialog.ShowDialog(window);
            }
        }
    }
}
