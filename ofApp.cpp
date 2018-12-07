#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetColor(39);
	ofFill();

	this->radius = 25;

	this->box2d.init();
	this->box2d.setGravity(0, 0);
	this->box2d.createBounds();
	this->box2d.setFPS(60);
	this->box2d.registerGrabbing();

	for (int i = 0; i < 50; i++) {

		auto circle = make_shared<ofxBox2dCircle>();
		circle->setPhysics(0.5, 0.63, 0.1);
		circle->setup(this->box2d.getWorld(), ofRandom(0, ofGetWidth()), ofRandom(0, ofGetHeight()), this->radius);
		this->circles.push_back(circle);
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	for (int i = 0; i < this->circles.size(); i++) {

		for (int j = i + 1; j < this->circles.size(); j++) {

			float distance = this->circles[i]->getPosition().distance(this->circles[j]->getPosition());
			if (distance < this->radius * 4) {

				this->circles[i]->addForce(this->circles[i]->getPosition() - this->circles[j]->getPosition(), ofMap(distance, this->radius, this->radius * 4, 0.5, 0.002));
				this->circles[j]->addForce(this->circles[j]->getPosition() - this->circles[i]->getPosition(), ofMap(distance, this->radius, this->radius * 4, 0.5, 0.002));
			}
		}
	}

	this->box2d.update();
}

//--------------------------------------------------------------
void ofApp::draw() {

	for (int i = 0; i < this->circles.size(); i++) {

		ofDrawCircle(this->circles[i]->getPosition(), this->radius * 0.2);

		for (int j = 0; j < this->circles.size(); j++) {

			if (i == j) {

				continue;
			}

			glm::vec2 my = glm::vec2(this->circles[i]->getPosition().x, this->circles[i]->getPosition().y);
			glm::vec2 you = glm::vec2(this->circles[j]->getPosition().x, this->circles[j]->getPosition().y);
			glm::vec2 distance = my - you;

			if (glm::length(distance) < this->radius * 4) {

				float my_rad = std::atan2(you.y - my.y, you.x - my.x);
				float you_rad = std::atan2(my.y - you.y, my.x - you.x);

				glm::vec2 my_point = my + glm::vec2(radius * cos(my_rad), radius * sin(my_rad));
				glm::vec2 you_point = you + glm::vec2(radius * cos(you_rad), radius * sin(you_rad));
				ofDrawLine(my_point, you_point);

				float rad_span = PI / ofMap(glm::length(distance), this->radius * 2, this->radius * 4, 1, 10);
				float rad_step = rad_span / 10;

				ofBeginShape();
				for (float r = you_rad - rad_span; r <= you_rad + rad_span; r += rad_step) {

					ofVertex(you + glm::vec2(radius * cos(r), radius * sin(r)));
				}
				for (float r = you_rad + rad_span; r >= you_rad - rad_span; r -= rad_step) {

					ofVertex(you + glm::vec2(radius * 0.5 * cos(r), radius * 0.5 * sin(r)));
				}
				ofEndShape(true);
			}
		}
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}