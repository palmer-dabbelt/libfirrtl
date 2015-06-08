#include "tempdir.bash"

orig="$(find | wc -l)"
out="$($PTEST_BINARY || true)"
fin="$(find | wc -l)"

if [[ "$out" == 0 ]]
then
    echo "no help text printed"
    exit 1
fi

if [[ "$orig" != "$fin" ]]
then
    echo "a file was created"
    exit 1
fi

