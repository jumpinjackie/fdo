cd ${FDOTHIRDPARTY}
./Thirdparty.sh
cd ..
aclocal
libtoolize --force
automake --add-missing --copy
autoconf
./configure --enable-debug=yes
make

