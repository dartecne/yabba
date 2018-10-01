#pragma once

#include "ofMain.h"
#define OUTPUT_BUFFER_SIZE	6


class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);


	void  generate_command(int dimmer_id, char port, int level);
	void ofApp::readBytes(int);
	void  ofApp::readBytesUntil(int num_bytes);

	ofTrueTypeFont		font;

	char		bytesRead[OUTPUT_BUFFER_SIZE];				// data from serial, we will be trying to read 3
	char		output_buffer[OUTPUT_BUFFER_SIZE];			
	int			nBytesRead;					// how much did we read?
	int			nTimesRead;					// how many times did we read?
	float		readTime;					// when did we last read?				

	ofSerial	serial;

};
