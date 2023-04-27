#!/bin/bash

# A script to run metrics on comrpession tools

# Check that the user has provided two arguments
if [[ $# -ne 2 ]]; then
    echo "Usage: $0 <compression_tool> <directory>"
    exit 1
fi

# Set the compression tool and directory
tool="$1"
directory="$2"
option="$3"

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
			
			if [[ "$tool" == "bzip2" ]]; then
            	t=$( { /bin/time -f "%e" "$tool" -9 -k "$file" >/dev/null; } 2>&1 )
            	compressed_size=("$(du -b "${file}.bz2" | awk '{print $1}')")
			elif  [[ "$tool" == "gzip" ]]; then            
            	t=$( { /bin/time -f "%e" "$tool" -k -9 "$file" >/dev/null; } 2>&1 )
            	compressed_size=("$(du -b "${file}.gz" | awk '{print $1}')")
			elif  [[ "$tool" == *genozip* ]]; then            
            	t=$( { /bin/time -f "%e" "$tool" --input=generic "$file" >/dev/null; } 2>&1 )
            	compressed_size=("$(du -b "${file}.genozip" | awk '{print $1}')")
			elif  [[ "$tool" == "xz" ]]; then            
            	t=$( { /bin/time -f "%e" "$tool" -k -9 "$file" >/dev/null; } 2>&1 )
            	compressed_size=("$(du -b "${file}.xz" | awk '{print $1}')")
			else
				echo "I only know about bzip2, gzip, xz, and genozip."
				exit 1
			fi
			# Store the compression time and size
            times+=("$t")
            # Remove the compressed file
			if [[ "$tool" == "bzip2" ]]; then
            	rm -f "${file}.bz2"
			elif  [[ "$tool" == "gzip" ]]; then            
            	rm -f "${file}.gz"
			elif  [[ "$tool" == *genozip* ]]; then            
            	rm -f "${file}.genozip"
			elif  [[ "$tool" == "xz" ]]; then            
            	rm -f "${file}.xz"
			fi
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

