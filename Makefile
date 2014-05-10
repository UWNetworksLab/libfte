GTEST_DIR = thirdparty/gtest-1.7.0
AES_DIR = thirdparty/aes

# the compiler: gcc for C program, define as g++ for C++
CC = emcc
CXX = em++
#AR = emar

ARFLAGS = rsc
CFLAGS = -O3 -Wall
CXXFLAGS  = -O3 -Wall -I/vagrant/sandbox/emscripten/system/lib/libcxxabi/include -I$(GMP_DIR) -Isrc -Ithirdparty -I$(GTEST_DIR)/include
LDFLAGS = -L$(GMP_DIR)/.libs -L$(AES_DIR) -L.libs -L$(GTEST_DIR) -lgtest -lgmp -lfte 

# the build target executable:
TARGET_TEST = bin/test
TARGET_TESTJS = bin/test.js
OBJ_TEST = src/tests.o \
           src/tests/dfas.o \
           src/tests/test_errors.o \
           src/tests/test_fte.o \
           src/tests/test_ffx.o \
           src/tests/test_ranker.o

TARGET_MAIN = bin/main
TARGET_MAINJS = bin/main.js
OBJ_MAIN = src/main.o \
           src/tests/dfas.o

TARGET_LIBFTE = .libs/libfte.a
OBJ_LIBFTE = src/ffx/conversions.o \
             src/ffx/key.o \
             src/ffx/encryption.o \
             src/ffx/ffx.o \
             src/fte/fte.o \
             src/fte/ranking/dfa.o

TARGET_LIBAES = .libs/libaes.a
OBJ_LIBAES = thirdparty/aes/aes_modes.o \
          thirdparty/aes/aescrypt.o \
          thirdparty/aes/aeskey.o \
          thirdparty/aes/aestab.o

TARGET_GTEST = $(GTEST_DIR)/libgtest.a

all: $(GTEST_DIR)/libgtest.a $(TARGET_TEST) $(TARGET_MAIN) $(TARGET_LIBFTE)

$(TARGET_GTEST):
	cd $(GTEST_DIR) && cmake . && $(MAKE)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(TARGET_TEST): $(TARGET_GTEST) $(OBJ_TEST)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

$(TARGET_TESTJS): $(TARGET_GTEST) $(TARGET_LIBFTE) $(OBJ_TEST)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(OBJ_TEST)

$(TARGET_MAIN): $(OBJ_MAIN) $(TARGET_LIBFTE)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@

$(TARGET_MAINJS): $(TARGET_LIBFTE) $(OBJ_MAIN)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(OBJ_MAIN)

$(TARGET_LIBAES): $(OBJ_LIBAES)
	$(AR) $(ARFLAGS) $(TARGET_LIBAES) $^

$(TARGET_LIBFTE): $(OBJ_LIBFTE) $(OBJ_LIBAES)
	$(AR) $(ARFLAGS) $(TARGET_LIBFTE) $^

test:
	@./$(TARGET_TEST)

clean:
	$(RM) $(TARGET_TEST)
	$(RM) $(TARGET_MAIN)
	$(RM) $(TARGET_MAINJS)
	$(RM) $(TARGET_LIBFTE)
	$(RM) $(OBJ_LIBAES)
	$(RM) $(OBJ_LIBFTE)
	$(RM) $(OBJ_MAIN)
	$(RM) $(OBJ_TEST)
	$(RM) bin/*.map
