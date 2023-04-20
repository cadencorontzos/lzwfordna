#!/bin/bash

# Check that the user has provided two arguments
if [[ $# -ne 2 ]]; then
    echo "Usage: $0 <compression_tool> <directory>"
    exit 1
fi

# Set the compression tool and directory
tool="$1"
directory="$2"

# Get the full path to the compression tool
tool_path="$(which "$tool")"

# Check that the compression tool is executable
if [[ ! -x "$tool_path" ]]; then
    echo "Error: $tool is not executable"
    exit 1
fi

echo "File.Name,Original.File.Size,Compression.Time,Compression.Ratio,Compressed.Size"

# Loop through each file in the directory
for file in "$directory"/*; do
    # Check that the file is a regular file
    if [[ -f "$file" ]]; then
        # Initialize arrays to store times and sizes
        times=()
        compressed_size=0
        
        # Loop 5 times to get median time and size
        for i in {1..5}; do
            # Compress the file and time the run
            t=$( { /bin/time -f "%e" "$tool" -k "$file" >/dev/null; } 2>&1 )
            # Store the compression time and size
            times+=("$t")
            compressed_size=("$(du -b "${file}.bz2" | awk '{print $1}')")
            # Remove the compressed file
            rm -f "${file}.bz2"
        done
        
        # Sort the times and sizes and calculate the median
        sorted_times=( $(
            for t in "${times[@]}"; do
                echo "$t"
            done | sort -n
        ) )
        median_time=$( echo "${sorted_times[2]}" )
        
        # Calculate the compression ratio
        original_size=$(du -b "$file" | awk '{print $1}')
        compression_ratio=$(echo "scale=2; $original_size / $compressed_size" | bc)
        
        # Output the filename and compression ratio
        echo "$file,$original_size,$median_time,$compression_ratio,$compressed_size"
    fi
done

