build:
	g++ -I include/ src/Random.cpp src/PointOfInterest.cpp src/Sensor.cpp src/HarmonySearch.cpp main.cpp -o main

run: build
	./main $(ARGS)
