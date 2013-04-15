#include "HelloWorldScene.h"

using namespace cocos2d;


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

		sprintf (name, "%s.png", color[tilesHintPos[k].color]);

		this->removeChild(tiles[i][j]);
		tiles[i][j] = CCSprite::create(name);
		tiles[i][j]->setPosition(ccp(44*i + 1.5*(i+1) + 22, 44*j + 1.5*(j+1) + 22));
		addChild(tiles[i][j]);
		a[i][j] = tilesHintPos[k].color;
	}

}

void HelloWorld::generatePrediction(int numberOfPoint){

	char* color[6] = {"white","blue","yellow","red","green","pink"};

	int numberOfColor = 5;
	int i = 0, j = 0;

	char name[20];

	for (int k = 0; k < numberOfPoint; k++){

		int actual = rand() % 5 + 1;
		sprintf (name, "%sHint.png", color[actual]);

		this->removeChild(tilesHint[i][j]);
		tilesHint[i][j] = CCSprite::create(name);
		tilesHint[i][j]->setPosition(ccp(tilesHintPos[k].x, tilesHintPos[k].y));
		addChild(tilesHint[i][j]);
		tilesHintPos[k].color = actual;

		j++;
		if (j > 2){
			i++; j = 0;
		}
	}

}
