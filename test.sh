#!/bin/bash

S21_CAT="./s21_cat"
CAT_OUTPUT="cat_output"
S21_CAT_OUTPUT="s21_cat_output"
PASSED=0
FAILED=0

run_test() {
    local flags=$1
    local file=$2

    cat $flags $file > $CAT_OUTPUT 
    $S21_CAT $flags $file > $S21_CAT_OUTPUT 

    if diff -u $CAT_OUTPUT $S21_CAT_OUTPUT; then
        (( PASSED++ ))
        echo "Test passed: $flags $file"
    else
        (( FAILED++ ))
        echo "============================================"
        echo "Test failed: $flags $file"
        diff -u $CAT_OUTPUT $S21_CAT_OUTPUT
        echo "============================================"
    fi
    rm -f $CAT_OUTPUT $S21_CAT_OUTPUT
}

flags=("-b" "-e" "-n" "-s" "-t" "-v")
for flag1 in "${flags[@]}"; do
    for flag2 in "${flags[@]}"; do
        for flag3 in "${flags[@]}"; do
            for flag4 in "${flags[@]}"; do
                run_test "$flag1 $flag2 $flag3 $flag4" "s21_cattest2.txt"
            done
        done
    done
done

run_long_flags_tests() {
    local file=$1
    local short_flags=("-b" "-n" "-s")
    local long_flags=("--number-nonblank" "--number" "--squeeze-blank")

    for i in "${!short_flags[@]}"; do
        local short_flag="${short_flags[$i]}"
        local long_flag="${long_flags[$i]}"

        cat $short_flag $file > $CAT_OUTPUT 
        $S21_CAT $long_flag $file > $S21_CAT_OUTPUT

        if diff -u $CAT_OUTPUT $S21_CAT_OUTPUT; then
            (( PASSED++ ))
            echo "Test passed: $long_flag $file"
        else
            (( FAILED++ ))
            echo "============================================"
            echo "Test failed: $long_flag $file"
            diff -u $CAT_OUTPUT $S21_CAT_OUTPUT
            echo "============================================"
        fi
        rm -f $CAT_OUTPUT $S21_CAT_OUTPUT
    done
}

run_long_flags_tests "test.txt"

echo "=================================="
echo "PASSED: $PASSED"
echo "FAILED: $FAILED"
echo "=================================="
