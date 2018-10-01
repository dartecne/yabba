#include "ofApp.h"

#define ACK	1

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetVerticalSync(true);

	ofBackground(255);
	ofSetLogLevel(OF_LOG_VERBOSE);

	font.load("DIN.otf", 30);

	serial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
	int baud = 9600;
	serial.setup("COM3", baud); //open the first device

	nTimesRead = 0;
	readTime = 0;
	std::cout << "setup OK\n";
	serial.writeBytes("ACK__", OUTPUT_BUFFER_SIZE);

	std::cout << "writen 'ACK__'\n"; // sends ACK so Arduino begins its loop
}

//--------------------------------------------------------------
void ofApp::update() {
	nTimesRead = 0;
	nBytesRead = 0;
	int nRead = 0;  // a temp variable to keep count per read

	int dimmer_id = 1;
//	char port = 'A';
	char port = 'B';
	int level = rand() % 100 + 1;

	this->generate_command( dimmer_id, port, level);
//	sprintf( output_buffer, "%s", "1A050");
	cout << "sending: ";// << output_buffer << "...";

	serial.writeBytes(output_buffer, OUTPUT_BUFFER_SIZE);
//	serial.writeBytes("1A100", OUTPUT_BUFFER_SIZE);
	cout << "waiting echo...\n";
	while (serial.available() != OUTPUT_BUFFER_SIZE);

	std::cout << "found:  " << serial.available() << "  bytes waiting\n"; 
	
//	this->readBytes(OUTPUT_BUFFER_SIZE);
	this->readBytesUntil(OUTPUT_BUFFER_SIZE-1);

	std::cout << "echo: " << bytesRead << "\n";

	readTime = ofGetElapsedTimef();
}


//--------------------------------------------------------------
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
void ofApp::readBytes(int bytesRequired) {
	int bytesRemaining = bytesRequired;
	// loop until we've read everything
	while (bytesRemaining > 0) {
		// check for data
		if (serial.available() > 0) {
			// try to read - note offset into the bytes[] array, this is so
			// that we don't overwrite the bytes we already have
			int bytesArrayOffset = bytesRequired - bytesRemaining;
			int result = serial.readBytes(&bytesRead[bytesArrayOffset],
				bytesRemaining);

			// check for error code
			if (result == OF_SERIAL_ERROR) {
				// something bad happened
				ofLog(OF_LOG_ERROR, "unrecoverable error reading from serial");
				// bail out
				break;
			}
			else if (result == OF_SERIAL_NO_DATA) {
				// nothing was read, try again
			}
			else {
				// we read some data!
				bytesRemaining -= result;
			}
		}
		else {
			ofLog(OF_LOG_WARNING, "no data in serial port for reading!");
			return;
		}
	}
}
//--------------------------------------------------------------
void ofApp::draw() {
	/*	if (nBytesRead > 0 && ((ofGetElapsedTimef() - readTime) < 0.5f)) {
	ofSetColor(0);
	}
	else {
	ofSetColor(220);
	}
	*/
	ofSetColor(0);
	string msg;
	msg += "Serial Test:\n";
	msg += "nBytes read " + ofToString(nBytesRead) + "\n";
	msg += "nTimes read " + ofToString(nTimesRead) + "\n";
	msg += "read: " + ofToString(bytesRead) + "\n";
	msg += "(at time " + ofToString(readTime, 3) + ")";
	font.drawString(msg, 50, 100);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

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

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

