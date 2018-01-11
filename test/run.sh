
mkdir -p ../build/
rm ../build/server ../build/client
cd ../lib/lib_src/
make re
cp libmy.a ../../server/
cp libmy.a ../../client/c/
cd ../server/
make re
cp server ../build/
cd ../client/c/
make re
cp client ../../build/
cd ../../test/
node test.js
