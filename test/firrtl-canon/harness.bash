gdb --batch --ex r --ex bt --args $PTEST_BINARY --input Top.firx --output out.firx

diff -u Top.firx out.firx
