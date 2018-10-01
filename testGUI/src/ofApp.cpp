#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	font.load("DIN.otf", 30);

	bomba_1.addListener(this, &ofApp::bomba_1Changed );
	gui.setup("panel"); // most of the time you don't need a name
	gui.add(bomba_1.set("Bomba 1", 0, 0, 100));
	gui.add(screenSize.set("screenSize", ""));

	bHide = false;

}

//--------------------------------------------------------------
void ofApp::bomba_1Changed( int & val ) {


}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackgroundGradient(ofColor::white, ofColor::gray);

//	ofSetColor(0);
	string msg;
	msg += "GUI Test:\n";
	msg += "bomba 1: " + ofToString(bomba_1.get(), 3) + ")";
	font.drawString(msg, 50, 100);
	if (!bHide) {
		gui.draw();
	}

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
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

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	screenSize = ofToString(w) + "x" + ofToString(h);

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
