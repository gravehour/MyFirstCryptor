CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

ifeq ($(OS),Windows_NT)
    detected_OS := Windows
else
    detected_OS := $(shell uname -s)
endif

ifeq ($(detected_OS),Windows)
    INCLUDES =
    LDFLAGS  = -lssl -lcrypto
    TARGET   = bin/cryptor.exe
    MKDIR    = mkdir -p

else ifeq ($(detected_OS),Darwin)
    ifeq ($(shell uname -m),arm64)
        OPENSSL_PREFIX = /opt/homebrew/opt/openssl@3
    else
        OPENSSL_PREFIX = /usr/local/opt/openssl@3
    endif

    INCLUDES = -I$(OPENSSL_PREFIX)/include
    LDFLAGS  = -L$(OPENSSL_PREFIX)/lib -lssl -lcrypto
    TARGET   = bin/cryptor
    MKDIR    = mkdir -p

else
    INCLUDES = $(shell pkg-config --cflags openssl 2>/dev/null || echo "")
    LDFLAGS  = $(shell pkg-config --libs openssl 2>/dev/null || echo "-lssl -lcrypto")
    TARGET   = bin/cryptor
    MKDIR    = mkdir -p
endif

SRC = cryptor.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(MKDIR) $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
