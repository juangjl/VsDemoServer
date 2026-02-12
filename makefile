TARGET   = $(CURDIR)

CC=g++
LL=g++

SRC=./src

OUT=./out
BAK=./bak

OS_NAME := $(shell uname -s)
#-----------------------------#
# OS TYPE
# WINDOWS : 0
# LINUXS  : 1
# MAC-OS  : 2
#-----------------------------
OS_TYPE_WINDOWS=0
OS_TYPE_UBUNTU=1
OS_TYPE_MAC_OS=2

OS_TYPE=OS_TYPE_WINDOWS
CFLAGS_OS= -Wno-conversion-null -Wno-pointer-arith
LFLAGS_OS=

ifeq ($(OS_NAME), Linux)
	CFLAGS_OS= -Wno-format-overflow  -Wno-conversion-null -Wno-pointer-arith $(CFLAG_BLUETOOTH) -I./ -Wno-format-truncation
	LFLAGS_OS= $(LFLAG_BLUETOOTH)

	CFLAG_SQLITE := $(shell pkg-config --cflags sqlite3 2>/dev/null)
	LFLAG_SQLITE := $(shell pkg-config --libs   sqlite3 2>/dev/null)

	OS_TYPE=OS_TYPE_UBUNTU
endif
 
ifeq ($(OS_NAME), Darwin)
	CFLAGS_OS= -Wno-format-security -Wno-deprecated-declarations	

	OS_TYPE=OS_TYPE_MAC_OS
endif

CFLAGS= -O0 -ansi -g  -Wno-unused-result -fstack-protector  -DOS_TYPE=$(OS_TYPE)  $(CFLAGS_OS)
LFLAGS= -lpthread -lm 

INC= -I./inc 

SOURCES := $(wildcard $(SRC)/*.cpp)

OBJECTS  := $(SOURCES:%.cpp=%.o)

$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LL) $(OBJECTS) $(LFLAGS) -o ${TARGET}.o
	@mv ${SRC}/*.o ${OUT} 2>/dev/null || true
	@mv ${TARGET}.o ${OUT}
	@echo "Linking complete!"

$(OBJECTS): $(OBJ)%.o : %.cpp
	@$(CC) $(CFLAGS) -c $< -o $@ ${INC}
	@echo "[CC] $<"

all:$(BINDIR)/$(TARGET)

clean:
	@rm -rf *.o *.out $(SRC)/*.o $(SRC)/*.out
	@rm -rf ${OUT}/*.o  *.o
