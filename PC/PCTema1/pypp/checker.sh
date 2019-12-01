GOOD_TESTS=0
BAD_BONUS=0


trap ctrl_c INT

function ctrl_c() {
        echo "Exiting..."
        rm -rf byterun.log
        exit
}


function clean
{
    rm -rf byterun.log
}

function checkTest
{
    description="Test $1/$2  "
    echo -ne "$description"
    for ((i = 0; i < 56 - ${#description}; i++)); do
        printf "."
    done
    printf " "

    ./run "tests/$1/$2.py" > /dev/null

    if [ $? -eq 0 ]; then
      `diff -Bw -u --ignore-all-space "tests/$1/$2.ref" byterun.log &> /dev/null`
      DIFF_RESULT=$?

      if [ $DIFF_RESULT -eq 0 ]; then
        echo -ne "OK\n"
        if [[ $1 == *x* ]]; then
          GOOD_TESTS=$((GOOD_TESTS+5))
          elif [[ $1 == *dense* ]]; then
          GOOD_TESTS=$((GOOD_TESTS+21))
          else
          GOOD_TESTS=$((GOOD_TESTS+1))
        fi
      else
       echo -ne "FAIL (files differ)\n"
       diff "tests/$1/$2.ref" byterun.log
      fi
    else
      echo -ne 'FAIL (program error)\n'
    fi
    clean
}

clean
checkTest "demo/smaller" "if_esmaller_false_prog"
checkTest "demo/smaller" "if_esmaller_true_prog"
checkTest "demo/smaller" "if_smaller_false_prog"
checkTest "demo/smaller" "if_smaller_true_prog"
checkTest "demo/simple_binary" "simple_add"
checkTest "demo/simple_binary" "simple_modulo"
checkTest "demo/simple_binary" "simple_substract"
checkTest "demo/simple_load" "simple_load"
checkTest "demo/bigger" "if_bigger_false_prog"
checkTest "demo/bigger" "if_bigger_true_prog"
checkTest "demo/bigger" "if_ebigger_false_prog"
checkTest "demo/bigger" "if_ebigger_true_prog"
checkTest "demo/equals" "if_equals_false_prog"
checkTest "demo/equals" "if_equals_true_prog"
checkTest "demo/equals" "if_nequals_false_prog"
checkTest "demo/equals" "if_nequals_true_prog"
checkTest "demo/functions" "add_prog"
checkTest "demo/functions" "sqrt_prog"
checkTest "demo/in" "if_in_false_prog"
checkTest "demo/in" "if_in_true_prog"
checkTest "demo/loop" "for_prog"
checkTest "demo/loop" "while_prog"
