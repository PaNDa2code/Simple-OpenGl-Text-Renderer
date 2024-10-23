#!/bin/bash

# Directory containing the shader files
SHADER_DIR="src/shaders"

# Output file for generated definitions (optional)
OUTPUT_FILE="$SHADER_DIR/shader_definitions.txt"

# Create or clear the output file
> "$OUTPUT_FILE"

# Loop through each shader file in the directory
for shader in "$SHADER_DIR"/*.glsl; do
  # Get the shader name without the path and extension
  shader_name=$(basename "$shader" .glsl)

  # Use awk to format the shader source into a single-line string
  shader_source=$(awk '{printf "%s\\n", $0}' "$shader")

  # Generate the definition string in the format -DFRAGMENT_SHADER_SOURCE="..."
  shader_define="-D${shader_name^^}_SOURCE=\"\\\"$shader_source\\\"\""

  # Output the definition string (for immediate use or storing)
  echo "$shader_define" >> "$OUTPUT_FILE"
done

# Print a message indicating where the definitions were saved
echo "[*] Shader definitions saved to $OUTPUT_FILE"
