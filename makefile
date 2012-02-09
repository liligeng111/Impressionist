#MakeFile:

FLTKFLAGS  =  `fltk-config --cflags --ldflags`

ImpSkelS12: ImpressionistDoc.o bitmap.o ImpBrush.o impressionist.o impressionistUI.o OriginalView.o PaintView.o PointBrush.o
	g++ -g -o  $(FLTKFLAGS) ImpSkelS12 ImpressionistDoc.o bitmap.o ImpBrush.o impressionist.o impressionistUI.o OriginalView.o PaintView.o PointBrush.o
ImpressionistDoc.o: ImpressionistDoc.cpp impressionistDoc.h impressionistUI.h impBrush.h pointBrush.h impressionist.h bitmap.h impressionist.h originalView.h paintView.h
	g++ -g -c  $(FLTKFLAGS) ImpressionistDoc.cpp
bitmap.o: bitmap.cpp bitmap.h
	g++ -g -c  $(FLTKFLAGS) bitmap.cpp
ImpBrush.o: ImpBrush.cpp impressionistDoc.h impressionistUI.h impBrush.h impressionist.h bitmap.h impressionist.h originalView.h paintView.h
	g++ -g -c  $(FLTKFLAGS) ImpBrush.cpp
impressionist.o: impressionist.cpp impressionistUI.h impressionistDoc.h impressionist.h originalView.h paintView.h impBrush.h impressionist.h bitmap.h
	g++ -g -c $(FLTKFLAGS)  impressionist.cpp
impressionistUI.o: impressionistUI.cpp impressionistUI.h impressionistDoc.h impressionist.h originalView.h paintView.h impBrush.h impressionist.h bitmap.h
	g++ -g -c  $(FLTKFLAGS) impressionistUI.cpp
OriginalView.o: OriginalView.cpp impressionist.h impressionistDoc.h originalView.h impBrush.h bitmap.h impBrush.h
	g++ -g -c  $(FLTKFLAGS) OriginalView.cpp
PaintView.o: PaintView.cpp impressionist.h impressionistDoc.h impressionistUI.h paintView.h impBrush.h bitmap.h originalView.h
	g++ -g -c  $(FLTKFLAGS) PaintView.cpp
PointBrush.o: PointBrush.cpp impressionistDoc.h impressionistUI.h pointBrush.h impressionist.h bitmap.h impressionist.h originalView.h paintView.h impBrush.h impBrush.h
	g++ -g -c $(FLTKFLAGS) PointBrush.cpp

clean:
	rm *.o
