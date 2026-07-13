#!/bin/sh
set -e

# Recreate dot/ directory
rm -rf dot
mkdir -p dot

cp init.q README.md LICENSE dot/

# Copy all files matching */init* and subdir README.md files into dot/, preserving directory structure
for file in */init* */README.md; do
    if [ -f "$file" ]; then
        dir=$(dirname "$file")
        mkdir -p "dot/$dir"
        cp "$file" "dot/$file"
    fi
done

# Zip the dot/ directory into dot.zip
rm -f dot.zip
zip -r dot.zip dot
