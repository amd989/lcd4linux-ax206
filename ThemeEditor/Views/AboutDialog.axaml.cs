using System.Diagnostics;
using Avalonia.Controls;
using Avalonia.Interactivity;

namespace ThemeEditor.Views
{
    public partial class AboutDialog : Window
    {
        public AboutDialog()
        {
            InitializeComponent();
        }

        private void GitHubLink_Click(object? sender, RoutedEventArgs e)
        {
            Process.Start(new ProcessStartInfo
            {
                FileName = "https://github.com/amd989/lcd4linux-ax206",
                UseShellExecute = true
            });
        }

        private void Close_Click(object? sender, RoutedEventArgs e)
        {
            Close();
        }
    }
}
