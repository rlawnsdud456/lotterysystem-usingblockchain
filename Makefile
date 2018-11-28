CXX      = g++ -std=c++11
SRCS     = $(wildcard *.cpp)
OBJS     = $(SRCS:.cpp=.o)
LIBS	 = -lpthread
TARGET   = runchain

all : $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET)  $(OBJS) $(LIBS)
	mkdir log
%.o : %.cpp
	$(CXX) -c $< -o $@
	
clean :
	rm -f $(TARGET)
	rm -f *.o
	rm -r log

demo:
	./runchain 2
