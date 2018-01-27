EVENT_DIR=./src/event 
DB_DIR=./src/db
TEST_DIR=./test

EVENT_SRC=$(wildcard ./src/event/*.cpp)
EVENT_OBJ=$(patsubst %.cpp, %.o, $(EVENT_SRC))

TEST_ADDRESS_SRC=./test/testaddress.cpp
TEST_ADDRESS_OBJ=./test/testaddress.o

TEST_TX_SRC=./test/testtx.cpp
TEST_TX_OBJ=./test/testtx.o

TEST_EVENT_SRC=./test/testevent.cpp
TEST_EVENT_OBJ=./test/testevent.o

DB_SRC=$(wildcard ./src/db/*.cpp)
DB_OBJ=$(patsubst %.cpp, %.o, $(DB_SRC))

LIBSRC=$(EVENT_SRC) $(DB_SRC)
LIBOBJ=$(EVENT_OBJ) $(DB_OBJ)

CC=g++

CPPFLAGS += -g -I$(EVENT_DIR) -I$(DB_DIR) -I/usr/local/mysql/include -std=c++11
LDFLAGS = -L/usr/local/mysql/lib -lmysqlclient



all: target/testtx target/testevent target/testaddress

target/testtx: $(LIBOBJ) $(TEST_TX_OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

target/testaddress: $(LIBOBJ) $(TEST_ADDRESS_OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

target/testevent: $(LIBOBJ) $(TEST_EVENT_OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

$(EVENT_DIR)/%.o : $(EVENT_DIR)/%.cpp 
	$(CC) -o  $@ -c $< $(CPPFLAGS)

$(DB_DIR)/%.o : $(DB_DIR)/%.cpp 
	$(CC) -o  $@ -c $< $(CPPFLAGS)

$(TEST_DIR)/%.o : $(TEST_DIR)/%.cpp 
	$(CC) -o  $@ -c $< $(CPPFLAGS)



clean:
	rm -f $(TEST_ADDRESS_OBJ) $(TEST_TX_OBJ) $(TEST_EVENT_OBJ)
	rm -f $(EVENT_OBJ)
	rm -f $(DB_OBJ)
	rm -f target/testtx target/testaddress target/testevent