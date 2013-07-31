EXEC_TEST=scribe_test
EXEC_STDIN=scribe_stdin
BUILD_DIR=../output

CC=g++
LIBS=../scribe_lib/thrift-0.9.0/lib/libthrift.a    -L../scribe_lib/thrift-0.9.0/lib 
INCLUDE=-I../scribe_lib/thrift-0.9.0/include/  -I../scribe_lib/thrift-0.9.0/include/thrift -I../scribe_lib/fb303/include/ -I../gen-cpp/
GEN_SRC=../gen-cpp/FacebookService.cpp  ../gen-cpp/fb303_constants.cpp ../gen-cpp/scribe_constants.cpp  \
		../gen-cpp/fb303_types.cpp  ../gen-cpp/scribe.cpp  ../gen-cpp/scribe_types.cpp 
SOURCE_TEST=scribe_test.cpp ${GEN_SRC}
SOURCE_STDIN=scribe_stdin.cpp ${GEN_SRC}

all: clean init $(EXEC_TEST) $(EXEC_STDIN)

init:
	mkdir $(BUILD_DIR)

$(EXEC_TEST):$(SOURCE_TEST)
	$(CC) -g -DHAVE_NETINET_IN_H -o $(BUILD_DIR)/$@ $(INCLUDE)  $(SOURCE_TEST) $(LIBS)  
$(EXEC_STDIN):$(SOURCE_STDIN)
	$(CC) -g -DHAVE_NETINET_IN_H -o $(BUILD_DIR)/$@ $(INCLUDE)  $(SOURCE_STDIN) $(LIBS)  

#scribe_test: $(SOURCE)
#	$(CC) -g -DHAVE_NETINET_IN_H -o $@ $(INCLUDE)  $(SOURCE_TEST) $(LIBS)  
#	$(CC) -g -DHAVE_NETINET_IN_H -o $@ $(INCLUDE)  $(SOURCE_TEST) $(LIBS)  
#	$(CC) -g -DHAVE_NETINET_IN_H -o $@ $(INCLUDE)  $(SOURCE_STDIN) $(LIBS)  

clean:
	$(shell find . -name '*.o' -exec rm -f {} \;)
	rm $(BUILD_DIR) -rf
