cd ${FDOTHIRDPARTY}
./Thirdparty.sh
cd ..
aclocal
libtoolize --force
automake --add-missing --copy
autoconf
# uncomment to build in debug
#./configure --enable-debug=yes
./configure 
make

