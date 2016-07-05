#include "cocos2d.h"
USING_NS_CC;
using namespace std;

class MyScene : public LayerColor
{

public:
	static Scene* createScene(string Id);

	virtual bool init();

	CREATE_FUNC(MyScene);
	
	void update(float dt);


private:
	static string id;


};