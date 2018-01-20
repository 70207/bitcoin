EVENT_DIR=./src/event 
DB_DIR=./src/db
TEST_DIR=./test

EVENT_SRC=$(wildcard ./src/event/*.cpp)
EVENT_OBJ=$(patsubst %.cpp, %.o, $(EVENT_SRC))

TEST_SRC=$(wildcard ./test/*.cpp)
TEST_OBJ=$(patsubst %.cpp, %.o, $(TEST_SRC))

DB_SRC=$(wildcard ./src/db/*.cpp)
DB_OBJ=$(patsubst %.cpp, %.o, $(DB_SRC))

SRC=$(EVENT_SRC) $(TEST_SRC) $(DB_SRC)
OBJ=$(EVENT_OBJ) $(TEST_OBJ) $(DB_OBJ)

CC=g++

CPPFLAGS += -g -I$(EVENT_DIR) -I$(DB_DIR) -I/usr/local/mysql/include -std=c++11
LDFLAGS = -L/usr/local/mysql/lib -lmysqlclient

all: test/test

test/test: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

$(EVENT_DIR)/%.o : $(EVENT_DIR)/%.cpp 
	$(CC) -o  $@ -c $< $(CPPFLAGS)

$(DB_DIR)/%.o : $(DB_DIR)/%.cpp 
	$(CC) -o  $@ -c $< $(CPPFLAGS)

$(TEST_DIR)/%.o : $(TEST_DIR)/%.cpp 
	$(CC) -o  $@ -c $< $(CPPFLAGS)



clean:
	rm -f $(TEST_OBJ)
	rm -f $(EVENT_OBJ)
	rm -f $(DB_OBJ)
	rm -f test/test