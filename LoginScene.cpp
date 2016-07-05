#include "LoginScene.h"


//c:/opencv/images/jang1.jpg
const std::string trainPath = "images/";
const std::string evalPath = "images/";

//HEADER
bool writeVocabulary(const string& filename, const Mat& vocabulary);


char ch[150];
//--------Using SURF as feature extractor and FlannBased for assigning a new point to the nearest one in the dictionarr
Ptr<DescriptorMatcher> matcher(new FlannBasedMatcher);
Ptr<DescriptorExtractor> extractor = new SurfDescriptorExtractor();
SurfFeatureDetector detector(500);

//---dictionary size=number of cluster's centroids
int dictionarySize = 10;
TermCriteria tc(CV_TERMCRIT_ITER, 10, 0.001);
int retries = 1;
int flags = KMEANS_PP_CENTERS;
BOWKMeansTrainer bowTrainer(dictionarySize, tc, retries, flags);
BOWImgDescriptorExtractor bowDE(extractor, matcher);

vector<KeyPoint> keypoint;
Mat features;



Mat groundTruth(0, 1, CV_32FC1);
Mat evalData(0, dictionarySize, CV_32FC1);

vector<KeyPoint> keypoint2;
Mat bowDescriptor2;

Mat results(0, 1, CV_32FC1);;

Mat labels(0, 1, CV_32FC1);
Mat trainingData(0, dictionarySize, CV_32FC1);
int k = 0;
vector<KeyPoint> keypoint1;
Mat bowDescriptor1;
CvSVM svm;

void LoginScene::onImageDownLoad(HttpClient* pender, HttpResponse* pResponse)
{
	HttpResponse* response = pResponse;

	if (!response)
	{
		cocos2d::log("No Response");
		return;
	}
	int statusCode = response->getResponseCode();

	char statusString[64] = {};
	sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, response->getHttpRequest()->getTag());
	cocos2d::log("response code: %d", statusCode);

	if (!response->isSucceed())
	{
		cocos2d::log("response failed");
		cocos2d::log("error buffer: %s", response->getErrorBuffer());
		return;
	}
	std::vector<char> *buffer = response->getResponseData();
	int dataSize = buffer->size();
	char *bdata = new char[dataSize];
	memset(bdata, 0, sizeof(dataSize));
	for (unsigned int i = 0; i < dataSize; i++){
		bdata[i] = (*buffer)[i];
	}
	cocos2d::Image * face = new cocos2d::Image();

	face->initWithImageData((unsigned char*)bdata, dataSize);

	// Save image file to device.
	//std::string writablePath = CCFileUtils::sharedFileUtils()->getWritablePath();
	//writablePath.append(response->getHttpRequest()->getTag());
	char path[50] = "images/";
	strcat(path, response->getHttpRequest()->getTag());
	face->saveToFile(path, true);
}

void LoginScene::thread1(int j )
{
	for (int i = 1; i <= NB_IMAGES; i++){

		sprintf(ch, "%d%d%s", j, i, ".jpg");
		//cout << ch << endl;

		HttpRequest* request = new HttpRequest();
		char url[100] = "http://joarastrawberry.com/face/";
		strcat(url, ch);
		request->setUrl(url);
		request->setRequestType(HttpRequest::Type::GET); request->setResponseCallback(this, httpresponse_selector(LoginScene::onImageDownLoad));
		request->setTag(ch);
		HttpClient::getInstance()->send(request); 
		request->release();
		
		char imageName[20];
		strcpy(imageName, trainPath.c_str());
		strcat(imageName, ch);

		img[i - 1][j - 1] = imread(imageName, 0);

		//cout << "rows " << img.rows << "cols " << img.cols << endl;

		detector.detect(img[i-1][j-1], keypoint);
		
		extractor->compute(img[i-1][j-1], keypoint, features);
		bowTrainer.add(features);
	}
}

void LoginScene :: collectclasscentroids() {
	
		
	return;
}


bool writeVocabulary(const string& filename, const Mat& vocabulary)
{
	cout << "Saving vocabulary..." << endl;
	FileStorage fs(filename, FileStorage::WRITE);
	if (fs.isOpened())
	{
		fs << "vocabulary" << vocabulary;
		return true;
	}
	return false;
}



Scene* LoginScene::createScene()
{
    auto scene = Scene::create();
    
    auto layer = LoginScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool LoginScene::init()
{
    if ( !LayerColor::initWithColor(Color4B(0,0,0,255)) )
    {
        return false;
    }
    
    //Size visibleSize = Director::getInstance()->getVisibleSize();
    //Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
   /* TextFieldTTF *textfield = TextFieldTTF::textFieldWithPlaceHolder("input id.", cocos2d::Size(480, 30), TextHAlignment::CENTER, "Arial", 20);
    textfield->setAnchorPoint(Vec2(0,0));
    textfield->setPosition(Vec2(0, 200));
    textfield->setTag(100);
    textfield->setColor(Color3B(255, 255, 255));
    */
    //this->addChild(textfield);
	auto back = Sprite::create("back.png");
	back->setAnchorPoint(Vec2(0, 0));
	this->addChild(back);

    this->scheduleUpdate();
	runTime = 0.0f;
	flag = true;
	auto text = LabelTTF::create("id", "fonts/arial.ttf", 25);
	text->setAnchorPoint(Vec2(0, 0));
	text->setPosition(Vec2(70, 250));
	addChild(text);

    Id = STTextField::create(" ", "fonts/arial.ttf", 20);
    Id->setPosition(Vec2(100,250));
    Id->setAnchorPoint(Vec2(0,0));
    Id->setMaxLength(15);
    Id->setMaxLengthEnabled(true);
    Id->setColor(Color3B(0,0,0));
    this->addChild(Id,2);
    
    
    auto barIdSprite = Sprite :: create("bar.png");
    barIdSprite->setAnchorPoint(Vec2(0,0));
    barIdSprite->setPosition(Vec2(100,250));
    barIdSprite->setTag(200);
    this->addChild(barIdSprite,1);
    
	/*
    Pw = STTextField::create(" ", "fonts/arial.ttf", 20);
    Pw->setPosition(Vec2(100,250));
    Pw->setAnchorPoint(Vec2(0,0));
    Pw->setMaxLength(15);
	Pw->setMaxLengthEnabled(true); 
	Pw->setPasswordStyleText("*");
	Pw->setPasswordEnabled(true);
	
    
    Pw->setColor(Color3B(0,0,0));
    this->addChild(Pw,2);
    
    
    auto barPwSprite = Sprite :: create("bar.png");
    barPwSprite->setAnchorPoint(Vec2(0,0));
    barPwSprite->setPosition(Vec2(100,250));
    barPwSprite->setTag(201);
    this->addChild(barPwSprite,1);*/
    
 
    
	auto loginBtn = MenuItemImage::create("l1.png", "l2.png", CC_CALLBACK_1(LoginScene::loginCheck, this));
	//loginBtn->setAnchorPoint(Vec2(1, 0));
	//loginBtn->setPosition(Vec2(800, 20));
	loginBtn->setTag(1);
	//this->addChild(loginBtn);



	auto faceBtn = MenuItemImage::create("f1.png", "f2.png", CC_CALLBACK_0(LoginScene::faceDetection, this));
	//faceBtn->setAnchorPoint(Vec2(1, 0));
	//faceBtn->setPosition(Vec2(800 - loginBtn->getContentSize().width - regiBtn->getContentSize().width - 30, 20));
	//this->addChild(faceBtn);

	auto menu = Menu::create(faceBtn, loginBtn, NULL);
	//menu->setAnchorPoint(Vec2(1, 0));
	menu->setPosition(Vec2(450, 50));
	menu->alignItemsHorizontally();

	this->addChild(menu);

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LoginScene::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	this->setTouchEnabled(true);
   
	/* images->initWithImageData(Buffer, nSize);

	auto *texture = new Texture2D();
	texture->initWithImage(images);
	if (texture) {
		auto Spr = Sprite::createWithTexture(texture);

		Spr->setAnchorPoint(Vec2(0, 0));
		this->addChild(Spr);
		RenderTexture* renderTexture = RenderTexture::create(Spr->getContentSize().width, Spr->getContentSize().height, Texture2D::PixelFormat::RGBA8888);
		
		renderTexture->retain();
		renderTexture->begin();
		Spr->visit();
		renderTexture->end();

		renderTexture->saveToFile("snapshot.png", Image::Format::PNG,true);


	}*/


	//auto threadScheduler = Director::getInstance()->getScheduler();
	//threadScheduler->performFunctionInCocosThread(CC_CALLBACK_0(LoginScene::runSVM,this));
	
	/*

	std::string strFullPath = FileUtils::getInstance()->getWritablePath() + "dd.png";
	
	ofstream os;
	
	os.open(strFullPath.c_str(),ios::binary);
	os <<data.getBytes();
	os.close();
	*/

	
	/*
	char* fileBinary = (char*)pBuffer;

    request->setUrl("http://joarastrawberry.com/index2.php");
	request->setRequestType(HttpRequest::Type::POSTFILE);
    request->setResponseCallback(this, httpresponse_selector(LoginScene::onHttpRequestCompleted));
    request->setTag("Post test");
	
		
		char postData[500] = "file=";
		strcat(postData, "ds");
		
		request->setRequestData(postData, strlen(postData));

	
    HttpClient::getInstance()->send(request);
    request->release();
    */
	
	auto scheduler = Director::getInstance()->getScheduler();

	 
	for (int j = 1; j <= NB_CLASSES; j++)
		scheduler->performFunctionInCocosThread(CC_CALLBACK_0(LoginScene::thread1, this,j));
    return true;
}
void LoginScene::loginCheck(Ref* sender)
{
	HttpRequest* request = new HttpRequest();

	request->setUrl("http://joarastrawberry.com/index.php");
	request->setRequestType(HttpRequest::Type::POST);
	request->setTag("Opencv Login");
	request->setResponseCallback(this, httpresponse_selector(LoginScene::onHttpRequestCompleted));
	char postData[500] = "Id=";
	strcat(postData, (Id->getString().c_str()));

	//strcat(postData, "&Pw=");
	//strcat(postData, (Pw->getString().c_str()));
	

	request->setRequestData(postData,strlen(postData));


	auto client = HttpClient::getInstance();
	client->enableCookies(NULL);
	client->send(request);
	request->release();
	/*
	ssize_t buff = 0;
	std::string imagePath = "HelloWorld.png";
	unsigned char* pBuffer = FileUtils::getInstance()->getFileData(imagePath.c_str(), "rb", &buff);
	auto images = new CCImage();
	Data data = FileUtils::getInstance()->getDataFromFile(imagePath);
	unsigned long nSize = data.getSize();
	unsigned char* Buffer = data.getBytes();
	request->setUrl("http://joarastrawberry.com/index2.php");
	request->setRequestType(HttpRequest::Type::POSTFILE);
	request->setResponseCallback(this, httpresponse_selector(LoginScene::onHttpRequestCompleted));
	request->setFilePath("HelloWorld.png");                  // 파일명을 전달하면 curl 에서 해당 파일을 읽어들여 전달
	request->setTag("Post test");
	request->setRequestData((char*)data.getBytes(), data.getSize());    // post form data 

	auto client = HttpClient::getInstance();
	client->enableCookies(NULL);
	client->send(request);
	request->release();
	*/

	

}
void LoginScene::nextScene()
{
	Scene* scene = MyScene::createScene(Id->getString());
	Director::getInstance()->replaceScene(scene);

}
int reSize1(){

	string face_cascade = "C:\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt.xml";

	CascadeClassifier face;

	Mat image(imread("img3.jpg"));//원본이미지 생성하고 읽기

	if (image.data == NULL)
	{
		cout << image << "이미지 열기 실패" << endl;
		return -1;
	}

	if (!face.load(face_cascade))
	{
		cout << "Cascade 파일 열기 실패" << endl;
		return -1;
	}


	/////////////////////////////////////////////////////////////////////////////////
	vector<cv::Rect>faces; //얼굴영역을 저장할 rect생성
	face.detectMultiScale(image, faces, 1.1, 3, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));//얼굴탐색


	int fsize = faces.size();


	//관심영역 설정위한 좌표
	//얼굴만 따기위한 좌표들
	int fx;//얼굴시작x좌표
	int fy;//얼굴시작y좌표
	int fw;//얼굴폭
	int fh;//얼굴높이



	int cx;//폭조절상수
	int cy;//높이조절상수
	//   cx = (faces[0].x)/10;
	//   cy = (faces[0].y)/10;

	cx = (faces[fsize - 1].width) / 10;   //사각형을 10개로 나눔(임의적으로)
	cy = (faces[fsize - 1].height) / 10;

	fx = (faces[0].x) + (1.1*cx); //9 5
	//   if(fx<1)
	//   {
	//      fx=1;
	//   }

	fy = (faces[0].y) + (2 * cy); //13 13
	//   if(fy<1)
	//   {
	//      fy=1;
	//   }

	fw = (faces[fsize - 1].width) - (2.33*cx); //16 10
	//   if((fw+fx)>inMat.cols)
	//   {
	//      fw=(inMat.cols)-fx;
	//   }
	//fh=(faces[fsize-1].height)+(25*cy);

	//   if(fw<0);
	//   {
	//      fw=1;
	//   }
	fh = fw;//폭과 높이 같게
	cv::Rect roi(fx, fy, fw, fh);//시작x좌표, 시작y좌표, 폭, 높이////

	cout << cx << " " << cy;

	Mat f_img = image(roi);

	Mat rs_img; //축소,확대 이미지 생성
	//다운리사이즈
	resize(f_img//원본이미지넣으세여
		, rs_img//리사이즈할 대상 이미지
		, cv::Size(64, 64)//사이즈는 이렇게 픽셀값정해도 되고, 원본이미지 행열 접근해서 축소,확대 가능해요
		//Size((image.cols)/2,(image.cols)/2)) 이렇게하면 비율축소,확대 가능한데
		//저희 프로젝트에서는 비율축소가아니라 고정축소,확대여서 일단 픽셀넣었어요
		, 0, 0 //뭔값인지 정확히 모르겠는데 0 , 0 이렇게 따로 두개 넣으면되네요
		, CV_INTER_LINEAR); //보간법 밑에 설명했습니다. 양선형 보간 사용함


	// 밝기 값만 따로 추출하기 위해
	Mat hsv;  //hsv

	cvtColor(rs_img, hsv, CV_BGR2HSV); //hsv컬러모델로 변환

	vector<Mat> channels; //hsv 채널생성
	split(hsv, channels); //hsv 채널분리

	//   equalizeHist(channels[0], channels[0]); //H채널
	//   equalizeHist(channels[1], channels[1]); //S채널
	equalizeHist(channels[2], channels[2]); //V채널   //평준화 시킨다,

	Mat result;
	merge(channels, hsv);   //hsv합치고

	cvtColor(hsv, result, CV_HSV2BGR);//RGB로 다시 변환한 것을 result에 저장

	//////////보간법//////////////
	/*
	0번   CV_INTER_NN        최근이웃보간법 제일후진겁니다.
	1번   CV_INTER_LINEAR    양선형보간법 쓸만합니다.
	2번   CV_INTER_CUBIC     확대할땐 이게 제일 좋은것같고
	3번   CV_INTER_AREA      축소할땐 이게 제일 좋은것같네요 무난하게 할려면 그냥 1번 양선형인듯..
	*/
	//////////////////////////////

	namedWindow("detect"); //얼굴검출영상출력
	imshow("detect", image);

	namedWindow("only face"); //얼굴부분편집영상 출력
	imshow("only face", f_img);

	namedWindow("resize image"); //리사이즈영상 출력
	imshow("resize image", rs_img);

	namedWindow("normalize image"); //노말라이즈영상 출력
	imshow("normalize image", result);

	imwrite("result.jpg", result); //노말라이즈 저장

	waitKey(0);

	return 0;
}

void LoginScene::faceDetection()
{
	// IplImage *frame;
	cv::Mat frame;
	CvCapture* capture = cvCaptureFromCAM(0);
	ifstream ifs;
	ofstream ofs;
	int num = 0;
	ifs.open("t.txt");
	ifs >> num;

	ofs.open("num.txt");


	int count = 0;
	int save_i = 0;
	if (capture)
	{
		cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, 300);
		cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, 300);
	}
	//string a = "webcam";
	// create a window
	cv::namedWindow("webcam", 1);
	int k = 2;
	// face detection configuration
	cv::CascadeClassifier face_classifier;
	//cv::CascadeClassifier eye_classifier;
	face_classifier.load("C:/opencv/sources/data/haarcascades/haarcascade_frontalface_default.xml");
	//eye_classifier.load( "C:/opencv/sources/data/haarcascades/haarcascades⁄haarcascade_eye.xml");


	while (cvWaitKey(10) != '32') {

		bool frame_valid = true;



		try {

			// get a new frame from webcam
			frame = cvQueryFrame(capture);


		}
		catch (cv::Exception& e) {

			std::cerr << "111Exception occurred. Ignoring frame... " << e.err << std::endl;
			frame_valid = false;

		}

		//  cvShowImage("Test", frame);

		if (cvWaitKey(33) >= 27)
			break;








		if (frame_valid) {
			try {
				// convert captured frame to gray scale & equalize
				cv::Mat grayframe;



				cv::cvtColor(frame, grayframe, CV_BGR2GRAY);
				cv::equalizeHist(grayframe, grayframe);

				// -------------------------------------------------------------
				// face detection routine

				// a vector array to store the face found
				std::vector<cv::Rect> faces;
				//      std::vector<cv::Rect> faces2;

				//CvSeq* faces;

				face_classifier.detectMultiScale(grayframe, faces,
					1.1, // increase search scale by 10% each pass
					3,   // merge groups of three detections
					CV_HAAR_FIND_BIGGEST_OBJECT | CV_HAAR_SCALE_IMAGE,
					cv::Size(30, 30)
					);

				/*eye_classifier.detectMultiScale(grayframe, faces2,
				1,
				3,
				CV_HAAR_FIND_BIGGEST_OBJECT|CV_HAAR_SCALE_IMAGE,
				cv::Size(10,10)
				);*/

				// -------------------------------------------------------------
				// draw the results
				int ch = waitKey(10);

				double scale = 1.3;
				//int count=0;
				//faces.size();
				for (int i = 0; i < faces.size(); i++) {
					cv::Point lb(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
					cv::Point tr(faces[i].x, faces[i].y);


					//CvPoint pt1, pt2;
					//pt1.x = (faces[i].x)*scale;
					//pt1.y = (faces[i].y)*scale;
					//cv::Point tr(pt1.x, pt1.y);lo
					//pt2.x = (faces[i].x + faces[i].width)*scale;
					//pt2.y = (faces[i].y + faces[i].height)*scale;
					//cv::Point lb(pt2.x, pt2.y);

					cv::rectangle(frame, lb, tr, cv::Scalar(0, 255, 0), 0.4, 4, 0);

					//cv::rectangle( frame, pt1, pt2, CV_RGB(255,0,0), 1, 8, 0 ); 

					imwrite("img3.jpg", frame);

					save_i = i;
					//////////////////////////////////////////////////

				}
				cv::imshow("webcam", frame);
				if (ch == 32){

					IplImage* img = cvLoadImage("img3.jpg", 1);
					cvSetImageROI(img, cvRect(faces[save_i].x, faces[save_i].y, faces[save_i].width, faces[save_i].height));
					cvSaveImage("img.jpg", img);
					k = reSize1();
					//IplImage* img = cvLoadImage("img3.jpg",1);

					///*cvSetImageROI( img, cvRect(faces[save_i].x, faces[save_i].y, faces[save_i].width , faces[save_i].height ));
					//char *name; 
					//sprintf(name, "%d", count);
					//char * h = ".jpg";
					//name[0] = count;
					//for(int i=1 ; i<5 ; i++){
					//   name[i] = h[i-1];
					//}
					//cvSaveImage(name, img);
					//++count;*/
					//cvSetImageROI( img, cvRect(faces[save_i].x, faces[i].y, faces[i].width , faces[i].height ));
					//cvSaveImage("img.jpg", img);
					//if(count >6){
					//waitKey(10);
					break;
					//}
				}

			}
			catch (cv::Exception& e) {
				std::cerr << "222Exception occurred. Ignoring frame... " << e.err << std::endl;
			}
		}

		//if ( cv::waitKey(30) >= 0 ) break;
	}



	cvDestroyWindow("Test");

	if (k == 0){
		cout << "성공" << endl;
		cvDestroyWindow("webcam");
		cvReleaseCapture(&capture);
		cvDestroyAllWindows();
		}
	else{
		cvDestroyWindow("webcam");
		cvReleaseCapture(&capture);
		cvDestroyAllWindows();
		cout << "실패" << endl;
	
	}

	vector<KeyPoint> keypoint5;
	Mat bowDescriptor5;
	Mat evalData1(0, dictionarySize, CV_32FC1);
	char* imageName1 = "Result.jpg";
	Mat img3 = imread(imageName1, 0);
	detector.detect(img3, keypoint5);//Detects keypoints in an image(first variant) or image set(second variant)
	bowDE.compute(img3, keypoint5, bowDescriptor5);   //bowDescriptor2
	evalData1.push_back(bowDescriptor5);
	float response = svm.predict(bowDescriptor5);
	cout << response << endl<<endl;
	switch ((int)response)
	{
	case 1:
		cout << "이영륜" << endl;
		break;
	case 2:
		cout << "김상현" << endl;
		break;
	case 3:
		cout << "김제언 " << endl;
		break;
	}
	myRes = response;
}
void LoginScene::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response) {
    if (!response) return;

    if (0 != strlen(response->getHttpRequest()->getTag())) {
        cocos2d::log("%s completed", response->getHttpRequest()->getTag());
    }
    
    long statusCode = response->getResponseCode();
    
    cocos2d::log("response code: %ld", statusCode);
    
    if (!response->isSucceed()) {
		cocos2d::log("response failed");
		cocos2d::log("error buffer: %s", response->getErrorBuffer());
        return;
    }
	
	std::vector<char> *buffer = response->getResponseData();
	
    char str[256] = {};
	
    for (unsigned int i = 0; i < buffer->size(); i++) {
        sprintf(str, "%s%c", str, (*buffer)[i]);
    }
    
	cocos2d::log("%s", str);
	
	if ( atoi(str) == myRes )
	{
		char str[100];
		strcpy(str, Id->getString().c_str());
		strcat(str, "님 환영 합니다!");
		MessageBox(str, "로그인 성공!!");
	}
	else{

		char str[100];
		strcpy(str, Id->getString().c_str());
		strcat(str, "님이 아니세요?");
		MessageBox(str, "로그인 실패!!");
	}
	
	/*
    cJSON *json = cJSON_Parse((const char *)str);
    
    cJSON *data_origin = cJSON_GetObjectItem(json, "origin");
    log("origin : %s", cJSON_Print(data_origin));
    
    cJSON *data_header = cJSON_GetObjectItem(json, "headers");
	log("header : %s", cJSON_Print(data_header));
    cJSON *data_host = cJSON_GetObjectItem(data_header, "Host");
    log("host : %s", cJSON_Print(data_host));
	*/
}


void LoginScene::update(float dt)
{
    
	runTime += dt;

	if (runTime > 0.5f && flag)
	{
		runSVM();
		flag = false;

	}
    //log("%s", Pw->getString().c_str());
    
}
/*
void LoginScene::keyboardWillShow(IMEKeyboardNotificationInfo &info)
{
    auto *textfield = (TextFieldTTF *)this->getChildByTag(100);
    textfield->setString("");
}

void LoginScene::keyboardWillHide(IMEKeyboardNotificationInfo &info)
{
    auto *textfield = (TextFieldTTF *)this->getChildByTag(100);
    auto *label = (Label *)this->getChildByTag(200);
    
    label->setString(textfield->getString());
}
*/
bool LoginScene::onTouchBegan(Touch* touch, Event* unused_event)
{
    
    Touch *pTouch = touch;
    Vec2 point = pTouch->getLocationInView();
    point = Director::getInstance()->convertToGL(point);
    
    auto idBar = this->getChildByTag(200);
    auto idRect = idBar->getBoundingBox();
    

    if(idRect.containsPoint(point))
    {
        Id->attachWithIME();
    }
    
    return true;
}
void LoginScene::runSVM()
{


	int height = 64;
	int width = 64;

	Mat image = Mat::zeros(height, width, CV_8UC3);
	int i, j;
	//Mat img[i-1][j-1];
	//cout << "Vector quantization..." << endl;
	
	
	collectclasscentroids(); //
	

	vector<Mat> descriptors = bowTrainer.getDescriptors();
	int count = 0;
	
	for (vector<Mat>::iterator iter = descriptors.begin(); iter != descriptors.end(); iter++)
	{
		count += iter->rows;
	}

	//cout << "Clustering " << count << " features" << endl;
	//choosing cluster's centroids as dictionary's words

	Mat dictionary = bowTrainer.cluster();

	//save da vocabulary
	writeVocabulary("vocabulary", dictionary);

	bowDE.setVocabulary(dictionary);
	//cout << "extracting histograms in the form of BOW for each image " << endl;
	//extracting histogram in the form of bow for each image
	for (j = 1; j <= NB_CLASSES; j++)
		for (i = 1; i <= NB_IMAGES; i++){


			//sprintf(ch, "%s%d%d%s", trainPath.c_str(), j, i, ".jpg");
			//const char* imageName = ch;
			//img[i-1][j-1] = imread(imageName, 0);

			detector.detect(img[i-1][j-1], keypoint1);
			bowDE.compute(img[i-1][j-1], keypoint1, bowDescriptor1);

			cout << "rows " << bowDescriptor1.rows << "cols " << bowDescriptor1.cols << endl;

			trainingData.push_back(bowDescriptor1);

			labels.push_back((float)j);
		}
	cout << "rows training dat" << trainingData.rows << "cols" << trainingData.cols << endl;


	//Setting up SVM parameters
	CvSVMParams params;
	params.kernel_type = CvSVM::RBF;
	params.svm_type = CvSVM::C_SVC;
	params.gamma = 0.50625000000000009;
	params.C = 312.50000000000000;
	params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 100, 0.000001);




	printf("%s\n", "Training SVM classifier");

	bool res = svm.train(trainingData, labels, cv::Mat(), cv::Mat(), params); //DEPRECIATED: use train_auto
	svm.save("Bow_svm_alpha");
	cout << "Processing evaluation data..." << endl;


	k = 0;

	for (j = 1; j <= NB_CLASSES; j++)
		for (i = 1; i <= NB_IMAGES; i++){


			//sprintf(ch, "%s%d%d%s", trainPath.c_str(), j, i, ".jpg");
			//const char* imageName = ch;
			//img[i-1][j-1] = imread(imageName, 0);

			detector.detect(img[i-1][j-1], keypoint2);//Detects keypoints in an image(first variant) or image set(second variant)
			bowDE.compute(img[i-1][j-1], keypoint2, bowDescriptor2);   //bowDescriptor2

			evalData.push_back(bowDescriptor2);
			groundTruth.push_back((float)j);
			float response = svm.predict(bowDescriptor2);

			cout << "respones :  " << response << endl;
			
			results.push_back(response);
		}




		

}
