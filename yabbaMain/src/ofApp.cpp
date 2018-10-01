#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetVerticalSync(true);
	ofSetLogLevel(OF_LOG_VERBOSE);

	font.load("DIN.otf", 28);

	/* GUI */
	for (int i = 0; i < NUM_BOMBAS; i++) {
		bomba_value[i].addListener(this, &ofApp::bombaChanged);
	}
	gui.setup("panel"); // most of the time you don't need a name
	big_bombs_group.setName( "grandes" );

	for (int i = 0; i < NUM_BIG_BOMBAS; i++ ) {
//		gui.add(bomba_value[i].set(bomba_name[i], 0, 0, 100 ));
		big_bombs_group.add(bomba_value[i].set(bomba_name[i], 0, 0, 100));
	}
	gui.add(big_bombs_group);
	medium_bombs_group.setName( "medianas" );
	for (int i = NUM_BIG_BOMBAS; i < (NUM_BIG_BOMBAS + NUM_MEDIUM_BOMBAS); i++) {
		medium_bombs_group.add(bomba_value[i].set(bomba_name[i], 0, 0, 100));
	}
	gui.add(medium_bombs_group);
	small_bombs_group.setName("pequeñas");
	for (int i = (NUM_BIG_BOMBAS + NUM_MEDIUM_BOMBAS); i < NUM_BOMBAS; i++) {
		small_bombs_group.add(bomba_value[i].set(bomba_name[i], 0, 0, 100));
	}
	gui.add(small_bombs_group);

	gui.add(all_on.set("ALL ON", false));
	gui.add(all_off.set("ALL OFF ", false));
	gui.add(link.set("LINK ", true));
/* TODO 	gui.add(play); */
	gui.add(screenSize.set("screenSize", ""));
	/**/
	bHide = false;

	int baud = 9600;
	serial.setup("COM3", baud); //open the first device

	readTime = 0;
	std::cout << "setup OK\n";
	serial.writeBytes("ACK__", OUTPUT_BUFFER_SIZE);

	std::cout << "writen 'ACK00'\n"; // sends ACK so Arduino begins its loop

	ofxTimeline::removeCocoaMenusFromGlut("TestTimeline");
	timeline.setup();
	timeline.setFrameRate(frame_rate);
	timeline.setDurationInFrames(timeline_duration * frame_rate);
	timeline.setLoopType(OF_LOOP_NONE);

	for (int i = 0; i < NUM_BOMBAS; i++) {
		ofTrueTypeFont t;
		string n = "Bomba ";
		n += bomba_name[i];

		t.load("DIN.otf", 12);
		bomba_value_label.push_back(t);
		bomba_name[i] = n;
	}

	timeline.setPageName("grandes");
	timeline.addPage("medianos");
	timeline.addPage("pequenhos");

	// bombas grandes y potentes
	timeline.setCurrentPage(0);
	for (int i = 0; i < NUM_BIG_BOMBAS; i++) {
		timeline.addCurves(bomba_name[i], ofRange(0, 100));
	}

	// bombas medianas
	timeline.setCurrentPage(1);
	for (int i = NUM_BIG_BOMBAS; i < (NUM_BIG_BOMBAS + NUM_MEDIUM_BOMBAS); i++) {
		timeline.addCurves(bomba_name[i], ofRange(0, 100));
	}

	// bombas pequenhas
	timeline.setCurrentPage(2);
	for (int i = (NUM_BIG_BOMBAS + NUM_MEDIUM_BOMBAS); i < NUM_BOMBAS; i++) {
		timeline.addCurves(bomba_name[i], ofRange(0, 100));
	}
	timeline.setWidth( ofGetWidth() - offset );
	timeline.setOffset(ofVec2f(offset, 100));

	timeline.setFrameBased(false);

}

//--------------------------------------------------------------
void ofApp::bombaChanged(int & val) {


}

//--------------------------------------------------------------
void ofApp::update() {
	div_t str_div;
	int dimmer_id = 1; // ID could be 1, 2 or 3
	char port = 'B';
	for (int i = 0; i < NUM_BOMBAS; i++) {
		if( link.get() ) bomba_value[i].set( timeline.getValue( bomba_name[i] ));
		str_div = div(i , 8);
		dimmer_id = str_div.quot + 1;
		port = bomba_name[i][6]; // name is "Bomba X"
	//	cout << "port" << bomba_name[i][0] << "\n";
	//	cout << "bomba_name" << bomba_name[i] << "\n";
		if (bomba_value_old[i].get() != bomba_value[i].get()) {
			int v = calibrate( bomba_value[i].get() );
//			this->generate_command(dimmer_id, port, bomba_value[i].get());
			this->generate_command(dimmer_id, port, v);
			//sprintf( output_buffer, "%s", "1A050");
			cout << "sending: " << output_buffer << "...";
			serial.writeBytes(output_buffer, OUTPUT_BUFFER_SIZE);
		}
		bomba_value_old[i] = bomba_value[i];
	}
	readTime = ofGetElapsedTimef();
	
}
//--------------------------------------------------------------
int ofApp::calibrate(int v) {
	if (v < 10) return 0;
	else if (v < 40) return 40;
	else return v;
}
//--------------------------------------------------------------
void ofApp::draw() {
	ofBackgroundGradient(ofColor::gray, ofColor::black);

	//	ofSetColor(0);
	string msg;
	msg += "YABBA: ";
//	msg += "at time " + ofToString(readTime, 3) + "\n";
	font.drawString(msg, offset, 50);

	if (!bHide) {
		gui.draw();
	}

	timeline.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == 'h') {
		bHide = !bHide;
	}
	else if (key == 's') {
		gui.saveToFile("settings.xml");
	}
	else if (key == 'l') {
		gui.loadFromFile("settings.xml");
	}
}

void  ofApp::readBytesUntil(int num_bytes) {
	int i = -1;
	do {
		i++;
		if (i > num_bytes) {
			ofLog(OF_LOG_WARNING, "trying to exceed buffer size in reading");
			//			memset(bytesRead, 0, OUTPUT_BUFFER_SIZE);
			bytesRead[OUTPUT_BUFFER_SIZE - 1] = '\0';
			return;
		}
		bytesRead[i] = serial.readByte();
	} while ((bytesRead[i] != '\r') | (bytesRead[i] != '\n') | (bytesRead[i] != '\0'));
}

//--------------------------------------------------------------
void  ofApp::wait_echo() {

	cout << "waiting echo...\n";
	while (serial.available() != OUTPUT_BUFFER_SIZE);

	std::cout << "found:  " << serial.available() << "  bytes waiting\n";

	//	this->readBytes(OUTPUT_BUFFER_SIZE);
	this->readBytesUntil(OUTPUT_BUFFER_SIZE - 1);

	std::cout << "echo: " << bytesRead << "\n";
}

//--------------------------------------------------------------
void  ofApp::generate_command(int dimmer_id, char port, int level) {
	output_buffer[0] = dimmer_id + '0';
	output_buffer[1] = port;
	char tmp[3];

	if (level < 100) {
		output_buffer[2] = '0';
		if (level >= 10) {
			sprintf(tmp, "%d", level);
			output_buffer[3] = tmp[0];
			output_buffer[4] = tmp[1];
		}
		else {
			output_buffer[3] = '0';
			output_buffer[4] = level + '0';
		}
	}
	else {
		output_buffer[2] = '1';
		output_buffer[3] = '0';
		output_buffer[4] = '0';
	}
	output_buffer[5] = '\0';
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	screenSize = ofToString(w) + "x" + ofToString(h);
	timeline.setWidth( w - offset );
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
