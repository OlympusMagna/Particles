CXX = g++
CXXFLAGS = -Wall -std=c++11 -I/usr/local/include

#  Executable name
EXEC = my_program  #  Change this to your executable's name

#  Source files
SRCS = main.cpp Particle.cpp Matrices.cpp Engine.cpp
OBJS = $(SRCS:.cpp=.o)  #  Automatically create list of object files

#  SFML libraries (adjust as needed for your system)
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system

#  Default rule (builds the executable)
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(OBJS) $(SFML_LIBS)

#  Run the executable
run: $(EXEC)
	./$(EXEC)

#  Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

#  Clean rule (removes object files and the executable)
clean:
	rm -f $(OBJS) $(EXEC)
