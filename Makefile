THIRDPARTY_DIR = third_party

AES_DIR = $(THIRDPARTY_DIR)/aes

# TODO: create configure.ac script such that we don't have hard-code paths
ifeq ($(GMP_DIR),1)
GMP_DIR = /usr/local
endif
GMP_LIB_DIR = $(GMP_DIR)/lib
GMP_INC_DIR = $(GMP_DIR)/include

GTEST_DIR = $(THIRDPARTY_DIR)/gtest-1.7.0
GTEST_LIB_DIR = $(GTEST_DIR)/lib/.libs
GTEST_INC_DIR = $(GTEST_DIR)/include

# TODO: remove hard-coded compilers. however, for now we only test/support emcc/em++
CC = emcc
CXX = em++
CXXFLAGS = -s DISABLE_EXCEPTION_CATCHING=0 --closure 1
AR = ar
NODEJS = nodejs
ARFLAGS = rsc
CFLAGS_ = $(CFLAGS) -g0 -O3 -Wall
CXXFLAGS_  = $(CXXFLAGS) -g0 -O3 -Wall -Isrc -I$(THIRDPARTY_DIR) -I$(GTEST_INC_DIR) -I$(GMP_INC_DIR)
LDFLAGS_ = $(LDFLAGS) -L. -L$(GTEST_LIB_DIR) -L$(GMP_LIB_DIR) -lgtest -lgmp -lgmpxx -lfte 

# the build target executable:
TARGET_TESTJS = bin/test.js
OBJ_TEST = src/tests.o \
           src/fte/ranking/sample_dfas.o \
           src/fte/test_fte.o \
           src/ffx/test_ffx.o \
           src/fte/ranking/test_dfa.o

TARGET_MAINJS = bin/main.js
OBJ_MAIN = src/main.o \
           src/fte/ranking/sample_dfas.o

TARGET_LIBFTE = libfte.a
OBJ_LIBFTE = src/ffx/conversions.o \
             src/ffx/key.o \
             src/ffx/encryption.o \
             src/ffx/ffx.o \
             src/fte/fte.o \
             src/fte/ranking/dfa.o

TARGET_LIBAES = $(THIRDPARTY_DIR)/aes/libaes.a
OBJ_LIBAES = $(THIRDPARTY_DIR)/aes/aes_modes.o \
          $(THIRDPARTY_DIR)/aes/aescrypt.o \
          $(THIRDPARTY_DIR)/aes/aeskey.o \
          $(THIRDPARTY_DIR)/aes/aestab.o

TARGET_GTEST = $(GTEST_LIB_DIR)/libgtest.a


default: $(TARGET_TEST) $(TARGET_MAIN)

$(TARGET_GTEST):
	cd $(GTEST_DIR) && ./configure --enable-static --disable-shared && $(MAKE)

%.o: %.c
	$(CC) $(CFLAGS_) -c -o $@ $<

%.o: %.cc
	$(CXX) $(CXXFLAGS_) -c -o $@ $<

$(TARGET_TESTJS): $(TARGET_GTEST) $(TARGET_LIBFTE) $(OBJ_TEST)
	$(CXX) $(CXXFLAGS_) $(LDFLAGS_) -o $@ $(OBJ_TEST)

$(TARGET_MAINJS): $(TARGET_LIBFTE) $(OBJ_MAIN)
	$(CXX) $(CXXFLAGS_) $(LDFLAGS_) -o $@ $(OBJ_MAIN)

$(TARGET_LIBAES): $(OBJ_LIBAES)
	$(AR) $(ARFLAGS) $(TARGET_LIBAES) $^

$(TARGET_LIBFTE): $(OBJ_LIBFTE) $(OBJ_LIBAES)
	$(AR) $(ARFLAGS) $(TARGET_LIBFTE) $^

test: $(TARGET_TESTJS)
	$(NODEJS) $(TARGET_TESTJS)

clean:
	$(RM) $(TARGET_MAINJS)
	$(RM) $(TARGET_LIBFTE)
	$(RM) $(OBJ_LIBAES)
	$(RM) $(OBJ_LIBFTE)
	$(RM) $(OBJ_MAIN)
	$(RM) $(OBJ_TEST)
	$(RM) bin/*.map
