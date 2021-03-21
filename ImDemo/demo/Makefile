TARGET=cmhi_cross

PROJECT_ROOT_PATH = ..
PROJECT_INCLUDE_PATH = $(PROJECT_ROOT_PATH)/interface
PROJECT_LIB_PATH = $(PROJECT_ROOT_PATH)/target

HOST_SYSTEM = $(shell uname | cut -f 1 -d_)
SYSTEM ?= $(HOST_SYSTEM)

#CXX = arm-linux-gnueabihf-g++
CXXFLAGS += -std=c++11
CXXFLAGS += -I$(PROJECT_INCLUDE_PATH)

LDFLAGS += -L$(PROJECT_LIB_PATH) -lcmhiim_cross

LDFLAGS += -L/home/zty/share/iov_linux_try/src/thirdpary/amr_lib/j6p  -lopencore-amrwb -lopencore-amrnb


OBJECTS	= \
	main.o

	
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $^ $(LDFLAGS) -o $@
	@rm -f $(OBJECTS)

clean:
	@rm -f $(OBJECTS)
	@rm -f $(TARGET)
	
