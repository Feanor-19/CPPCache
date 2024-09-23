#!/bin/bash

# If number of arguments less then 2; print usage and exit
if [ $# -lt 2 ]; then
    printf "Usage: %s <application> <testfile>\n" "$0" >&2
    exit 1
fi

bin="$1"           # The application (from command arg)
file="$2"          # The test file (from command arg)
diff="diff -iad"   # Diff command

# An array, do not have to declare it, but is supposedly faster
# declare -a file_base=("file1" "file2" "file3")

# Padd file_base with suffixes
file_in="$file.in"             # The in file
file_out_val="$file.out"       # The out file to check against
file_out_tst="$file.out.tst"   # The out file from the application

# Validate infile exists (do the same for out validate file)
if [ ! -f "$file_in" ]; then
    printf "In file %s is missing\n" "$file_in"
    continue;
fi
if [ ! -f "$file_out_val" ]; then
    printf "Validation file %s is missing\n" "$file_out_val"
    continue;
fi

#printf "Testing against %s\n" "$file_in"

# Run application, redirect in file to app, and output to out file
"./$bin" < "$file_in" > "$file_out_tst"

# Execute diff
$diff "$file_out_tst" "$file_out_val"

# Check exit code from previous command (ie diff)
# We need to add this to a variable else we can't print it
# as it will be changed by the if [
# Iff not 0 then the files differ (at least with diff)
e_code=$?
if [ $e_code != 0 ]; then
    printf "TEST FAIL : %d\n" "$e_code"
    exit 1
else
    printf "TEST OK!\n"
    exit 0
fi