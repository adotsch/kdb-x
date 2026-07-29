#!/bin/sh
set -e

# Recreate dot/ directory
rm -rf dot
mkdir -p dot

# Copy all files matching */init* and subdir markdown files into dot/, preserving directory structure
for file in */init* */*.md; do
    if [ -f "$file" ]; then
        dir=$(dirname "$file")
        mkdir -p "dot/$dir"
        cp "$file" "dot/$file"
    fi
done

cp init.q README.md LICENSE dot/

# Zip the dot/ directory into dot.zip
rm -f dot.zip
zip -r dot.zip dot
