#include "HelloWorldScene.h"
#include "iostream"
#include "sstream"

using namespace cocos2d;
using namespace std;

CCScene* HelloWorld::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        HelloWorld *layer = HelloWorld::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////

        CC_BREAK_IF(! CCLayerColor::initWithColor(ccc4(255,255,255,255)));

        //////////////////////////////////////////////////////////////////////////
        // add your codes below...
        //////////////////////////////////////////////////////////////////////////

        // 1. Add a menu item with "X" image, which is clicked to quit the program.

        // Create a "close" menu item with close icon, it's an auto release object.
/*        
		CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
            "CloseNormal.png",
            "CloseSelected.png",
            this,
            menu_selector(HelloWorld::menuCloseCallback));
        CC_BREAK_IF(! pCloseItem);

        // Place the menu item bottom-right conner.
        pCloseItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20));

        // Create a menu with the "close" menu item, it's an auto release object.
        CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
        pMenu->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenu);

        // Add the menu to HelloWorld layer as a child layer.
        this->addChild(pMenu, 1);
*/
        // 2. Add a label shows "Hello World".

        // Create a label and initialize with string "Hello World".
        CCLabelTTF* pLabel = CCLabelTTF::create("The New Lines", "Arial", 24);
        CC_BREAK_IF(! pLabel);

        // Get window size and place the label upper. 
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        pLabel->setPosition(ccp(size.width / 2, size.height - 50));

        // Add the label to HelloWorld layer as a child layer.
        this->addChild(pLabel, 1);
		
		totalScore = 0;

		CCLabelTTF * scoreLabel = CCLabelTTF::create("Score","TimeNewsRoman", 20);
		CC_BREAK_IF(!scoreLabel);
		scoreLabel->setPosition(ccp(30, size.height - 100));
		this->addChild(scoreLabel,1);


		firstTouch = false;
		gotScore = false;

		keyBackTouch = false;

		keyBack = CCSprite::create("back.png");
		keyBack->setPosition(ccp(size.width-100, size.height-100));
		addChild(keyBack);



        for (int i = 0; i < 7; i++){
			for (int j = 0; j < 7; j++){

				tiles[i][j] = CCSprite::create("white.png");
				addChild(tiles[i][j]);
				tiles[i][j]->setPosition(ccp(44*i + 1.5*(i+1) + 22, 44*j + 1.5*(j+1) + 22));

				a[i][j] = 0;
			
			}
        }

		generateLines(3);
		
		scheduleUpdate();
		this->setTouchEnabled(true);

        bRet = true;
    } while (0);

    return bRet;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}

void HelloWorld::ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent){
	
	CCTouch* touch = (CCTouch*)(pTouches->anyObject());
	CCPoint location = touch->getLocationInView();
	location = CCDirector::sharedDirector()->convertToGL(location);	

	int x = -1 , y = -1;
	for (int i = 0; i<7; i++)
		for (int j = 0; j<7; j++)
			if (tiles[i][j]->boundingBox().containsPoint(location)){
				x = i;
				y = j;
				break;
			}


	// inspect the key back

	if (keyBack->boundingBox().containsPoint(location)){
		setKeyBack();
		return;
	}

	if ((x == -1) || ( y == -1)) return;

	if (a[x][y] == 0){
		if (!firstTouch) return;	
	}
	else {
		x1 = x; y1 = y;
		firstTouch = true;
		return;
	}

	x2 = x; y2 = y;
	bfs(x1, y1, x2, y2);
	if (haveWay){	

		for (int i = 0; i<7; i++)
			for(int j = 0; j < 7; j++) aBack[i][j] = a[i][j];

		char* color[6] = {"white","blue","yellow","red","green","pink"};
		char name[20];
		sprintf (name, "%s.png", color[a[x1][y1]]);
		this->removeChild(tiles[x2][y2],true);
		tiles[x2][y2] = CCSprite::create(name);
		tiles[x2][y2]->setPosition(ccp(44*x2 + 1.5*(x2+1) + 22, 44*y2 + 1.5*(y2+1) + 22));
		a[x2][y2] = a[x1][y1];
		addChild(tiles[x2][y2]);

		this->removeChild(tiles[x1][y1],true);
		tiles[x1][y1] = CCSprite::create("white.png");
		tiles[x1][y1]->setPosition(ccp(44*x1 + 1.5*(x1+1) + 22, 44*y1 + 1.5*(y1+1) + 22));
		a[x1][y1] = 0;
		addChild(tiles[x1][y1]);

		firstTouch = false;

		gotScore = false;
		checkResult();
		if (!gotScore) generateLines(3);


		CCLOG("%i %i %i %i %i %i %i\n",a[0][6], a[1][6],a[2][6],a[3][6],a[4][6],a[5][6], a[6][6]);
		CCLOG("%i %i %i %i %i %i %i\n",a[0][5], a[1][5],a[2][5],a[3][5],a[4][5],a[5][5], a[6][5]);
		CCLOG("%i %i %i %i %i %i %i\n",a[0][4], a[1][4],a[2][4],a[3][4],a[4][4],a[5][4], a[6][4]);
		CCLOG("%i %i %i %i %i %i %i\n",a[0][3], a[1][3],a[2][3],a[3][3],a[4][3],a[5][3], a[6][3]);
		CCLOG("%i %i %i %i %i %i %i\n",a[0][2], a[1][2],a[2][2],a[3][2],a[4][2],a[5][2], a[6][2]);
		CCLOG("%i %i %i %i %i %i %i\n",a[0][1], a[1][1],a[2][1],a[3][1],a[4][1],a[5][1], a[6][1]);
		CCLOG("%i %i %i %i %i %i %i\n",a[0][0], a[1][0],a[2][0],a[3][0],a[4][0],a[5][0], a[6][0]);
		CCLOG("-----");



	}
}

void HelloWorld::bfs(int startx, int starty, int finishx, int finishy){
	
	struct coordinate{
		int x; 
		int y;
	};

	int tx[4] = {-1,0,1,0};
	int ty[4] = {0,1,0,-1};

	coordinate q[50];
	bool fre[7][7];

	int i, j, u, v, first, last;

	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 7; j++) fre[i][j] = true;
	fre[startx][starty] = false;

	first = 1; last = 1;
	q[1].x = startx;
	q[1].y = starty;

	haveWay = false;

	while (first <= last){
		i = q[first].x;
		j = q[first].y;
		first++;
		for (int k = 0; k < 4; k++){
			u = i + tx[k];
			v = j + ty[k];
			if ((0<=u) && (u<7) && (0<=v) && (v<7))
				if (fre[u][v] && (a[u][v] == 0)){
					last++;
					q[last].x = u;
					q[last].y = v;
					fre[u][v] = false;
					if ((u == finishx) && (v == finishy)){
						haveWay = true;
						return;
					}
				}
		}
	}

}

void HelloWorld::generateLines(int numberOfPoint){

	char* color[6] = {"white","blue","yellow","red","green","pink"};

	int numberOfColor = 5;
	int i, j;

	char name[20];

	for (int k = 0; k < numberOfPoint; k++){
		do{
			i = rand() % 7;
			j = rand() % 7;
		} while (a[i][j] != 0);

		int actual = rand() % 5 + 1;
		sprintf (name, "%s.png", color[actual]);

		this->removeChild(tiles[i][j]);
		tiles[i][j] = CCSprite::create(name);
		tiles[i][j]->setPosition(ccp(44*i + 1.5*(i+1) + 22, 44*j + 1.5*(j+1) + 22));
		addChild(tiles[i][j]);
		a[i][j] = actual;
	}

}

void HelloWorld::checkResult(){
	int u, v, countRow, countColumn, countDiagonalRight, countDiagonalLeft, colorNumber;

	int countOne = 0, countTwo = 0, countThree = 0, countFour = 0;;

	int remember[7][7];


	for (int i = 0; i<7; i++)
		for(int j = 0; j < 7; j++) remember[i][j] = 0;

	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 7; j++)
			if (a[i][j] != 0){

				countColumn = 1;
				countRow = 1;
				countDiagonalRight = 1;
				countDiagonalLeft = 1;

				colorNumber = a[i][j];

				// column
				u = i; v = j + 1;
				while ( v < 7){
					if (a[u][v] != colorNumber) break;
					else{
						v++;
						countColumn++;
					}
				}

				if (countColumn >=4){
					v--;
					while ( v >= j ){
						remember[u][v]++;
						v--;
					}
					countOne++;
				}

				// row
				u = i + 1; v = j;
				while ( u < 7){
					if (a[u][v] != colorNumber) break;
					else{
						u++;
						countRow++;
					}
				}

				if (countRow >=4){
					u--;
					while ( u >= i ){
						remember[u][v]++;
						u--;
					}
					countOne++;
				}

				//diagonal right
				u = i + 1; v = j + 1;
				while ((u<7) && (v<7)){
					if (a[u][v] != colorNumber) break;
					else{
						u++; 
						v++;
						countDiagonalRight++;
					}
				}

				if (countDiagonalRight >=4){
					u--; v--;
					while ( (u >= i) && (v >= j) ){
						remember[u][v]++;
						u--;
						v--;
					}
					countOne++;
				}

				//diagonal left
				u = i - 1; v = j + 1;
				while ((u>=0) && (v<7)){
					if (a[u][v] != colorNumber) break;
					else{
						u--; 
						v++;
						countDiagonalLeft++;
					}
				}

				if (countDiagonalLeft >=4){
					u++; v--;
					while ( (u <= i) && (v >= j) ){
						remember[u][v]++;
						u++;
						v--;
					}
					countOne++;
				}

			}

	for (int i = 0; i<7; i++)
		for (int j = 0; j<7; j++){
			if (remember[i][j] == 2) countTwo++;
			if (remember[i][j] == 3) countThree++;
			if (remember[i][j] == 4) countFour;

			if (remember[i][j] > 0){

				gotScore = true;

				this->removeChild(tiles[i][j]);
				tiles[i][j] = CCSprite::create("white.png");
				tiles[i][j]->setPosition(ccp(44*i + 1.5*(i+1) + 22, 44*j + 1.5*(j+1) + 22));
				addChild(tiles[i][j]);

				a[i][j] = 0;
			}
		}

	totalScore= totalScore + (countOne - 2*countTwo - 3*countThree - 4*countFour) + 4*countTwo + 6*countThree + 8* countFour;
}

void HelloWorld::update(float pDt){

}

void HelloWorld::setKeyBack(){

	char* color[6] = {"white","blue","yellow","red","green","pink"};

	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 7; j++)
			if (a[i][j] != aBack[i][j]){

				a[i][j] = aBack[i][j];

				char name[20];
				sprintf (name, "%s.png", color[a[i][j]]);
				
				this->removeChild(tiles[i][j]);
				tiles[i][j] = CCSprite::create(name);
				tiles[i][j]->setPosition(ccp(44*i + 1.5*(i+1) + 22, 44*j + 1.5*(j+1) + 22));
				addChild(tiles[i][j]);

			}
}

void HelloWorld::setKeyMove(){

}