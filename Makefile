GTEST_DIR = thirdparty/gtest-1.7.0
AES_DIR = thirdparty/aes

# the compiler: gcc for C program, define as g++ for C++
CC = emcc
CXX = em++
AR = emar

CXXFLAGS  = -O0 -Wall -I$(GMP_DIR) -Isrc -Ithirdparty -I$(GTEST_DIR)/include
LDFLAGS = -L$(GMP_DIR)/.libs -L$(AES_DIR) -L$(GTEST_DIR) -lgmp

# the build target executable:
TARGET_TEST = bin/test
OBJ_TEST = src/tests.o \
           src/ffx/conversions.o \
           src/ffx/encryption.o \
           src/ffx/key.o \
           src/ffx/ffx.o \
           src/fte/fte.o \
           src/fte/ranking/dfa.o \
           src/tests/dfas.o \
           src/tests/test_errors.o \
           src/tests/test_fte.o \
           src/tests/test_ffx.o \
           src/tests/test_ranker.o

TARGET_MAIN = bin/main
TARGET_MAINJS = bin/main.js
OBJ_MAIN = src/main.o \
           src/ffx/conversions.o \
           src/ffx/encryption.o \
           src/ffx/key.o \
           src/ffx/ffx.o \
           src/fte/fte.o \
           src/fte/ranking/dfa.o \
           src/tests/dfas.o

TARGET_LIBFTE = .libs/libfte.a
TARGET_LIBFTEJS = .libs/libfte.js
OBJ_LIBFTE = src/ffx/conversions.o \
             src/ffx/encryption.o \
             src/ffx/key.o \
             src/ffx/ffx.o \
             src/fte/fte.o \
             src/fte/ranking/dfa.o \

OBJ_AES = thirdparty/aes/aes_modes.o \
          thirdparty/aes/aescrypt.o \
          thirdparty/aes/aeskey.o \
          thirdparty/aes/aestab.o

all: $(GTEST_DIR)/libgtest.a $(LIBAES) $(TARGET_TEST) $(TARGET_MAIN) $(TARGET_LIBFTE)

$(GTEST_DIR)/libgtest.a:
	cd $(GTEST_DIR) && cmake . && $(MAKE)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(TARGET_TEST): $(OBJ_TEST)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

$(TARGET_MAIN): $(OBJ_MAIN) $(OBJ_AES)
	$(CXX) $(CXXFLAGS) $(OBJ_MAIN) $(OBJ_AES) $(LDFLAGS) -o $@ $^

$(TARGET_MAINJS): $(OBJ_MAIN) $(OBJ_AES)
	$(CXX) $(CXXFLAGS) $(OBJ_MAIN) $(OBJ_AES) $(LDFLAGS) -o $@ $^

$(LIBAES): $(OBJ_AES)
	$(AR) rsc $(LIBAES) $(OBJ_AES)

$(TARGET_LIBFTE): $(LIBAES) $(OBJ_LIBFTE)
	$(AR) rsc $(TARGET_LIBFTE) $(OBJ_LIBFTE) $(OBJ_AES)

$(TARGET_LIBFTEJS): $(LIBAESJS) $(OBJ_LIBFTE)
	$(AR) rsc $(TARGET_LIBFTEJS) $(OBJ_LIBFTE) $(LIBAES)

test:
	@./$(TARGET_TEST)

clean:
	$(RM) $(TARGET_TEST)
	$(RM) $(OBJ_TEST)
	$(RM) $(TARGET_MAIN)
	$(RM) $(OBJ_MAIN)
	$(RM) $(LIBAES)
	$(RM) $(OBJ_AES)
	$(RM) $(TARGET_LIBFTE)
	$(RM) $(TARGET_LIBFTEJS)
