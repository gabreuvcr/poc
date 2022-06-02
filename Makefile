build:
	g++ -I include/ src/PointOfInterest.cpp src/Sensor.cpp src/HarmonySearch.cpp main.cpp -o main

run: build
	./main $(ARGS)
