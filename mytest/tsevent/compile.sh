./libtool --mode=compile --tag=CXX g++ -g -O  -std=c++11 -I./src/db -I/usr/local/mysql/include/ -c ./src/db/mysql_db.cpp
./libtool --mode=compile --tag=CXX g++ -g -O -std=c++11 -I./src/event  -c ./src/event/event.cpp
./libtool --mode=compile --tag=CXX g++ -g -O -std=c++11 -I./src/event -I./src/db -c ./test/test.cpp
./libtool --mode=link --tag=CXX g++ -g -O -std=c++11 ./event.lo ./mysql_db.lo ./test.lo  -o ./test/test   -L/usr/local/mysql/lib -lmysqlclient -ldl 

#/usr/lib/x86_64-linux-gnu/libdl.a /usr/lib/x86_64-linux-gnu/libpthread.a
