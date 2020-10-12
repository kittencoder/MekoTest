
CXX:=g++
CXX_FLAGS:=-std=c++2a -g
CXX_CFLAGS:=$(CXX_FLAGS) -Wall
CXX_LFLAGS:=$(CXX_FLAGS) 

MEKO_SRC:=$(shell find Meko -name *.cpp)
EXAMPLES_SRC:=$(shell find Examples -name *.cpp)
MEKO_OBJS:=$(MEKO_SRC:.cpp=.o)
EXAMPLE_APPLICATIONS:=$(EXAMPLES_SRC:.cpp=.app)

INCLUDES:=-IMeko

MEKO_MAIN:=MekoTestSuite.a

all: $(MEKO_MAIN) $(EXAMPLE_APPLICATIONS)
	@echo Build successful! have fun

$(MEKO_MAIN): $(MEKO_OBJS)
	@echo "Assembling \e[95mMekoTestSuite\e[0m library"
	@ar -crs $(MEKO_MAIN) $(MEKO_OBJS)

%.app: %.cpp $(MEKO_MAIN)
	@echo "Building Example \e[95m$<\e[0m"
	@$(CXX) $(CXX_CFLAGS) -o $@ $< $(MEKO_MAIN) $(CXX_LFLAGS) $(INCLUDES)

%.o: %.cpp %.h
	@echo "Building MekoTestSuite part \e[95m$<\e[0m"
	@$(CXX) $(CXX_CFLAGS) -c $< -o $@

clean:
	@$(RM) $(MEKO_MAIN) $(MEKO_OBJS) $(EXAMPLE_APPLICATIONS)