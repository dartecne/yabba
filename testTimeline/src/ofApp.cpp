/**
 * ofxTimeline -- AudioWaveform Example
 * openFrameworks graphical timeline addon
 *
 * Copyright (c) 2011-2012 James George http://www.jamesgeorge.org
 * Development Supported by YCAM InterLab http://interlab.ycam.jp/en/ +
 *
 * http://github.com/YCAMinterLab
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
    ofEnableSmoothing();
	
	glEnable(GL_DEPTH_TEST);
	ofEnableLighting();
	
	light.setPosition(ofGetWidth()*.5, ofGetHeight()*.25, 0);
	light.enable();
		
    
	ofxTimeline::removeCocoaMenusFromGlut( "TestTimeline" );
	timeline.setup();
    timeline.setFrameRate( frame_rate);
	timeline.setDurationInFrames( timeline_duration * frame_rate );
	timeline.setLoopType(OF_LOOP_NONE);
	timeline.addPage("pag1");
	timeline.addPage("pag2");
	timeline.addPage("pag3");
	
	timeline.setCurrentPage(0);
	timeline.addCurves( "curva1 pag1" );
	timeline.addCurves("curva2 pag1");
	timeline.addCurves( "test" );
	timeline.setCurrentPage(1);
	timeline.addCurves("curva1 pag2");
	timeline.addCurves("curva2 pag2");
	timeline.addCurves("test");

	for (int i = 0; i < NUM_BOMBAS; i++) {
		ofTrueTypeFont t;
		string n = "Bomba ";
		n += ofToString(i);

		t.load("DIN.otf", 12);
		bomba_value_label.push_back(t);
		bomba_name.push_back(n);
	}

	for (int i = 0; i < NUM_BOMBAS; i++ ) {
		timeline.addCurves(bomba_name[i], ofRange(0, 100));
	}
    timeline.setFrameBased(false);
	t.load("DIN.otf", 12);

//	timeline.setWidth( 600 );
//	timeline.setOffset(ofVec2f(100, 100));
	
	title.load("DIN.otf", 32);
	
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	string msg;

	ofBackground(.15*255);

	msg = "YABBA:\n";
	title.drawString(msg, 250, 20);

	for (int i = 0; i < NUM_BOMBAS; i++) {
		ofPoint p;
		p.y = timeline.getValue(bomba_name[i]) * ofGetHeight() / 100;
		p.x = i * 20;
		bomba_value_label[i].drawString(ofToString(timeline.getValue(bomba_name[i])), p.x, p.y);
	}

	t.drawString(ofToString(timeline.getValue("test")), ofGetWidth()/2, ofGetHeight()/2);

	timeline.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
//	timeline.collapseAllTracks();

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
	