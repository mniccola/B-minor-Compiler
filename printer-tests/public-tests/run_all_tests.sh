#!/usr/bin/bash


echo "---- good files ----"
for tfile in good*.bminor
do
    ../.././bminor -print $tfile > $tfile.out1
    ../.././bminor -print $tfile.out1 > $tfile.out2

    if diff -q $tfile.out1 $tfile.out2;
    then
        echo "$tfile --> success (as expected)"
    else
        echo "$tfile --> failure (INCORRECT)"
        diff -q $tfile.out1 $tfile.out2
    fi
done

echo "---- bad files ----"
for tfile in bad*.bminor
do
    if ../.././bminor -print $tfile > /dev/null
    then
        echo "$tfile --> success (INCORRECT)"
    else
        echo "$tfile --> failure (as expected)"
    fi
done




clean_output_files.sh:
#!/usr/bin/bash

rm *.out*
