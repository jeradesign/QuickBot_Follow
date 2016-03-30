# Makefile for Lepton_Data_Sender 

.PHONY: run

# run: Lepton_Data_Sender
# 	./Lepton_Data_Sender $(LEPTON_VISUALIZER_IP)

run: QuickBot_Follow
	./QuickBot_Follow $(LEPTON_VISUALIZER_IP)

QuickBot_Follow: QuickBot_Follow.cpp Lepton_Driver.cpp Lepton_Driver.h Lepton_Frame.h Person_Finder.cpp OpenCV_Adapter.cpp OpenCV_Adapter.h Calibration.cpp Calibration.h
	g++ -std=c++0x -o QuickBot_Follow QuickBot_Follow.cpp Lepton_Driver.cpp Person_Finder.cpp OpenCV_Adapter.cpp Calibration.cpp -lopencv_core -lopencv_imgproc -lmraa

Lepton_Data_Sender: Lepton_Data_Sender.cpp Lepton_Driver.cpp Lepton_Driver.h
	g++ -o Lepton_Data_Sender Lepton_Data_Sender.cpp Lepton_Driver.cpp -L/usr/local/lib/arm-linux-gnueabihf/ -lmraa

clean:
	rm -rf Lepton_Data_Sender QuickBot_Follow
