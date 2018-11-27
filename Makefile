CXX      = g++ -std=c++11
SRCS     = $(wildcard *.cpp)
OBJS     = $(SRCS:.cpp=.o)
TARGET   = runchain

all : $(TARGET)
	$(CXX) -o $(TARGET)  $(OBJS) 
$(TARGET) :
	$(CXX) -c $(SRCS)
	
clean :
	rm -f $(TARGET)
	rm -f *.o
