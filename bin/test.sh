#!/usr/bin/env bash
code=0
cd input
for day in *; do
    for part in a b; do
        name=${day}${part}
        exe=../build/$name
        out=out-$name
        answer=../answers/$name

        if [ ! -x $exe ]; then
            continue
        fi

        printf "$name... "

        $exe &>$out
        rv=$?

        if [ $rv -ne 0 ]; then
            echo exitted with $rv
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
done

exit $code
