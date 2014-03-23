CXX = g++

TARGET = mkapi
SRC = mkapi.cpp
OBJS = $(SRC:.cpp=.o) 
LLVM_CXXFLAGS=$(shell llvm-config --cxxflags)
LLVM_COMPONENTS=core mc mcparser support bitreader
LLVM_LIBS=$(shell llvm-config --libs $(LLVM_COMPONENTS))
LLVM_LDFLAGS=$(LLVM_LIBS) $(shell llvm-config --ldflags)
#why llvm-config --libs --ldflags xxx will link error?
#-lLLVMSupport -lLLVMMC 
CLANG_LDFLAGS = \
  -lclangFrontendTool -lclangFrontend -lclangDriver \
  -lclangSerialization -lclangCodeGen -lclangParse \
  -lclangSema -lclangStaticAnalyzerFrontend \
  -lclangStaticAnalyzerCheckers -lclangStaticAnalyzerCore \
  -lclangAnalysis -lclangARCMigrate -lclangRewriteCore \
  -lclangEdit -lclangAST -lclangLex -lclangBasic

CXXFLAGS = $(LLVM_CXXFLAGS)
LDFLAGS = $(CLANG_LDFLAGS) $(LLVM_LDFLAGS)

all: $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $^

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^  $(LDFLAGS) 

clean:
	rm -rf *.o *.ll $(TARGET)
