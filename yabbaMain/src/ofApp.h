#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxTimeline.h"

#define NUM_BOMBAS	20		// number of bombs to control. divided in big, medium and small ones
#define NUM_BIG_BOMBAS	4	
#define NUM_MEDIUM_BOMBAS	9
#define NUM_SMALL_BOMBAS	7

#define OUTPUT_BUFFER_SIZE	6



class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	int calibrate(int);
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

	void generate_command(int dimmer_id, char port, int level);
	void ofApp::readBytes(int);
	void ofApp::readBytesUntil(int num_bytes);
	void ofApp::wait_echo();
	void ofApp::bombaChanged(int & val);

	ofTrueTypeFont	font;

	bool bHide;
	std::string bomba_name[NUM_BOMBAS] = { "A", "B", "C", "D", "E", "F", "G","H",
											"A", "B", "C", "D", "E", "F", "G","H",
											"A", "B", "C", "D" };

//											"I", "J", "K", "L", "M", "N", "O", "P",
//											"K", "R", "S", "T"};

	ofParameter<string> screenSize;
	ofxLabel info;
	ofParameterGroup big_bombs_group;
	ofParameterGroup medium_bombs_group;
	ofParameterGroup small_bombs_group;
	ofParameter<int> bomba_value[NUM_BOMBAS];
	ofParameter<int> bomba_value_old[NUM_BOMBAS];
	ofParameter<bool> all_on;
	ofParameter<bool> all_off;
	ofParameter<bool> link; // links gui params with timeline values
	ofxButton play;

	ofxPanel gui;
	int offset = 250;


	char		bytesRead[OUTPUT_BUFFER_SIZE];				// data from serial, we will be trying to read 3
	char		output_buffer[OUTPUT_BUFFER_SIZE];
	float		readTime;					// when did we last read?				

	ofSerial	serial;

	ofxTimeline timeline;

	int frame_rate = 2; // frames per sec
	int timeline_duration = 11 * 60 * 60; // 11 hours in secs

	ofTrueTypeFont title;
	ofTrueTypeFont t;

	vector<ofTrueTypeFont> bomba_value_label;

};
