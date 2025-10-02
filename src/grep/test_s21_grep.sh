#!/bin/bash

TEST_COUNTER=0
COUNTER_SUCCESS=0
COUNTER_FAIL=0
SUCCESS="Files result.txt and expected.txt are identical"


echo "\nTests. Part #1"
echo "================================================================="
echo "\nTesting grep with test_text.txt without flags...................."
(( TEST_COUNTER = TEST_COUNTER + 1 ))
./s21_grep the ../tests/fixtures/test_text.txt > result.txt
grep the ../tests/fixtures/test_text.txt > expected.txt

DIFF_RES="$(diff -s result.txt expected.txt)"
if [ "$DIFF_RES" == "$SUCCESS" ]
then 
    (( COUNTER_SUCCESS = COUNTER_SUCCESS + 1 ))
    echo "was SUCCESSFUL"
else
    (( COUNTER_FAIL = COUNTER_FAIL + 1 ))
    echo "was NOT SUCCESSFUL"
    echo "$(diff result.txt expected.txt)"
fi
rm result.txt expected.txt

echo "\nTesting grep without arguments..................................."
(( TEST_COUNTER = TEST_COUNTER + 1 ))
if ./s21_grep
then 
    (( COUNTER_SUCCESS = COUNTER_SUCCESS + 1 ))
    echo "was SUCCESSFUL"
else
    (( COUNTER_FAIL = COUNTER_FAIL + 1 ))
    echo "was NOT SUCCESSFUL"
    echo "$(diff result.txt expected.txt)"
fi

echo "\nTesting with empty file.........................................."
(( TEST_COUNTER = TEST_COUNTER + 1 ))
if ./s21_grep the ../tests/fixtures/test_empty_file.txt
then 
    (( COUNTER_SUCCESS = COUNTER_SUCCESS + 1 ))
    echo "was SUCCESSFUL"
else
    (( COUNTER_FAIL = COUNTER_FAIL + 1 ))
    echo "was NOT SUCCESSFUL"
fi

echo "\nTesting grep with 2 files without flags.........................."
(( TEST_COUNTER = TEST_COUNTER + 1 ))
./s21_grep the ../tests/fixtures/test_text.txt ../tests/fixtures/test_2_text.txt > result.txt
grep the ../tests/fixtures/test_text.txt ../tests/fixtures/test_2_text.txt > expected.txt

DIFF_RES="$(diff -s result.txt expected.txt)"
if [ "$DIFF_RES" == "$SUCCESS" ]
then 
    (( COUNTER_SUCCESS = COUNTER_SUCCESS + 1 ))
    echo "was SUCCESSFUL"
else
    (( COUNTER_FAIL = COUNTER_FAIL + 1 ))
    echo "was NOT SUCCESSFUL"
    echo "$(diff -y result.txt expected.txt)"
fi
rm result.txt expected.txt

echo "\nTests. Part #2"
echo "================================================================="
for flag in -e -i -v -c -l -n -o
do
    echo "\nTesting flag $flag with test_text.txt..............................."
    (( TEST_COUNTER = TEST_COUNTER + 1 ))
    ./s21_grep $flag ca. ../tests/fixtures/test_text.txt > result.txt
    grep $flag ca. ../tests/fixtures/test_text.txt > expected.txt

    DIFF_RES="$(diff -s result.txt expected.txt)"
    if [ "$DIFF_RES" == "$SUCCESS" ]
    then 
        (( COUNTER_SUCCESS = COUNTER_SUCCESS + 1 ))
        echo "was SUCCESSFUL"
    else
        (( COUNTER_FAIL = COUNTER_FAIL + 1 ))
        echo "was NOT SUCCESSFUL"
        echo "$(diff result.txt expected.txt)"
    fi
    rm result.txt expected.txt
done

echo "\nTests. Part #3"
echo "================================================================="
echo "\nTesting flag -h with 2 files....................................."
(( TEST_COUNTER = TEST_COUNTER + 1 ))
./s21_grep -h ca. ../tests/fixtures/test_text.txt ../tests/fixtures/test_2_text.txt > result.txt
grep -h ca. ../tests/fixtures/test_text.txt ../tests/fixtures/test_2_text.txt > expected.txt

DIFF_RES="$(diff -s result.txt expected.txt)"
if [ "$DIFF_RES" == "$SUCCESS" ]
then 
    (( COUNTER_SUCCESS = COUNTER_SUCCESS + 1 ))
    echo "was SUCCESSFUL"
else
    (( COUNTER_FAIL = COUNTER_FAIL + 1 ))
    echo "was NOT SUCCESSFUL"
    echo "$(diff result.txt expected.txt)"
fi
rm result.txt expected.txt

echo "\nTesting flag -f with 2 files....................................."
(( TEST_COUNTER = TEST_COUNTER + 1 ))
./s21_grep -f ../tests/fixtures/reg.txt ../tests/fixtures/test_text.txt ../tests/fixtures/test_2_text.txt > result.txt
grep -f ../tests/fixtures/reg.txt ../tests/fixtures/test_text.txt ../tests/fixtures/test_2_text.txt > expected.txt

DIFF_RES="$(diff -s result.txt expected.txt)"
if [ "$DIFF_RES" == "$SUCCESS" ]
then 
    (( COUNTER_SUCCESS = COUNTER_SUCCESS + 1 ))
    echo "was SUCCESSFUL"
else
    (( COUNTER_FAIL = COUNTER_FAIL + 1 ))
    echo "was NOT SUCCESSFUL"
    echo "$(diff result.txt expected.txt)"
fi
rm result.txt expected.txt

echo "\nTesting flag -s with empty file.................................."
(( TEST_COUNTER = TEST_COUNTER + 1 ))
./s21_grep -s the ../tests/fixtures/test_empty_file.txt > result.txt
grep -s the ../tests/fixtures/test_empty_file.txt > expected.txt

DIFF_RES="$(diff -s result.txt expected.txt)"
if [ "$DIFF_RES" == "$SUCCESS" ]
then 
    (( COUNTER_SUCCESS = COUNTER_SUCCESS + 1 ))
    echo "was SUCCESSFUL"
else
    (( COUNTER_FAIL = COUNTER_FAIL + 1 ))
    echo "was NOT SUCCESSFUL"
    echo "$(diff result.txt expected.txt)"
fi
rm result.txt expected.txt

echo "\nTests. Part #4"
echo "================================================================="
for flag_1 in i v c l n o
do
    for flag_2 in e i v c l n o
    do
        if [ $flag_1 != $flag_2 ]
        then
            echo "\nTesting combination of flags -$flag_1 & -$flag_2 with test_text.txt"
            (( TEST_COUNTER = TEST_COUNTER + 1 ))
            ./s21_grep -$flag_1$flag_2 th. ../tests/fixtures/test_text.txt > result.txt
            grep -$flag_1$flag_2 th. ../tests/fixtures/test_text.txt > expected.txt

            DIFF_RES="$(diff -s result.txt expected.txt)"
            if [ "$DIFF_RES" == "$SUCCESS" ]
            then 
                (( COUNTER_SUCCESS = COUNTER_SUCCESS + 1 ))
                echo "was SUCCESSFUL"
            else
                (( COUNTER_FAIL = COUNTER_FAIL + 1 ))
                echo "was NOT SUCCESSFUL"
                echo "$(diff -y result.txt expected.txt)"
            fi
            rm result.txt expected.txt
        fi
    done
done

echo "\nTests. Part #5"
echo "================================================================="
for flag_1 in i v c l n h 
do
    for flag_2 in i v c l n h
    do
        if [ $flag_1 != $flag_2 ]
        then
            echo "\nTesting combination of flags -$flag_1 & -$flag_2 with 2 files...."
            (( TEST_COUNTER = TEST_COUNTER + 1 ))
            ./s21_grep -$flag_1$flag_2 the ../tests/fixtures/test_text.txt ../tests/fixtures/test_2_text.txt > result.txt
            grep -$flag_1$flag_2 the ../tests/fixtures/test_text.txt ../tests/fixtures/test_2_text.txt > expected.txt

            DIFF_RES="$(diff -s result.txt expected.txt)"
            if [ "$DIFF_RES" == "$SUCCESS" ]
            then 
                (( COUNTER_SUCCESS = COUNTER_SUCCESS + 1 ))
                echo "was SUCCESSFUL"
            else
                (( COUNTER_FAIL = COUNTER_FAIL + 1 ))
                echo "was NOT SUCCESSFUL"
                echo "$(diff -y result.txt expected.txt)"
            fi
            rm result.txt expected.txt
        fi
    done
done

echo "\n================================================================="
echo "RESULT: from $TEST_COUNTER tests"
echo "Successful: $COUNTER_SUCCESS"
echo "Failed: $COUNTER_FAIL"
