# this is a makefile for caffe  
TARGET := test 
# caffe directory  
CAFFE_DIR := /usr/local/
# caffe include  
CAFFE_INCLUDE := $(CAFFE_DIR)/include/caffe
# caffe build   
CAFFE_INSTALL := $(CAFFE_DIR)
# caffe build lib  
CAFFE_BUILD_LIB := $(CAFFE_BUILD)/lib  
# caffe build src  
CAFFE_BUILD_SRC := $(CAFFE_BUILD)/src  
 
# dependency libraries and caffe linking libs  
LFLAGS := -pthread -lnsl -lX11  
LFLAGS += -lcaffe -lglog -lgflags -lopencv_flann -lprotobuf -lboost_system -lboost_filesystem  -lboost_thread -lboost_coroutine -lboost_context -lboost_regex -lm  -lleveldb -lsnappy -llmdb  -lopencv_core -lopencv_highgui -lopencv_objdetect -lstdc++ -lcblas -latlas  
 
LFLAGS += -Wl,-rpath=$(CAFFE_INSTALL) -L/usr/lib/x86_64-linux-gnu/hdf5/serial
LFLAGS += -L/usr/local/lib -lopencv_core  -lopencv_highgui
 
# rules  
CFLAGS := -g -I . -I $(CAFFE_INCLUDE) -I $(CAFFE_DIR)/src -I $(CAFFE_BUILD_SRC) -I/usr/include/hdf5/serial -I ./ -Wall -DCPU_ONLY -DUSE_OPENCV  
CC := g++  
 
SRC += write_caffe.cpp  #这里一定是所有的cpp文件
 
$(TARGET):$(SRC)  
	$(CC) -o $(TARGET) -O0 -std=c++11 $(SRC) $(CFLAGS) $(LFLAGS) -L$(CAFFE_BUILD_LIB) -Wno-sign-compare  
 
clean :  
	@rm -f $(TARGET)

