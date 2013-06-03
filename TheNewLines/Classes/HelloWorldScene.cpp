#include "HelloWorldScene.h"
#include "sstream"

using namespace cocos2d;

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
	animationControl = 0;
	duration = 0.25;
	multiPlayer = false;
	bonusPoint = false;
	mutexMapPlay = NULL;
	mutexMapView = NULL;
	friendScore = 0;
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
        CCLabelTTF* pLabel = CCLabelTTF::create("The New Lines", "Arial", 20);
		pLabel->setColor(ccc3(1,0,1));
        CC_BREAK_IF(! pLabel);

        // Get window size and place the label upper. 
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        pLabel->setPosition(ccp(size.width - 80, size.height - 20));

        // Add the label to HelloWorld layer as a child layer.
        this->addChild(pLabel, 3);

		mLoadingBarBG = CCSprite::create("loading.png");
		mLoadingBar = CCProgressTimer::create(mLoadingBarBG);
		mLoadingBar->setPercentage(0.0f);
		mLoadingBar->setType(kCCProgressTimerTypeBar);
		mLoadingBar->setBarChangeRate(ccp (1, 0 ));
		mLoadingBar->setMidpoint(ccp (-5,0));
		mLoadingBar->setPosition(ccp (size.width -80 , size.height - 20));
		this->addChild(mLoadingBar);


		totalScore = 0;
		oldTotalScore = 0;
		level = 1;
		countNumber = 0;
		limitLine = 40;

		
		showTotalScore();
		
		showCountNumber();

		firstTouch = false;
		gotScore = false;

		keyBackTouch = false;
		keyMoveTouch = false;

		countKeyMove = 2;
		countKeyBack = 3;
		
		//key move option
		showKeyMove();

		//key back option
		showKeyBack();

		usedOneTime = true;


		initTilesHint();

		x1 = 0; y1 = 0;

        for (int i = 0; i < 7; i++){
			for (int j = 0; j < 7; j++){

				tiles[i][j] = CCSprite::create("white.png");
				tiles[i][j]->setPosition(ccp(44*i + 1.5*(i+1) + 22, 44*j + 1.5*(j+1) + 22));
				addChild(tiles[i][j], 1);

				a[i][j] = 0;
			
			}
        }

		generatePrediction(level+2);
		generateLines(level+2);
		generatePrediction(level+2);
		
		//scheduleUpdate();
		this->setTouchEnabled(true);

        bRet = true;
    } while (0);

    return bRet;
}

void HelloWorld::initTilesHint(){

	CCSize size = CCDirector::sharedDirector()->getWinSize();

	int k = 0;

	for (int i = 0; i<2; i++){
		int w = size.width - 79.5;
		int h = size.height - 100 - i*31.5;

		for (int j = 0; j<3; j++){
			tilesHintPos[k].x = w;
			tilesHintPos[k].y = h;

			tilesHint[i][j] = CCSprite::create("whiteHint.png");
			tilesHint[i][j]->setPosition(ccp(tilesHintPos[k].x, tilesHintPos[k].y));
			addChild(tilesHint[i][j]);

			w += 30 + 1.5;
			k++;
		}
	}

}

void HelloWorld::ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent){
	touchesAction(pTouches, pEvent);
	if(mutexMapPlay != NULL && mutexMapPlay->try_lock() == false)
		mutexMapPlay->unlock();
//	pthread_mutex_unlock(mutexMapPlay);
	if(checkEndGame())
		endGame();
	
}

void HelloWorld::touchesAction(CCSet* pTouches, CCEvent* pEvent){
	CCSize size = CCDirector::sharedDirector()->getWinSize();
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
		
		if ((countKeyBack == 0) || (usedOneTime)) return;

		//this->removeChild(keyBack,true);
		//keyBack = CCSprite::create("backBold.png");
		//keyBack->setPosition(ccp(size.width-60, size.height-60));
		//addChild(keyBack);
		
		setKeyBack();

		//this->removeChild(keyBack,true);
		//keyBack = CCSprite::create("back.png");
		//keyBack->setPosition(ccp(size.width-60, size.height-60));
		//addChild(keyBack);

		return;
	}

	// inspect the key move

	if (keyMove->boundingBox().containsPoint(location)){

		if (countKeyMove == 0) return;

		keyMoveTouch = !keyMoveTouch;

		this->removeChild(keyMove,true);

		if (keyMoveTouch) keyMove = CCSprite::create("moveBold.png");
		else keyMove = CCSprite::create("move.png");
		keyMove->setPosition(ccp(size.width-25, size.height-60));
		addChild(keyMove);

	}

	if ((x == -1) || ( y == -1)) return;

	if (a[x][y] == 0){
		if (!firstTouch) return;	
	}
	else {

		char* color[6] = {"white","blue","yellow","red","green","pink"};
		char name[20];

		this->removeChild(tiles[x1][y1],true);

		sprintf (name, "%s.png", color[a[x1][y1]]);
		tiles[x1][y1] = CCSprite::create(name);
		tiles[x1][y1]->setPosition(ccp(44*x1 + 1.5*(x1+1) + 22, 44*y1 + 1.5*(y1+1) + 22));
		addChild(tiles[x1][y1], 2);
			
		x1 = x; y1 = y;

		sprintf (name, "%sBold.png", color[a[x1][y1]]);
		
		this->removeChild(tiles[x1][y1],true);
		tiles[x1][y1] = CCSprite::create(name);
		tiles[x1][y1]->setPosition(ccp(44*x1 + 1.5*(x1+1) + 22, 44*y1 + 1.5*(y1+1) + 22));
		addChild(tiles[x1][y1], 2);


		firstTouch = true;
		return;
	}

	x2 = x; y2 = y;

	setKeyMove();

	if (haveWay){	

		// save old data;
		for (int i = 0; i<7; i++)
			for(int j = 0; j < 7; j++) aBack[i][j] = a[i][j];

		oldTotalScore = totalScore;
		oldLevel = level;
		oldCountNumber = countNumber;


		char* color[6] = {"white","blue","yellow","red","green","pink"};
		char name[20];
		sprintf (name, "%s.png", color[a[x1][y1]]);
		this->removeChild(tiles[x2][y2],true);
		this->addBox(x2, y2, name);
		a[x2][y2] = a[x1][y1];

		this->removeBox(x1, y1);
		tiles[x1][y1] = CCSprite::create("white.png");
		tiles[x1][y1]->setPosition(ccp(44*x1 + 1.5*(x1+1) + 22, 44*y1 + 1.5*(y1+1) + 22));
		a[x1][y1] = 0;
		addChild(tiles[x1][y1], 1);

		firstTouch = false;
		usedOneTime = false;

		gotScore = false;
		checkResult();
		if (!gotScore){
			if(!generateLines(level+2)){
				return;
			};
			generatePrediction(level+2);
			checkResult();
			if (gotScore) runLoadingBar();
		}
		else{
			runLoadingBar();
		}
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

void HelloWorld::checkResult(){
	int u, v, countRow, countColumn, countDiagonalRight, countDiagonalLeft, colorNumber;

	//int countOne = 0, countTwo = 0, countThree = 0, countFour = 0;

	int remember[7][7];

	eat save[7][7];

	for (int i = 0; i<7; i++)
		for(int j = 0; j < 7; j++){
			remember[i][j] = 0;
			save[i][j].col = 0;
			save[i][j].row = 0;
			save[i][j].dial = 0;
			save[i][j].diar = 0;
		}

	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 7; j++)
			if (a[i][j] != 0){

				countColumn = 1;
				countRow = 1;
				countDiagonalRight = 1;
				countDiagonalLeft = 1;

				colorNumber = a[i][j];

				// column
				if (save[i][j].col == 0){
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
							save[u][v].col = countColumn;
							v--;
						}
						countNumber++;
						totalScore += (20 + (countColumn - 4)*8);
						///CCLOG("col %i", totalScore);
					}
				}

				// row
				if (save[i][j].row == 0){
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
							save[u][v].row = countRow;
							u--;
						}
						countNumber++;
						totalScore += (20 + (countRow - 4)*8);
						//CCLOG("row %i", totalScore);
					}
				}

				//diagonal right
				if (save[i][j].diar == 0){
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
							save[u][v].diar = countDiagonalRight;
							u--;
							v--;
						}
						countNumber++;
						totalScore += (20 + (countDiagonalRight - 4)*8);
					}
				}

				//diagonal left
				if (save[i][j].dial == 0){
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
							save[u][v].dial = countDiagonalLeft;
							u++;
							v--;
						}
						countNumber++;
						totalScore += (20 + (countDiagonalLeft - 4)*8);
					}
				}

			}

	for (int i = 0; i<7; i++)
		for (int j = 0; j<7; j++){
			
			int sumLines = save[i][j].col + save[i][j].row + save[i][j].dial + save[i][j].diar;
			
			//if (remember[i][j] == 2) countTwo++;
			//if (remember[i][j] == 3) countThree++;
			//if (remember[i][j] == 4) countFour++;
			

			if (remember[i][j] > 1){
				
				countKeyMove += (remember[i][j] - 1);

				int score = ((remember[i][j]*20) + (sumLines - remember[i][j]*4)*8);
				totalScore -= score;

				for (int k = remember[i][j]; k>1; k--) score = score * k;
				totalScore += score;
		
			}

			if (remember[i][j] > 0){

				gotScore = true;

				this->removeBox(i,j);
				tiles[i][j] = CCSprite::create("white.png");
				tiles[i][j]->setPosition(ccp(44*i + 1.5*(i+1) + 22, 44*j + 1.5*(j+1) + 22));
				addChild(tiles[i][j], 1);

				a[i][j] = 0;
			}
		}

	if (countKeyMove > 0) showKeyMove();

	if (countNumber >= limitLine){
		countNumber = countNumber - limitLine;
		level++;
		limitLine+=10;
		countKeyBack++;

	}

	showKeyBack();

	generatePrediction(level+2);

	showCountNumber();

	showTotalScore();
}

void HelloWorld::addBox(int i, int j, char name[]){
	this->removeChild(tiles[i][j]);
	tiles[i][j] = CCSprite::create(name);
	tiles[i][j]->setPosition(ccp(44*i + 1.5*(i+1) + 22, 44*j + 1.5*(j+1) + 22));
    tiles[i][j]->setScale(1.5);

	CCAction * addAnimation = 
		CCSequence::create(
			CCMoveTo::create(duration, tiles[i][j]->getPosition()),
			CCScaleTo::create(duration, 1),
			CCRepeat::create(
				CCSequence::create(
					CCFadeOut::create(0.25),
					CCFadeIn::create(0.25),
				0),
			2),
		0);
	tiles[i][j]->runAction(addAnimation);
	addChild(tiles[i][j], 2);
}

void HelloWorld::removeBox(int i, int j){
    tiles[i][j]->setScale(1.2);
    CCAction * removeAnimation = CCSequence::create(
			CCSpawn::create(
				CCRepeat::create(
					CCRotateBy::create(duration, 180),
					2),
				CCFadeOut::create(duration*2),
			0),
			CCCallFuncN::create(this, callfuncN_selector(HelloWorld::finishAnimation)),
        0);
	tiles[i][j]->runAction(removeAnimation);
}

void HelloWorld::finishAnimation(CCNode * pNode)
{
    this->removeChild(pNode, true);
}

void HelloWorld::endGame()
{
	WaitingScene *waitLayer = WaitingScene::create();
	Sleep(1);
	char buffer[50];
	char *prefix, *bonus;
	prefix = ""; bonus = "";
	if(multiPlayer){
		if(bonusPoint){
			bonus = "(bonus+300 point)";
			totalScore = totalScore +300;
		}else
			friendScore = friendScore + 300;
		if(totalScore > friendScore){
			prefix = "You Win\n";
		}else{
			prefix = "You lose\n";
		}
		
	}
	sprintf(buffer,"%sYour point: %i%s", prefix, totalScore, bonus);
	waitLayer->setNotify(buffer);
	
	CCScene * scene = CCScene::create();
	scene->addChild(waitLayer);
	CCDirector::sharedDirector()->replaceScene(scene);
}