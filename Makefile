%.o : %.cpp
	g++ -c $< -o $@

%.out : %.o
	g++  $< -lsfml-graphics -lsfml-window -lsfml-system -o $@