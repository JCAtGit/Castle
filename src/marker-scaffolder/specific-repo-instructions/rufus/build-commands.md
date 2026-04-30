# Build procedure rufus

## Details
Gebruikt GNU Automake

## Notes
Uses mostly "standard" procedures
Except the ./configure, where the host is explicit. Rufus only builds for windows.

## (standard) Procedure
./bootstrap.sh
./configure --host=x86_64-w64-mingw32

./configure --host=x86_64-w64-mingw32 --with-win32-api=win32 

make