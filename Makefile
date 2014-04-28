GTEST_DIR = thirdparty/gtest-1.7.0
GMP_DIR = /usr/local/lib

# the compiler: gcc for C program, define as g++ for C++
CXX = g++

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CXXFLAGS  = -g -Wall -Isrc -I$(GTEST_DIR)/include
LDFLAGS = -L$(GMP_DIR) -L$(GTEST_DIR) -lgmpxx -lgmp -lgtest

# the build target executable:
TARGET_TEST = bin/test
OBJ_TEST = src/test.o src/ranker.o src/tests/dfas.o src/tests/cause_exceptions.o src/tests/rank_unrank.o

TARGET_MAIN = bin/main
OBJ_MAIN = src/main.o src/ranker.o src/tests/dfas.o

all: $(GTEST_DIR)/libgtest.a $(TARGET_TEST) $(TARGET_MAIN)

$(GTEST_DIR)/libgtest.a:
	cd $(GTEST_DIR) && cmake . && $(MAKE)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(TARGET_TEST): $(OBJ_TEST)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

$(TARGET_MAIN): $(OBJ_MAIN)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

test:
	@./$(TARGET_TEST)

clean:
	$(RM) $(TARGET_TEST)
	$(RM) $(OBJ_TEST)
	$(RM) $(TARGET_MAIN)
	$(RM) $(OBJ_MAIN)
