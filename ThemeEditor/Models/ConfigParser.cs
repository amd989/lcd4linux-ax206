using System;
using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;

namespace ThemeEditor.Models
{
    public class ConfigBlock
    {
        public string Type { get; set; } = string.Empty;
        public string Name { get; set; } = string.Empty;
        public bool IsGlobalProperty { get; set; } = false;
        public List<ConfigProperty> Properties { get; set; } = new();
        public List<ConfigBlock> NestedBlocks { get; set; } = new();
    }

    public class ConfigProperty
    {
        public string Key { get; set; } = string.Empty;
        public string Value { get; set; } = string.Empty;
        public bool IsComment { get; set; } = false;
        public string? RawLine { get; set; }
    }

    public class LcdConfig
    {
        public List<ConfigBlock> Blocks { get; set; } = new();
    }

    public static class ConfigParser
    {
        public static LcdConfig Parse(string configText)
        {
            var config = new LcdConfig();
            
            // Normalize line endings
            configText = configText.Replace("\r\n", "\n");
            
            // Simple tokenizer
            var lines = configText.Split(new[] { '\n' }, StringSplitOptions.None);
            
            var stack = new Stack<ConfigBlock>();
            
            foreach (var line in lines)
            {
                var trimmed = line.Trim();
                
                // Preserve empty lines and comments
                if (string.IsNullOrWhiteSpace(trimmed) || trimmed.StartsWith("#"))
                {
                    var comment = new ConfigProperty { IsComment = true, RawLine = line };
                    if (stack.Count > 0)
                        stack.Peek().Properties.Add(comment);
                    else
                        config.Blocks.Add(new ConfigBlock { IsGlobalProperty = true, Name = line, Type = "" });
                    continue;
                }

                // Block Start
                if (trimmed.EndsWith("{"))
                {
                    var header = trimmed.Substring(0, trimmed.Length - 1).Trim();
                    var parts = SplitWithQuotes(header);
                    
                    var newBlock = new ConfigBlock
                    {
                        Type = parts.Count > 0 ? parts[0] : "",
                        Name = parts.Count > 1 ? parts[1].Trim('\'', '"') : ""
                    };

                    if (stack.Count > 0)
                    {
                        stack.Peek().NestedBlocks.Add(newBlock);
                    }
                    else
                    {
                        config.Blocks.Add(newBlock);
                    }
                    
                    stack.Push(newBlock);
                }
                // Block End
                else if (trimmed == "}")
                {
                    if (stack.Count > 0)
                    {
                        stack.Pop();
                    }
                }
                // Property
                else
                {
                    var parts = SplitWithQuotes(trimmed, 2);
                    if (parts.Count >= 1)
                    {
                        var key = parts[0];
                        var value = parts.Count > 1 ? string.Join(" ", parts.Skip(1)) : "";

                        if (stack.Count > 0)
                        {
                            stack.Peek().Properties.Add(new ConfigProperty { Key = key, Value = value });
                        }
                        else
                        {
                            // Global properties like: Display 'DPF'
                            // Let's treat them as standalone blocks for simplicity or ignore
                            var dummyBlock = new ConfigBlock { Type = key, Name = value };
                            config.Blocks.Add(dummyBlock);
                        }
                    }
                }
            }

            return config;
        }

        private static List<string> SplitWithQuotes(string input, int maxSplits = -1)
        {
            var result = new List<string>();
            var inQuotes = false;
            var currentToken = "";

            for (int i = 0; i < input.Length; i++)
            {
                char c = input[i];

                if (c == '\'' || c == '"')
                {
                    inQuotes = !inQuotes;
                    currentToken += c;
                }
                else if (char.IsWhiteSpace(c) && !inQuotes)
                {
                    if (!string.IsNullOrWhiteSpace(currentToken))
                    {
                        result.Add(currentToken);
                        currentToken = "";
                        
                        if (maxSplits > 0 && result.Count == maxSplits - 1)
                        {
                            result.Add(input.Substring(i + 1).Trim());
                            return result;
                        }
                    }
                }
                else
                {
                    currentToken += c;
                }
            }

            if (!string.IsNullOrWhiteSpace(currentToken))
            {
                result.Add(currentToken);
            }

            return result;
        }
    }
}
