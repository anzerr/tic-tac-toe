mkdir -p ../build/
rm ../build/server ../build/client
cd ../server/
make re
cp server ../build/
cd ../client/c/
make re
cp client ../../build/
cd ../../test/
node test.js
