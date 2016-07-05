//
//  LoginScene.h
//  opencvLogin
//
//  Created by jeunkim on 2016. 6. 6..
//
//

#ifndef LoginScene_h
#define LoginScene_h

#include "cocos2d.h"
#include "MyScene.h"
#include "STTextField.h"
#include "network/HttpClient.h"
#include "cJSON.h"
#include <fstream>

#include "opencv2/core/core.hpp"
#include "opencv2/ml/ml.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <vector>
#include <stdio.h>
#include <iostream>
#include "opencv/cv.h"


#define NB_IMAGES 9
#define NB_CLASSES 3
using namespace cv;
using namespace std;

using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using namespace cocos2d::network;
USING_NS_CC;

class LoginScene : public LayerColor, public IMEDelegate
{
public:
    static Scene* createScene();
    
    virtual bool init();
    
    CREATE_FUNC(LoginScene);
    
    //void keyboardWillShow(IMEKeyboardNotificationInfo &info);
    //void keyboardWillHide(IMEKeyboardNotificationInfo &info);
    //float getTextDimensions();
	void runSVM();
	void loginCheck(Ref * sender);
	void faceDetection();
	void nextScene();
	void onImageDownLoad(HttpClient* sender, HttpResponse* response);
	void thread1(int j);
	void collectclasscentroids();
    void update(float dt);
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event);
    void onHttpRequestCompleted(HttpClient *sender, HttpResponse *response);
    
private:
    STTextField* Id;
    
    STTextField* Pw;
	float runTime;
	bool flag;
	int myRes;
	Mat img[NB_IMAGES][NB_CLASSES];
};

#endif /* LoginScene_h */
