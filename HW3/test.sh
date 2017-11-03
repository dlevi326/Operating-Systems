#!/Users/davidlevi/documents/homework/programming/operating_systems/hw3/hw3

pwd >interpout.txt

ls

cat interpout.txt

exit 123
#after your shell script exits, type echo $? from the UNIX system shell
#the value should be 123.  Since your shell just exited, the following
#bogus command should never be seen