#include "HelloWorldScene.h"
#include "CurlTest.h"
#include "AppMacros.h"
#include "stdio.h"
#include "stdlib.h"
#include <enet/enet.h>

USING_NS_CC;

CCLabelTTF* pLabel;
char* notify;
HelloWorld *playLayer;
HelloWorld *viewLayer;
ENetHost * server;
ENetPeer *peer;
int channel, currentState, nextAtk;
char packetBuffer[150];
std::mutex mutexMapView;
std::mutex mutexMapPlay;
//pthread_mutex_t mutexMapView = PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_t mutexMapPlay = PTHREAD_MUTEX_INITIALIZER;
int oldScore;

CCScene* CurlTest::scene()
{
    CCScene *scene = CCScene::create();    
    CurlTest *layer = CurlTest::create();
    scene->addChild(layer);
	if (enet_initialize () != 0)
	{
		CCLOG("An error occurred while initializing ENet.\n");
		return NULL;
	}
    return scene;
}

void initServer(){
	CCLOG("begin initServer");
	channel = 0;
	ENetAddress address;
	/* Bind the server to the default localhost. */
	/* A specific host address can be specified by */
	/* enet_address_set_host (& address, "x.x.x.x"); */
	address.host = ENET_HOST_ANY;
	/* Bind the server to port 1234. */
	address.port = 1234;
	server = enet_host_create (& address /* the address to bind the server host to */,
				1 /* allow up to 32 clients and/or outgoing connections */,
				1 /* allow up to 2 channels to be used, 0 and 1 */,
				0 /* assume any amount of incoming bandwidth */,
				0 /* assume any amount of outgoing bandwidth */);
	if (server == NULL){
		CCLOG("An error occurred while trying to create an ENet server host.\n");
		return;
	}
}

// on "init" you need to initialize your instance
bool CurlTest::init(){
    if ( !CCLayerColor::initWithColor(ccc4(255,255,255,255)) )
    {
        return false;
    }

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();	

    CCMenuItemImage *send = CCMenuItemImage::create(
                                        "send.png",
                                        "connect.png",
                                        this,
                                        menu_selector(CurlTest::send));
    CCMenuItemImage *recv = CCMenuItemImage::create(
                                        "recv.png",
                                        "wait.png",
                                        this,
                                        menu_selector(CurlTest::recv));

	CCMenu* pMenu = CCMenu::create();
	pMenu->addChild(send);
	pMenu->addChild(recv);
    pMenu->setPosition(ccp(origin.x + visibleSize.width/2 ,
								origin.y + recv->getContentSize().height));
	pMenu->alignItemsHorizontally();

	notify = "";
    pLabel = CCLabelTTF::create(notify, "Arial", TITLE_FONT_SIZE);
    pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height/2));
	
    pTextField = CCTextFieldTTF::textFieldWithPlaceHolder("<enter your friend's IP>",
        "Arial",
		TITLE_FONT_SIZE);
    pTextField->setPosition(ccp(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - 2*pTextField->getContentSize().height));
	
    this->addChild(pMenu);
    this->addChild(pLabel);
    this->addChild(pTextField);
	this->setTouchEnabled(true);

	peer = NULL;
	playLayer = NULL;
	viewLayer = NULL;
	initServer();
	return true;
}

void CurlTest::initStartGame()
{
	CCLOG("begin initStartGame");
	this->removeAllChildren();
	this->setTouchEnabled(false);
	currentState = 0;
	nextAtk = 0;
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();	

    viewLayer = HelloWorld::create();
    playLayer = HelloWorld::create();
	playLayer->mutexMapView = &mutexMapView;
	playLayer->mutexMapPlay = &mutexMapPlay;
	playLayer->multiPlayer = true;
	CCLOG("add playLayer");
	this->addChild(playLayer);
	viewLayer->setTouchEnabled(false);
	viewLayer->setScale(0.3);
	viewLayer->setPosition(ccp(origin.x + visibleSize.width - viewLayer->getContentSize().width*2/3,
								origin.y - viewLayer->getContentSize().height/3));
	CCLOG("add viewLayer");
	this->addChild(viewLayer);
	CCLOG("finish initStart");
}

void CurlTest::draw (void){
//	CCLOG("begin draw");
	CCLayer::draw();
	mutexMapView.lock();
//	pthread_mutex_lock(&mutexMapView);
	if(viewLayer != NULL){
		viewLayer->countKeyBack = 1;
		viewLayer->countKeyMove = 0;
		viewLayer->setKeyBack();
		playLayer->friendScore = viewLayer->totalScore;
		if(viewLayer->checkEndGame()){
			playLayer->bonusPoint = true;
			playLayer->endGame();
		}
	}
	if(nextAtk !=0){
		playLayer->generateLines(nextAtk*4);
		playLayer->checkResult();		
		mutexMapPlay.unlock();
//		pthread_mutex_unlock(&mutexMapPlay);
		nextAtk = 0;
		if(playLayer->checkEndGame()){
			playLayer->bonusPoint = false;
			playLayer->endGame();
		}

	}
	mutexMapView.unlock();
//	pthread_mutex_unlock(&mutexMapView);
}

void CurlTest::ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent){
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CCTouch* touch = (CCTouch*)(pTouches->anyObject());
	CCPoint location = touch->getLocationInView();
	location = CCDirector::sharedDirector()->convertToGL(location);	

	if(pTextField->boundingBox().containsPoint(location)){
		pTextField->attachWithIME();
	}else{
        pTextField->detachWithIME();
	}
}

void updateMap(HelloWorld* view, enet_uint8* data){	
	int k = 0;
	CCLOG("update...");
	mutexMapView.lock();
//	pthread_mutex_lock(&mutexMapView);
	for(int i =0; i < 7; i++)
		for(int j =0; j < 7; j++){
			view->aBack[i][j] = data[k] - '0';
			k++;
		}
	nextAtk = nextAtk + data[k] - 1;
	k++;

	int temp = 0;
	int i = 0;
	while(data[i]!= 0)
		i++;
	i--;
	while(i>=k){
		temp = temp*10 + data[i] - '0';
		i--;
	}
	CCLOG("friend score: %i", temp);
	view->oldTotalScore = temp;
	
	mutexMapView.unlock();
//	pthread_mutex_unlock(&mutexMapView);
}

void mapLayerToString(HelloWorld* view, char packetBuffer[]){
	int k = 0;
	for(int i =0; i < 7; i++)
		for(int j =0; j < 7; j++){
			packetBuffer[k] = '0' + view->a[i][j];
			k++;
		}

	int atkPoint = 1;
	CCLOG("%i %i", view->totalScore, oldScore);
	if(view->totalScore - oldScore > 20){
		atkPoint =atkPoint + ((view->totalScore - oldScore - 20 + 7)/8);
	}
	oldScore = view->totalScore;
	int score = (view->totalScore / 100);
	if(score > currentState){
		atkPoint = atkPoint + (score - currentState);
		currentState = score;
	}
	if(atkPoint >4) atkPoint = 4;
	CCLOG("atkpoint %i", atkPoint);
	packetBuffer[k] = atkPoint;
	k++;
	
	int temp = view->totalScore;
	while(temp){
		packetBuffer[k] = '0' + (temp%10);
		temp = temp / 10;
		k++;
	}
	packetBuffer[k] = 0;
	CCLOG("%s %i", packetBuffer, strlen(packetBuffer));
}

void* ThreadSend(){
	while(1){
		CCLOG("wait mutex to send");
		
		mutexMapPlay.lock();
//		pthread_mutex_lock(&mutexMapPlay);		
		mapLayerToString(playLayer, packetBuffer);
//		strcpy(packetBuffer, "0000000030000003000000000000000000000010000000000\1");
		ENetPacket * packet = enet_packet_create (packetBuffer,
								150,
								ENET_PACKET_FLAG_RELIABLE);
		CCLOG(packet->data);
		/* Send the packet to the peer over channel id 0. */
		/* One could also broadcast the packet by */
		/* enet_host_broadcast (host, 0, packet); */
		enet_peer_send (peer, 0, packet);
		/* One could just use enet_host_service() instead. */
		enet_host_flush (server);
		CCLOG("finish send packet");
	}

	enet_peer_disconnect (peer, 0);
	enet_peer_reset (peer);
	CCLOG("exit thread send");
    return NULL;
}

void* ThreadRecv(){
	ENetEvent event;
	/* Wait up to 1000 milliseconds for an event. */
	while(1){
		CCLOG("begin recv");
		while (enet_host_service (server, & event, 1000000) > 0){
			switch (event.type){
				case ENET_EVENT_TYPE_CONNECT:
					CCLOG("A new client connected from %x:%u.\n",
					event.peer -> address.host,
					event.peer -> address.port);
					/* Store any relevant client information here. */
					char temp[50];
					strcat(temp, "Client information");
					event.peer -> data = temp;
					//channel = 1;
					break;
				case ENET_EVENT_TYPE_RECEIVE:
					CCLOG("A packet of length %u containing %s was received on channel %u.\n",
					event.packet -> dataLength,
					event.packet -> data,
					event.channelID);
					
					updateMap(viewLayer, event.packet -> data);
					CCLOG("update finish");

					enet_packet_destroy (event.packet);
					CCLOG("packet destroyed");
					break;
				case ENET_EVENT_TYPE_DISCONNECT:
					CCLOG("%s disconected.\n", event.peer -> data);
					/* Reset the peer's client information. */
					event.peer -> data = NULL;
			}
		}
	}
	enet_host_destroy(server);

    return NULL;
}

void CurlTest::send(CCObject* pSender)
{
	ENetAddress address;
	ENetEvent event;
	char ip[50];
	strcpy(ip, pTextField->getString ());
	/* Connect to IP:1234. */
	CCLOG("ip: %s", ip);
	enet_address_set_host (& address, ip);
	address.port = 1234;
	/* Initiate the connection, allocating the two channels 0 and 1. */
	peer = enet_host_connect (server, & address, 1, 0);
	if (peer == NULL){
		CCLOG("No available peers for initiating an ENet connection.\n");
		return;
	}
	/* Wait up to 5 seconds for the connection attempt to succeed. */
	if (enet_host_service (server, & event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
	{
		CCLOG ("Connection to IP:1234 succeeded.");
		initStartGame();
	}
	else
	{
		CCLOG ("Connection to host:1234 failed.");
		enet_peer_reset (peer);
		pLabel-> setString("connect fail");
		return;
	}
	
	std::thread threadRecv(ThreadRecv);
	std::thread threadSend(ThreadSend);

/*	pthread_t threadRecv;
	pthread_t threadSend;
	pthread_create(&threadRecv, NULL, &ThreadRecv, NULL);
	pthread_create(&threadSend, NULL, &ThreadSend, NULL);	*/
}

void CurlTest::recv(CCObject* pSender)
{
	CCLOG("waiting for connection");
	ENetEvent event;

	while (enet_host_service (server, & event, 10000) > 0){
		if(event.type==ENET_EVENT_TYPE_CONNECT){
			CCLOG("connected %x:%u.\n",
			event.peer -> address.host,
			event.peer -> address.port);
			char* temp;
			temp = "Client";
			event.peer -> data = temp;
			peer = event.peer;
			initStartGame();
				
			std::thread threadRecv(ThreadRecv);
			std::thread threadSend(ThreadSend);
			return;
/*				pthread_t threadRecv;
				pthread_t threadSend;
				pthread_create(&threadRecv, NULL, &ThreadRecv, NULL);
				pthread_create(&threadSend, NULL, &ThreadSend, NULL);	*/
		}
		if(event.type==ENET_EVENT_TYPE_DISCONNECT){
				CCLOG("%s disconected.\n", event.peer -> data);
				enet_peer_reset (peer);
		}
	}
	pLabel-> setString("time out");
}

