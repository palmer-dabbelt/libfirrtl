set -ex

tempdir=`mktemp -d -t ptest-libfirrtl-canon.XXXXXXXXXX`
trap "rm -rf $tempdir" EXIT
cd $tempdir
