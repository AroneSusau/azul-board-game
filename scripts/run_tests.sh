#! /bin/bash

# This script was written to automate the process of running
# all the tests found in the tests directory.

# Test meta data.
TEST_COUNT=0
SUCCESS_COUNT=0
FAILED=0
VERBOSE=$1

# Check if Azul exists.
if [ -f "./Azul" ]
then

    SUITES=$(ls tests)

    # Loop through test suite directories
    for test_suite in $SUITES
    do

        UNITS=$(ls "tests/$test_suite")

        # Loop through unit test directories
        for unit_test in $UNITS
        do  

            TEST_COUNT=$(($TEST_COUNT+1))

            # Gather test file details.
            DIR="tests/$test_suite/$unit_test"
            OUTPUT=$DIR/program.out
            EXPECTED=$DIR/expected.out
            ARGS_PATH=$DIR/program.args
            ARGS=$(if [ -f $ARGS_PATH ]; then cat $ARGS_PATH; fi)

            # Run Azul with args and pass in the input
            ./Azul $ARGS < $DIR/*.in &> $OUTPUT

            # Check for a difference in output vs expected output.
            RESULT=$(diff $OUTPUT $EXPECTED -y --suppress-common-lines)

            if [ ! -z "$RESULT" ]
            then
                FAILED=1
                
                # Print failure details, most of this is just for the verbose flag formatting.
                if [ $VERBOSE -a $VERBOSE == 1 ]; then echo ""; fi;
                
                printf "\nTEST FAILED - $test_suite: $unit_test"

                if [ $VERBOSE -a $VERBOSE == 1 ]; then printf "\n$RESULT"; fi;

            else
                SUCCESS_COUNT=$(($SUCCESS_COUNT+1))
            fi

        done
    done

    if [ $FAILED -eq 1 ]; then echo ""; fi;

    printf "\nResult: $SUCCESS_COUNT tests passed out of $TEST_COUNT. \n"

else
    printf "\nERROR: Azul executable does not exist! Please compile the programing using the 'make' command. \n"
fi