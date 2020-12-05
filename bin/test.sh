#!/bin/bash
code=0
cd data
for day in ?; do
    cd $day

    for part in a b; do
        name=${day}${part}
        exe=../../build/$name
        out=out-$name
        answer=../../answers/$name

        if [ ! -x $exe ]; then
            continue
        fi

        printf "$name... "

        if ! $exe &>$out; then
            echo exitted with $?
            cat $out
            code=1

        elif ! cmp $out $answer &>/dev/null; then
            echo differs
            echo '  answer file:'
            cat $answer
            echo '  program:'
            cat $out
            code=1

        else
            echo okay

        fi

        echo
        rm -f $out
    done

    cd ..
done

exit $code
