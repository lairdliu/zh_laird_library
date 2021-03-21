include ../../etc/make.ini

ifeq ($(ACRO),j6p)
    TARGET_PATH = $(PROJECT_ROOT_PATH)/target
    TARGET = $(PROJECT_ROOT_PATH)/../extern/lib/arm/libcmhiim.so
    STRIP = arm-linux-gnueabihf-strip

    PROJECT_ROOT_PATH = .
    PROJECT_SERVICE_PATH = $(PROJECT_ROOT_PATH)/service
    PROJECT_COMMON_PATH = $(PROJECT_ROOT_PATH)/common
    PROJECT_API_PATH = $(PROJECT_ROOT_PATH)/api
    PROJECT_INTERFACE_PATH = $(PROJECT_ROOT_PATH)/interface
    PROJECT_ACCOUNT_PATH = $(PROJECT_SERVICE_PATH)/account
    PROJECT_CONTACT_PATH = $(PROJECT_SERVICE_PATH)/contact
    PROJECT_CHAT_PATH = $(PROJECT_SERVICE_PATH)/chat
    PROJECT_DATABASE_PATH = $(PROJECT_COMMON_PATH)/database
    PROJECT_GRPC_PATH = $(PROJECT_COMMON_PATH)/grpc
    PROJECT_BASE64_PATH=$(PROJECT_COMMON_PATH)/base64
    PROJECT_HTTP_PATH = $(PROJECT_COMMON_PATH)/http
    PROJECT_JSON_PATH = $(PROJECT_COMMON_PATH)/json
    PROJECT_AMR_PATH = $(PROJECT_COMMON_PATH)/amr
    PROJECT_API_COMMON_PATH = $(PROJECT_API_PATH)/common
    PROJECT_API_OUTER_PATH = $(PROJECT_API_PATH)/outer

    PROJECT_NORMAL_LIB_PATH = $(PROJECT_ROOT_PATH)/lib/normal
    PROJECT_CROSS_LIB_PATH = $(PROJECT_ROOT_PATH)/lib/cross
    PROJECT_AMR_LIB_PATH = $(PROJECT_ROOT_PATH)/lib/amr/cross

    HOST_SYSTEM = $(shell uname | cut -f 1 -d_)
    SYSTEM ?= $(HOST_SYSTEM)

    #CXX = g++ -g
    #AR = ar rc
    CXXFLAGS += -std=c++11
    CXXFLAGS += -I$(PROJECT_COMMON_PATH) -I$(PROJECT_INTERFACE_PATH) -I$(PROJECT_ACCOUNT_PATH)\
                -I$(PROJECT_DATABASE_PATH) -I$(PROJECT_CONTACT_PATH) -I$(PROJECT_CHAT_PATH) \
                -I$(PROJECT_ROOT_PATH) -I$(PROJECT_GRPC_PATH) -I$(PROJECT_API_PATH) \
                -I$(PROJECT_HTTP_PATH)  -I$(PROJECT_AMR_PATH)  -I$(PROJECT_BASE64_PATH)

    ifeq ($(SYSTEM),Darwin)
    LDFLAGS += -L$(PROJECT_CROSS_LIB_PATH) -L$(PROJECT_AMR_LIB_PATH)\
               -lgrpc++_reflection\
               -ldl -lsqlite3 -lgrpc -lgrpc++ -lpthread -pthread -lcurl -luuid -lopencore-amrnb -lopencore-amrwb
    else
    LDFLAGS += -L$(PROJECT_CROSS_LIB_PATH) -L$(PROJECT_AMR_LIB_PATH)\
               -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed\
               -ldl -lsqlite3 -lgrpc -lgrpc++ -lpthread -pthread -lcurl -luuid -lopencore-amrnb -lopencore-amrwb
    endif

    SOURCES += $(wildcard $(PROJECT_API_COMMON_PATH)/*.cc)
    SOURCES += $(PROJECT_API_OUTER_PATH)/connector.pb.cc\
        $(PROJECT_API_OUTER_PATH)/grpc.pb.cc\
        $(PROJECT_API_OUTER_PATH)/grpc.grpc.pb.cc\
        $(PROJECT_API_OUTER_PATH)/chat.pb.cc\
        $(PROJECT_API_OUTER_PATH)/conversation.pb.cc\
        $(PROJECT_API_OUTER_PATH)/friends.pb.cc\
        $(PROJECT_API_OUTER_PATH)/history.pb.cc\
        $(PROJECT_API_OUTER_PATH)/user.pb.cc
    SOURCES += $(wildcard $(PROJECT_BASE64_PATH)/*.cc)
    SOURCES += $(wildcard $(PROJECT_COMMON_PATH)/*.cc)
    SOURCES += $(wildcard $(PROJECT_GRPC_PATH)/*.cc)
    SOURCES += $(wildcard $(PROJECT_DATABASE_PATH)/*.cc)
    SOURCES += $(wildcard $(PROJECT_HTTP_PATH)/*.cc)
    SOURCES += $(wildcard $(PROJECT_JSON_PATH)/*.cc)
    SOURCES += $(wildcard $(PROJECT_CHAT_PATH)/*.cc)
    SOURCES += $(wildcard $(PROJECT_AMR_PATH)/*.cc)
    SOURCES += $(wildcard $(PROJECT_CONTACT_PATH)/*.cc)
    SOURCES += $(wildcard $(PROJECT_ACCOUNT_PATH)/*.cc)
    SOURCES += $(PROJECT_ROOT_PATH)/cmhi_im_client_imp.cc
    OBJECTS = $(patsubst %.cc,%.o,$(SOURCES))


else ifeq ($(ACRO),j6)
    TARGET_PATH = $(PROJECT_ROOT_PATH)/target
    TARGET = $(PROJECT_ROOT_PATH)/../extern/lib/arm_j6/libcmhiim.so
    STRIP = arm-linux-gnueabihf-strip

    PROJECT_ROOT_PATH = .
    PROJECT_SERVICE_PATH = $(PROJECT_ROOT_PATH)/service
    PROJECT_COMMON_PATH = $(PROJECT_ROOT_PATH)/common
    PROJECT_API_PATH = $(PROJECT_ROOT_PATH)/api
    PROJECT_INTERFACE_PATH = $(PROJECT_ROOT_PATH)/interface
    PROJECT_ACCOUNT_PATH = $(PROJECT_SERVICE_PATH)/account
    PROJECT_CONTACT_PATH = $(PROJECT_SERVICE_PATH)/contact
    PROJECT_CHAT_PATH = $(PROJECT_SERVICE_PATH)/chat
    PROJECT_DATABASE_PATH = $(PROJECT_COMMON_PATH)/database
    PROJECT_GRPC_PATH = $(PROJECT_COMMON_PATH)/grpc
    PROJECT_BASE64_PATH=$(PROJECT_COMMON_PATH)/base64
    PROJECT_HTTP_PATH = $(PROJECT_COMMON_PATH)/http
    PROJECT_JSON_PATH = $(PROJECT_COMMON_PATH)/json
    PROJECT_AMR_PATH = $(PROJECT_COMMON_PATH)/amr
    PROJECT_API_COMMON_PATH = $(PROJECT_API_PATH)/common
    PROJECT_API_OUTER_PATH = $(PROJECT_API_PATH)/outer

    PROJECT_NORMAL_LIB_PATH = $(PROJECT_ROOT_PATH)/lib/normal
    PROJECT_CROSS_LIB_PATH = $(PROJECT_ROOT_PATH)/lib/cross
    PROJECT_AMR_LIB_PATH = $(PROJECT_ROOT_PATH)/lib/amr/cross

    HOST_SYSTEM = $(shell uname | cut -f 1 -d_)
    SYSTEM ?= $(HOST_SYSTEM)

    #CXX = g++ -g
    #AR = ar rc
    CXXFLAGS += -std=c++11
    CXXFLAGS += -I$(PROJECT_COMMON_PATH) -I$(PROJECT_INTERFACE_PATH) -I$(PROJECT_ACCOUNT_PATH)\
                -I$(PROJECT_DATABASE_PATH) -I$(PROJECT_CONTACT_PATH) -I$(PROJECT_CHAT_PATH) \
                -I$(PROJECT_ROOT_PATH) -I$(PROJECT_GRPC_PATH) -I$(PROJECT_API_PATH) \
                -I$(PROJECT_HTTP_PATH)  -I$(PROJECT_AMR_PATH)  -I$(PROJECT_BASE64_PATH)

    ifeq ($(SYSTEM),Darwin)
    LDFLAGS += -L$(PROJECT_CROSS_LIB_PATH) -L$(PROJECT_AMR_LIB_PATH)\
               -lgrpc++_reflection\
               -ldl -lsqlite3 -lgrpc -lgrpc++ -lpthread -pthread -lcurl -luuid -lopencore-amrnb -lopencore-amrwb
    else
    LDFLAGS += -L$(PROJECT_CROSS_LIB_PATH) -L$(PROJECT_AMR_LIB_PATH)\
               -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed\
               -ldl -lsqlite3 -lgrpc -lgrpc++ -lpthread -pthread -lcurl -luuid -lopencore-amrnb -lopencore-amrwb
    endif


    SOURCES += $(wildcard $(PROJECT_API_COMMON_PATH)/*.cc)
    SOURCES += $(PROJECT_API_OUTER_PATH)/connector.pb.cc\
        $(PROJECT_API_OUTER_PATH)/grpc.pb.cc\
        $(PROJECT_API_OUTER_PATH)/grpc.grpc.pb.cc\
        $(PROJECT_API_OUTER_PATH)/chat.pb.cc\
        $(PROJECT_API_OUTER_PATH)/conversation.pb.cc\
        $(PROJECT_API_OUTER_PATH)/friends.pb.cc\
        $(PROJECT_API_OUTER_PATH)/history.pb.cc\
        $(PROJECT_API_OUTER_PATH)/user.pb.cc

    SOURCES += $(wildcard $(PROJECT_BASE64_PATH)/*.cc)
    SOURCES += $(wildcard $(PROJECT_COMMON_PATH)/*.cc)
    SOURCES += $(wildcard $(PROJECT_GRPC_PATH)/*.cc)
    SOURCES += $(wildcard $(PROJECT_DATABASE_PATH)/*.cc)
    SOURCES += $(wildcard $(PROJECT_HTTP_PATH)/*.cc)
    SOURCES += $(wildcard $(PROJECT_JSON_PATH)/*.cc)
    SOURCES += $(wildcard $(PROJECT_CHAT_PATH)/*.cc)
    SOURCES += $(wildcard $(PROJECT_AMR_PATH)/*.cc)
    SOURCES += $(wildcard $(PROJECT_CONTACT_PATH)/*.cc)
    SOURCES += $(wildcard $(PROJECT_ACCOUNT_PATH)/*.cc)
    SOURCES += $(PROJECT_ROOT_PATH)/cmhi_im_client_imp.cc
    OBJECTS = $(patsubst %.cc,%.o,$(SOURCES))

else ifeq ($(ACRO),2712)
    TARGET_PATH = $(PROJECT_ROOT_PATH)/target
    TARGET = $(PROJECT_ROOT_PATH)/../extern/lib/2712/libcmhiim.so
    #STRIP = arm-linux-gnueabihf-strip

    PROJECT_ROOT_PATH = .
    PROJECT_SERVICE_PATH = $(PROJECT_ROOT_PATH)/service
    PROJECT_COMMON_PATH = $(PROJECT_ROOT_PATH)/common
    PROJECT_API_PATH = $(PROJECT_ROOT_PATH)/api
    PROJECT_INTERFACE_PATH = $(PROJECT_ROOT_PATH)/interface
    PROJECT_ACCOUNT_PATH = $(PROJECT_SERVICE_PATH)/account
    PROJECT_CONTACT_PATH = $(PROJECT_SERVICE_PATH)/contact
    PROJECT_CHAT_PATH = $(PROJECT_SERVICE_PATH)/chat
    PROJECT_DATABASE_PATH = $(PROJECT_COMMON_PATH)/database
    PROJECT_GRPC_PATH = $(PROJECT_COMMON_PATH)/grpc
    PROJECT_BASE64_PATH=$(PROJECT_COMMON_PATH)/base64
    PROJECT_HTTP_PATH = $(PROJECT_COMMON_PATH)/http
    PROJECT_JSON_PATH = $(PROJECT_COMMON_PATH)/json
    PROJECT_AMR_PATH = $(PROJECT_COMMON_PATH)/amr
    PROJECT_API_COMMON_PATH = $(PROJECT_API_PATH)/common
    PROJECT_API_OUTER_PATH = $(PROJECT_API_PATH)/outer

    #PROJECT_NORMAL_LIB_PATH = $(PROJECT_ROOT_PATH)/lib/normal
    #PROJECT_CROSS_LIB_PATH = $(PROJECT_ROOT_PATH)/lib/2712
    #PROJECT_AMR_LIB_PATH = $(PROJECT_ROOT_PATH)/lib/amr/2712

    HOST_SYSTEM = $(shell uname | cut -f 1 -d_)
    SYSTEM ?= $(HOST_SYSTEM)

    #CXX = g++ -g
    AR := $(AR) rc
    CXXFLAGS += -std=c++11
    CXXFLAGS += -I$(PROJECT_COMMON_PATH) -I$(PROJECT_INTERFACE_PATH) -I$(PROJECT_ACCOUNT_PATH)\
                -I$(PROJECT_DATABASE_PATH) -I$(PROJECT_CONTACT_PATH) -I$(PROJECT_CHAT_PATH) \
                -I$(PROJECT_ROOT_PATH) -I$(PROJECT_GRPC_PATH) -I$(PROJECT_API_PATH) \
                -I$(PROJECT_HTTP_PATH)  -I$(PROJECT_AMR_PATH)  -I$(PROJECT_BASE64_PATH)

    ifeq ($(SYSTEM),Darwin)
    LDFLAGS += -L$(PROJECT_CROSS_LIB_PATH) -L$(PROJECT_AMR_LIB_PATH)\
               -lgrpc++_reflection\
               -ldl -lsqlite3 -lgrpc -lgrpc++ -lpthread -pthread -lcurl -luuid -lopencore-amrnb -lopencore-amrwb
    else
    LDFLAGS += -L$(PROJECT_CROSS_LIB_PATH) -L$(PROJECT_AMR_LIB_PATH)\
               -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed\
               -ldl -lsqlite3 -lgrpc -lgrpc++ -lpthread -pthread -lcurl -luuid -lopencore-amrnb -lopencore-amrwb
    endif


    SOURCES += $(wildcard $(PROJECT_API_COMMON_PATH)/*.cc)
    SOURCES += $(PROJECT_API_OUTER_PATH)/connector.pb.cc\
        $(PROJECT_API_OUTER_PATH)/grpc.pb.cc\
        $(PROJECT_API_OUTER_PATH)/grpc.grpc.pb.cc\
        $(PROJECT_API_OUTER_PATH)/chat.pb.cc\
        $(PROJECT_API_OUTER_PATH)/conversation.pb.cc\
        $(PROJECT_API_OUTER_PATH)/friends.pb.cc\
        $(PROJECT_API_OUTER_PATH)/history.pb.cc\
        $(PROJECT_API_OUTER_PATH)/user.pb.cc

    SOURCES += $(wildcard $(PROJECT_BASE64_PATH)/*.cc)
    SOURCES += $(wildcard $(PROJECT_COMMON_PATH)/*.cc)
    SOURCES += $(wildcard $(PROJECT_GRPC_PATH)/*.cc)
    SOURCES += $(wildcard $(PROJECT_DATABASE_PATH)/*.cc)
    SOURCES += $(wildcard $(PROJECT_HTTP_PATH)/*.cc)
    SOURCES += $(wildcard $(PROJECT_JSON_PATH)/*.cc)
    SOURCES += $(wildcard $(PROJECT_CHAT_PATH)/*.cc)
    SOURCES += $(wildcard $(PROJECT_AMR_PATH)/*.cc)
    SOURCES += $(wildcard $(PROJECT_CONTACT_PATH)/*.cc)
    SOURCES += $(wildcard $(PROJECT_ACCOUNT_PATH)/*.cc)
    SOURCES += $(PROJECT_ROOT_PATH)/cmhi_im_client_imp.cc
    OBJECTS = $(patsubst %.cc,%.o,$(SOURCES))

else
        TARGET_PATH = $(PROJECT_ROOT_PATH)/target
        TARGET = $(PROJECT_ROOT_PATH)/../extern/lib/x86/libcmhiim.so
        STRIP = strip

        PROJECT_ROOT_PATH = .
        PROJECT_SERVICE_PATH = $(PROJECT_ROOT_PATH)/service
        PROJECT_COMMON_PATH = $(PROJECT_ROOT_PATH)/common
        PROJECT_API_PATH = $(PROJECT_ROOT_PATH)/api
        PROJECT_INTERFACE_PATH = $(PROJECT_ROOT_PATH)/interface
        PROJECT_ACCOUNT_PATH = $(PROJECT_SERVICE_PATH)/account
        PROJECT_CONTACT_PATH = $(PROJECT_SERVICE_PATH)/contact
        PROJECT_CHAT_PATH = $(PROJECT_SERVICE_PATH)/chat
        PROJECT_DATABASE_PATH = $(PROJECT_COMMON_PATH)/database
        PROJECT_BASE64_PATH=$(PROJECT_COMMON_PATH)/base64
        PROJECT_GRPC_PATH = $(PROJECT_COMMON_PATH)/grpc
        PROJECT_HTTP_PATH = $(PROJECT_COMMON_PATH)/http
        PROJECT_JSON_PATH = $(PROJECT_COMMON_PATH)/json
        PROJECT_AMR_PATH = $(PROJECT_COMMON_PATH)/amr
        PROJECT_API_COMMON_PATH = $(PROJECT_API_PATH)/common
        PROJECT_API_OUTER_PATH = $(PROJECT_API_PATH)/outer

        PROJECT_NORMAL_LIB_PATH = $(PROJECT_ROOT_PATH)/lib/normal
        PROJECT_CROSS_LIB_PATH = $(PROJECT_ROOT_PATH)/lib/cross
        PROJECT_AMR_LIB_PATH = $(PROJECT_ROOT_PATH)/lib/amr/ubuntu

        HOST_SYSTEM = $(shell uname | cut -f 1 -d_)
        SYSTEM ?= $(HOST_SYSTEM)

        CXX = g++ -g
        AR = ar rc
        CXXFLAGS += -std=c++11
        CXXFLAGS += -I$(PROJECT_COMMON_PATH) -I$(PROJECT_INTERFACE_PATH) -I$(PROJECT_ACCOUNT_PATH)\
                    -I$(PROJECT_DATABASE_PATH) -I$(PROJECT_CONTACT_PATH) -I$(PROJECT_CHAT_PATH) \
                    -I$(PROJECT_ROOT_PATH) -I$(PROJECT_GRPC_PATH) -I$(PROJECT_API_PATH) \
                    -I$(PROJECT_HTTP_PATH) -I$(PROJECT_AMR_PATH) -I$(PROJECT_BASE64_PATH)

        ifeq ($(SYSTEM),Darwin)
        LDFLAGS += -L/usr/local/lib -L$(PROJECT_NORMAL_LIB_PATH) -L$(PROJECT_AMR_LIB_PATH)\
                   -lgrpc++_reflection\
                   -ldl -lsqlite3 -lgrpc -lgrpc++ -lprotobuf -lpthread -pthread -lcurl -luuid -lopencore-amrnb -lopencore-amrwb
        else
        LDFLAGS += -L/usr/local/lib -L$(PROJECT_NORMAL_LIB_PATH) -L$(PROJECT_AMR_LIB_PATH)\
                   -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed\
                   -ldl -lsqlite3 -lgrpc -lgrpc++ -lprotobuf -lpthread -pthread -lcurl -luuid -lopencore-amrnb -lopencore-amrwb
        endif


        SOURCES += $(wildcard $(PROJECT_API_COMMON_PATH)/*.cc)
        SOURCES += $(PROJECT_API_OUTER_PATH)/connector.pb.cc\
            $(PROJECT_API_OUTER_PATH)/grpc.pb.cc\
            $(PROJECT_API_OUTER_PATH)/grpc.grpc.pb.cc\
            $(PROJECT_API_OUTER_PATH)/chat.pb.cc\
            $(PROJECT_API_OUTER_PATH)/conversation.pb.cc\
            $(PROJECT_API_OUTER_PATH)/friends.pb.cc\
            $(PROJECT_API_OUTER_PATH)/history.pb.cc\
            $(PROJECT_API_OUTER_PATH)/user.pb.cc
        SOURCES += $(wildcard $(PROJECT_BASE64_PATH)/*.cc)
        SOURCES += $(wildcard $(PROJECT_COMMON_PATH)/*.cc)
        SOURCES += $(wildcard $(PROJECT_GRPC_PATH)/*.cc)
        SOURCES += $(wildcard $(PROJECT_DATABASE_PATH)/*.cc)
        SOURCES += $(wildcard $(PROJECT_HTTP_PATH)/*.cc)
        SOURCES += $(wildcard $(PROJECT_JSON_PATH)/*.cc)
        SOURCES += $(wildcard $(PROJECT_AMR_PATH)/*.cc)
        SOURCES += $(wildcard $(PROJECT_CHAT_PATH)/*.cc)
        SOURCES += $(wildcard $(PROJECT_CONTACT_PATH)/*.cc)
        SOURCES += $(wildcard $(PROJECT_ACCOUNT_PATH)/*.cc)
        SOURCES += $(PROJECT_ROOT_PATH)/cmhi_im_client_imp.cc
        OBJECTS = $(patsubst %.cc,%.o,$(SOURCES))

endif

all:$(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) -shared -fPIC $^ $(LDFLAGS) -o $@
	$(STRIP) $(TARGET)
#	@rm -f $(OBJECTS)

#=============================================================================
# Compile rules
#=============================================================================
%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -fPIC $^ $(LDFLAGS) -o $@

clean:
	rm -f $(OBJECTS)
	rm -f $(TARGET)
	@find . -name \*.o | xargs -I {} rm -fr {}


