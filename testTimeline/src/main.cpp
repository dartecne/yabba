/**
 * Curves demo
 * ofxTimeline
 *
 * Shows how to use ofxTimeline to create a simple curves and color change
 */

#include "testApp.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( ){

	/**/
	ofGLFWWindowSettings window;
	//#ifdef USE_PROGRAMMABLE_GL
	//	windowSettings.setGLVersion(4, 1);
	//#endif
	window.setSize(1024, 768);
	window.windowMode = OF_WINDOW;

	ofCreateWindow(window);

	ofRunApp(new testApp());
	/**/

}
