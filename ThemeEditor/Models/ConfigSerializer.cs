using System;
using System.Text;

namespace ThemeEditor.Models
{
    public static class ConfigSerializer
    {
        public static string Serialize(LcdConfig config)
        {
            var sb = new StringBuilder();

            foreach (var block in config.Blocks)
            {
                if (block.IsGlobalProperty && string.IsNullOrEmpty(block.Type))
                {
                    sb.AppendLine(block.Name);
                    continue;
                }
                SerializeBlock(sb, block, 0);
                sb.AppendLine();
            }

            return sb.ToString().TrimEnd();
        }

        private static void SerializeBlock(StringBuilder sb, ConfigBlock block, int indentLevel)
        {
            string indent = new string(' ', indentLevel * 4);

            if (block.Properties.Count == 0 && block.NestedBlocks.Count == 0 && !string.IsNullOrEmpty(block.Name) && string.IsNullOrEmpty(block.Type))
            {
                // Edge case handling or properties added directly to root
                sb.AppendLine($"{indent}{block.Name}");
                return;
            }

            if (!string.IsNullOrEmpty(block.Type))
            {
                // Global property fallback (no properties and no nested blocks)
                if (block.NestedBlocks.Count == 0 && block.Properties.Count == 0)
                {
                    if (!string.IsNullOrEmpty(block.Name))
                    {
                        sb.AppendLine($"{indent}{block.Type} {block.Name}");
                    }
                    else
                    {
                        sb.AppendLine($"{indent}{block.Type}");
                    }
                    return;
                }

                if (!string.IsNullOrEmpty(block.Name))
                {
                    sb.AppendLine($"{indent}{block.Type} {block.Name} {{");
                }
                else
                {
                    sb.AppendLine($"{indent}{block.Type} {{");
                }
            }
            else
            {
                // Fallback for blocks without a type
                sb.AppendLine($"{indent}{{");
            }

            foreach (var prop in block.Properties)
            {
                if (prop.IsComment)
                    sb.AppendLine(prop.RawLine);
                else
                    sb.AppendLine($"{indent}    {prop.Key} {prop.Value}");
            }

            foreach (var nested in block.NestedBlocks)
            {
                SerializeBlock(sb, nested, indentLevel + 1);
            }

            if (!string.IsNullOrEmpty(block.Type) && block.NestedBlocks.Count == 0 && block.Properties.Count == 0)
            {
                return;
            }

            sb.AppendLine($"{indent}}}");
        }
    }
}
