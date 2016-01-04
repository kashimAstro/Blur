#include "ofMain.h"
#include "ofxGui.h"

class Blur : public ofBaseApp {
	public:
		ofxPanel gui;
		ofParameter<bool> vertical;
		ofParameter<bool> horizontal;


		ofFbo fbo;
		ofFbo xfbo;

		ofShader shader;
		ofEasyCam cam;
		int w,h;

		void setup(){
        	    ofSetFrameRate(60);
                    w=ofGetScreenWidth();
                    h=ofGetScreenHeight();
	            ofDisableArbTex();
	            fbo.allocate(w,h);
	            xfbo.allocate(w,h);
		    shader.load("blur");
		    gui.setup();
		    gui.add(vertical.set("vertical",0,0,1));
                    gui.add(horizontal.set("horizontal",0,0,1));
		}

		void update() {
                    fbo.begin();
    	            ofEnableDepthTest();
			ofClear(0,0,0,255);
			cam.begin();
			int boxCount = 100;
			float spacing=0.8;
			float move = 0.4;
			int maxBoxSize = 100;
		        float cloudSize = ofGetWidth() / 2;

			for(int i = 0; i < boxCount; i++) {
		                ofPushMatrix();
		                float t = (ofGetElapsedTimef() + i * spacing) * move;
				ofVec3f pos(
				ofSignedNoise(t, 0, 0),
				ofSignedNoise(0, t, 0),
				ofSignedNoise(0, 0, t));

				float boxSize = maxBoxSize * ofNoise(pos.x, pos.y, pos.z);

				pos *= cloudSize;
				ofTranslate(pos);
				ofRotateX(pos.x);
				ofRotateY(pos.y);
				ofRotateZ(pos.z);

				ofSetColor(ofColor::fromHsb(sinf(t) * 128 + 128, 255, 255));
				ofDrawBox(boxSize * 1.1f);

				ofPopMatrix();
		        }

			cam.end();
                    ofDisableDepthTest();
                    fbo.end();
       		}

	        void draw() {
                    ofBackgroundGradient(255,0);
                    shader.begin();
		    shader.setUniformTexture("s_texture",fbo.getTexture(),1);
		    shader.setUniform1i("vertical",vertical);
		    shader.setUniform1i("horizontal",horizontal);


                      fbo.draw(0,0);
                    shader.end();
                    gui.draw();
        	}
};

int main(){
        ofSetupOpenGL(800, 600, OF_WINDOW);
        ofRunApp(new Blur());
}
