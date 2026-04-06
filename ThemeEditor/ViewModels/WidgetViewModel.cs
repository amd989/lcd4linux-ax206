using System;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using Avalonia.Media.Imaging;
using Avalonia.Media;
using CommunityToolkit.Mvvm.ComponentModel;
using ThemeEditor.Models;

namespace ThemeEditor.ViewModels
{
    public partial class WidgetViewModel : ObservableObject, IDisposable
    {
        private ConfigBlock _block;

        public WidgetViewModel(ConfigBlock block, string workspaceRoot, UndoRedoManager? undoManager = null)
        {
            _block = block;
            Name = block.Name;

            var classProp = block.Properties.FirstOrDefault(p => p.Key == "class");
            Class = classProp?.Value?.Trim('\'', '"') ?? "Unknown";

            Properties = new ObservableCollection<PropertyViewModel>(
                block.Properties.Where(p => !p.IsComment).Select(p => new PropertyViewModel(p, undoManager))
            );

            foreach (var prop in Properties)
            {
                prop.PropertyChanged += (s, e) =>
                {
                    if (e.PropertyName == nameof(PropertyViewModel.Value))
                    {
                        OnPropertyChanged(nameof(Width));
                        OnPropertyChanged(nameof(Height));
                        OnPropertyChanged(nameof(DisplayText));
                        OnPropertyChanged(nameof(ForegroundBrush));
                        OnPropertyChanged(nameof(BackgroundBrush));
                        OnPropertyChanged(nameof(TextAlignment));
                    }
                };
            }

            if (Class.Equals("Image", System.StringComparison.OrdinalIgnoreCase))
            {
                var fileProp = GetProperty("file");
                if (!string.IsNullOrEmpty(fileProp))
                {
                    string fullPath = Path.Combine(workspaceRoot, fileProp);
                    if (File.Exists(fullPath))
                    {
                        try
                        {
                            ImageBitmap = new Bitmap(fullPath);
                        }
                        catch { }
                    }
                }
            }
        }

        public string Name { get; }
        public string Class { get; }

        public Bitmap? ImageBitmap { get; }
        public bool IsImage => ImageBitmap != null;
        public bool IsText => !IsImage;

        public ObservableCollection<PropertyViewModel> Properties { get; }

        public string GetProperty(string key, string defaultValue = "")
        {
            var p = Properties.FirstOrDefault(p => p.Key == key);
            return p != null ? p.Value.Trim('\'', '"') : defaultValue;
        }

        // Layout properties (X, Y) bound from Layout section
        [ObservableProperty]
        private int _x;

        [ObservableProperty]
        private int _y;

        [ObservableProperty]
        [NotifyPropertyChangedFor(nameof(AvaloniaZIndex))]
        private int _zIndex;

        // In LCD4Linux, lower layer numbers (like 0 and 1) are drawn on top of higher layer numbers (like 2).
        // Avalonia draws higher ZIndex values on top. So we invert it.
        public int AvaloniaZIndex => -_zIndex;

        public double Width
        {
            get
            {
                if (Class.Equals("GraphicBar", System.StringComparison.OrdinalIgnoreCase) || Class.Equals("sparkline", System.StringComparison.OrdinalIgnoreCase))
                {
                    var dir = GetProperty("direction", "E");
                    if (dir == "E" || dir == "W" || string.IsNullOrEmpty(dir))
                    {
                        if (int.TryParse(GetProperty("length"), out int valL)) return valL;
                    }
                    else
                    {
                        if (int.TryParse(GetProperty("width"), out int valW)) return valW;
                    }
                }
                
                var w = GetProperty("width");
                if (int.TryParse(w, out int val)) return val;
                return IsImage && ImageBitmap != null ? ImageBitmap.Size.Width : 100;
            }
        }

        public double Height
        {
            get
            {
                if (Class.Equals("GraphicBar", System.StringComparison.OrdinalIgnoreCase) || Class.Equals("sparkline", System.StringComparison.OrdinalIgnoreCase))
                {
                    var dir = GetProperty("direction", "E");
                    if (dir == "E" || dir == "W" || string.IsNullOrEmpty(dir))
                    {
                        if (int.TryParse(GetProperty("width"), out int valW)) return valW;
                    }
                    else
                    {
                        if (int.TryParse(GetProperty("length"), out int valL)) return valL;
                    }
                }

                var h = GetProperty("height");
                if (int.TryParse(h, out int val)) return val;
                return IsImage && ImageBitmap != null ? ImageBitmap.Size.Height : 50;
            }
        }

        public string DisplayText
        {
            get
            {
                if (Class.Equals("Truetype", System.StringComparison.OrdinalIgnoreCase))
                {
                    return GetProperty("expression").Replace("'", "");
                }
                return Name;
            }
        }

        public IBrush ForegroundBrush 
        {
            get 
            {
                var c = GetProperty("fcolor");
                if (string.IsNullOrEmpty(c)) c = GetProperty("color");
                return ParseBrush(c, "#FFFFFFFF"); // White fallback
            }
        }

        public IBrush BackgroundBrush
        {
            get 
            {
                var c = GetProperty("background");
                // 0x40 is ~25% opacity
                return ParseBrush(c, "#40000000"); // Translucent black/dark gray by default
            }
        }

        private IBrush ParseBrush(string colorStr, string fallbackStr)
        {
            if (!string.IsNullOrEmpty(colorStr))
            {
                colorStr = colorStr.Trim('\'', '"', '#');
                if (colorStr.Length >= 6)
                {
                    var r = colorStr.Substring(0, 2);
                    var g = colorStr.Substring(2, 2);
                    var b = colorStr.Substring(4, 2);
                    int alpha = 255;
                    if (colorStr.Length == 8)
                    {
                        // LCD4Linux alpha is inverted: 00 = opaque, ff = transparent
                        if (int.TryParse(colorStr.Substring(6, 2), System.Globalization.NumberStyles.HexNumber, null, out int aVal))
                        {
                            alpha = 255 - aVal;
                        }
                    }
                    if (byte.TryParse(r, System.Globalization.NumberStyles.HexNumber, null, out byte rb) &&
                        byte.TryParse(g, System.Globalization.NumberStyles.HexNumber, null, out byte gb) &&
                        byte.TryParse(b, System.Globalization.NumberStyles.HexNumber, null, out byte bb))
                    {
                        return new SolidColorBrush(Color.FromArgb((byte)alpha, rb, gb, bb));
                    }
                }
            }
            return new SolidColorBrush(Color.Parse(fallbackStr));
        }

        public bool IsBackground => Name.Equals("Background", System.StringComparison.OrdinalIgnoreCase);

        public TextAlignment TextAlignment
        {
            get
            {
                var align = GetProperty("align", "L").ToUpper();
                return align switch
                {
                    "C" => TextAlignment.Center,
                    "R" => TextAlignment.Right,
                    _ => TextAlignment.Left
                };
            }
        }

        [ObservableProperty]
        private bool _isSelected;

        public void Dispose()
        {
            ImageBitmap?.Dispose();
            GC.SuppressFinalize(this);
        }
    }

    public partial class PropertyViewModel : ObservableObject
    {
        private ConfigProperty _property;
        private readonly UndoRedoManager? _undoManager;

        public PropertyViewModel(ConfigProperty property, UndoRedoManager? undoManager = null)
        {
            _property = property;
            _undoManager = undoManager;
        }

        public string Key => _property.Key;

        public string Value
        {
            get => _property.Value;
            set
            {
                if (_property.Value != value)
                {
                    var oldValue = _property.Value;
                    OnPropertyChanging();
                    _property.Value = value;
                    OnPropertyChanged();
                    _undoManager?.Push(new PropertyChangeAction(this, oldValue, value));
                    if (IsColor) OnPropertyChanged(nameof(ColorValue));
                    if (IsNumeric) OnPropertyChanged(nameof(NumericValue));
                    if (IsBoolean) OnPropertyChanged(nameof(BooleanValue));
                }
            }
        }

        public bool IsColor => Key.Equals("fcolor", StringComparison.OrdinalIgnoreCase) ||
                               Key.Equals("color", StringComparison.OrdinalIgnoreCase) ||
                               Key.Equals("colorhigh", StringComparison.OrdinalIgnoreCase) ||
                               Key.Equals("colorlow", StringComparison.OrdinalIgnoreCase) ||
                               Key.Equals("background", StringComparison.OrdinalIgnoreCase);

        public bool IsAlign => Key.Equals("align", StringComparison.OrdinalIgnoreCase);

        public bool IsNumeric => Key.Equals("width", StringComparison.OrdinalIgnoreCase) ||
                                 Key.Equals("height", StringComparison.OrdinalIgnoreCase) ||
                                 Key.Equals("length", StringComparison.OrdinalIgnoreCase) ||
                                 Key.Equals("max", StringComparison.OrdinalIgnoreCase) ||
                                 Key.Equals("min", StringComparison.OrdinalIgnoreCase) ||
                                 Key.Equals("samples", StringComparison.OrdinalIgnoreCase) ||
                                 Key.Equals("size", StringComparison.OrdinalIgnoreCase);

        public bool IsBoolean => Key.Equals("reload", StringComparison.OrdinalIgnoreCase) ||
                                 Key.Equals("visible", StringComparison.OrdinalIgnoreCase);

        public bool IsClass => Key.Equals("class", StringComparison.OrdinalIgnoreCase);

        public bool IsStandard => !IsColor && !IsAlign && !IsNumeric && !IsBoolean && !IsClass;

        public string[] ClassOptions => new[] { "Truetype", "GraphicBar", "Image", "Gauge", "Sparkline", "bar", "text", "icon", "timer", "keypad", "gpo" };

        public string SelectedClass
        {
            get => Value.Trim('\'', '"');
            set => Value = $"'{value}'";
        }

        public bool BooleanValue
        {
            get => Value.Trim('\'', '"') == "1";
            set => Value = value ? "1" : "0";
        }

        public decimal? NumericValue
        {
            get
            {
                var val = Value.Trim('\'', '"');
                if (decimal.TryParse(val, out decimal result))
                    return result;
                return null;
            }
            set
            {
                if (value.HasValue)
                    Value = ((int)value.Value).ToString();
            }
        }

        public Color ColorValue
        {
            get
            {
                string val = Value.Trim('\'', '"', '#');
                if (val.Length >= 6)
                {
                    try
                    {
                        var r = byte.Parse(val.Substring(0, 2), System.Globalization.NumberStyles.HexNumber);
                        var g = byte.Parse(val.Substring(2, 2), System.Globalization.NumberStyles.HexNumber);
                        var b = byte.Parse(val.Substring(4, 2), System.Globalization.NumberStyles.HexNumber);
                        int alpha = 255;
                        if (val.Length == 8)
                        {
                            // LCD4Linux alpha is inverted: 00 = opaque, ff = transparent
                            if (int.TryParse(val.Substring(6, 2), System.Globalization.NumberStyles.HexNumber, null, out int aVal))
                            {
                                alpha = 255 - aVal;
                            }
                        }
                        return Color.FromArgb((byte)alpha, r, g, b);
                    }
                    catch { }
                }
                return Colors.White;
            }
            set
            {
                // Convert back to LCD4Linux hex
                // Note: we invert alpha back
                int lcdAlpha = 255 - value.A;
                string hex = $"'{value.R:x2}{value.G:x2}{value.B:x2}{lcdAlpha:x2}'";
                Value = hex;
            }
        }

        public string[] AlignOptions => new[] { "L", "C", "R" };
        
        public string SelectedAlign
        {
            get => Value.Trim('\'', '"').ToUpper();
            set => Value = $"'{value}'";
        }
    }
}
