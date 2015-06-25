#include "HelloWorldScene.h"
#define CONSTDISTANCE 350
#define CONSTALTITUDE 800
#define STARTALTITUDE 900
USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vec2(0,-500));
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    //Vec2 origin = Director::getInstance()->getVisibleOrigin();

    return true;
}

void HelloWorld::onEnter(){
    Layer::onEnter();
    
//    ttf = Label::createWithTTF("3", "fonts/arial.ttf", 50);
//    ttf->setColor(Color3B::YELLOW);
//    ttf->setPosition(visibleSize / 2);
//    this->addChild(ttf,2,9);
    
    addEdge();
    addBird(visibleSize.width / 2, visibleSize.height /2);
    addGround();
    
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuRestart, this));
    
    closeItem->setPosition(Vec2(visibleSize.width - closeItem->getContentSize().width/2 ,
                                closeItem->getContentSize().height/2));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu,3);
    
    maxscore = UserDefault::getInstance()->getIntegerForKey("maxscore");
    
    label = Label::createWithTTF("score : 0", "fonts/arial.ttf", 30);
    label->setPosition(visibleSize.width - 100,visibleSize.height - 50);
    this->addChild(label,2);
    
    maxlabel = Label::createWithTTF("", "fonts/arial.ttf", 30);
    maxlabel->setString(__String::createWithFormat("maxscore : %i",maxscore)->getCString());
    maxlabel->setPosition(100,visibleSize.height - 50);
    this->addChild(maxlabel,2);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this](Touch * touch,Event * event){
        bird->getPhysicsBody()->setVelocity(Vec2(0,200));
        return false;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    isGameOver = false;
    score = 0;
    srand((unsigned)time(NULL));
    
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = [this](PhysicsContact & contact){
        if (!isGameOver) {
            isGameOver = true;
            log("contact ground");
            MessageBox("Game Over", "Game Over");
            this->unscheduleUpdate();
        }
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
    
    scheduleUpdate();
    
//    Director::getInstance()->pause();
//    
//    auto delay = DelayTime::create(1.0f);
//
//    auto action2 = CallFuncN::create(CC_CALLBACK_1(HelloWorld::setSec2,this));
//    auto action1 = CallFuncN::create(CC_CALLBACK_1(HelloWorld::setSec1,this));
//    auto action0 = CallFuncN::create(CC_CALLBACK_1(HelloWorld::setSec0,this));
//    
//    auto action = Sequence::create(delay,action2,delay,action1,delay,action0,NULL);
//    
//    this->runAction(action);
}

void HelloWorld::addBird(float x, float y){
    bird = Sprite::create("bird.png");
    bird->setScale(2.0f);
    bird->setPhysicsBody(PhysicsBody::createBox(bird->getContentSize() * 2));
    bird->setPosition(x,y);
    
    bird->getPhysicsBody()->setContactTestBitmask(0x01);
    addChild(bird,1,10);
}

void HelloWorld::addGround(){
    auto ground1 = Sprite::create("ground.png");
    ground1->setScale(2.0f);
    ground1->setAnchorPoint(Vec2(0, 0));
    ground1->setPosition(Vec2(0,0));
    addChild(ground1, 2, 1);
    
    auto ground2 = Sprite::create("ground.png");
    ground2->setScale(2.0f);
    ground2->setAnchorPoint(Vec2(0,0));
    ground2->setPosition(Vec2(666,0));
    addChild(ground2,2,2);
    
    auto bar1Up = addUpBar();
    bar1Up->setAnchorPoint(Vec2(0,0));
    auto bar1Down = addDownBar();
    auto bar2Up = addUpBar();
    auto bar2Down = addDownBar();
    auto bar3Up = addUpBar();
    auto bar3Down = addDownBar();
    
    bar1Up->setPosition(700,CONSTALTITUDE + 200);
    bar1Down->setPosition(700,200);
    bar2Up->setPosition(700 + CONSTDISTANCE,CONSTALTITUDE + 300);
    bar2Down->setPosition(700 + CONSTDISTANCE,300);
    bar3Up->setPosition(700 + CONSTDISTANCE * 2,CONSTALTITUDE + 200);
    bar3Down->setPosition(700 + CONSTDISTANCE * 2,200);
    
    addChild(bar1Up,1,3);
    addChild(bar1Down,1,4);
    addChild(bar2Up,1,5);
    addChild(bar2Down,1,6);
    addChild(bar3Up,1,7);
    addChild(bar3Down,1,8);

}

void HelloWorld::addEdge(){
    auto size = Director::getInstance()->getVisibleSize();
    
    auto body = PhysicsBody::createEdgeBox(size);
    body->setContactTestBitmask(0x01);
    auto edgeShape = Node::create();
    edgeShape->setContentSize(size);
    edgeShape->setPhysicsBody(body);
    edgeShape->setPosition(size.width / 2,size.height / 2 + 220);
    addChild(edgeShape);
}

void HelloWorld::update(float dt){
    auto gd1 = this->getChildByTag(1);
    auto gd2 = this->getChildByTag(2);
    
    auto gd1PositionX = gd1->getPositionX();
    auto gd2PositionX = gd2->getPositionX();
    
    gd1->setPositionX(gd1PositionX - 5);
    gd2->setPositionX(gd2PositionX - 5);
    
    if (gd1PositionX <= -666) {
        gd1->setPositionX(gd2PositionX + 666);
    }
    
    if (gd2PositionX <= -666) {
        gd2->setPositionX(gd1PositionX + 666);
    }
    
    auto bar1Up = this->getChildByTag(3);
    auto bar1Down = this->getChildByTag(4);
    auto bar2Up = this->getChildByTag(5);
    auto bar2Down = this->getChildByTag(6);
    auto bar3Up = this->getChildByTag(7);
    auto bar3Down = this->getChildByTag(8);
    
    auto bar1PositionX = bar1Up->getPositionX();
    auto bar2PositionX = bar2Up->getPositionX();
    auto bar3PositionX = bar3Up->getPositionX();
    
    bar1Up->setPositionX(bar1PositionX - 5);
    bar1Down->setPositionX(bar1PositionX - 5);
    bar2Up->setPositionX(bar2PositionX - 5);
    bar2Down->setPositionX(bar2PositionX - 5);
    bar3Up->setPositionX(bar3PositionX - 5);
    bar3Down->setPositionX(bar3PositionX - 5);
    
    if(bar1PositionX <= -100){
        bar1Up->setPosition(bar3PositionX + CONSTDISTANCE,STARTALTITUDE + CCRANDOM_0_1() * 300);
        bar1Down->setPosition(bar3PositionX + CONSTDISTANCE,bar1Up->getPositionY() - CONSTALTITUDE);
    }
    
    if(bar2PositionX <= -100){
        bar2Up->setPosition(bar1PositionX + CONSTDISTANCE,STARTALTITUDE + CCRANDOM_0_1() * 300);
        bar2Down->setPosition(bar1PositionX + CONSTDISTANCE,bar2Up->getPositionY() - CONSTALTITUDE);
    }
    
    if(bar3PositionX <= -100){
        bar3Up->setPosition(bar2PositionX + CONSTDISTANCE,STARTALTITUDE + CCRANDOM_0_1() * 300);
        bar3Down->setPosition(bar2PositionX + CONSTDISTANCE,bar3Up->getPositionY() - CONSTALTITUDE);
    }

    auto birdPositionX = bird->getPositionX();
    if ((birdPositionX >= bar1PositionX && birdPositionX < bar1PositionX + 5) ||
        (birdPositionX >= bar2PositionX && birdPositionX < bar2PositionX + 5) ||
        (birdPositionX >= bar3PositionX && birdPositionX < bar3PositionX + 5)
        ) {
        score += 1;
        label->setString(__String::createWithFormat("score : %i",score)->getCString());
        if (score > maxscore) {
            maxlabel->setString(__String::createWithFormat("maxscore : %i",score)->getCString());
            UserDefault::getInstance()->setIntegerForKey("maxscore", maxscore);
        }
    }
}

Sprite * HelloWorld::addUpBar(){
    auto spr = Sprite::create("up_bar.png");
    auto body = PhysicsBody::createEdgeBox(spr->getContentSize() * 2);
    body->setContactTestBitmask(0x01);
    spr->setScale(2.0f);
    spr->setAnchorPoint(Vec2(0,0));
    spr->setPhysicsBody(body);
    return spr;
}

Sprite * HelloWorld::addDownBar(){
    auto spr = Sprite::create("down_bar.png");
    auto body = PhysicsBody::createEdgeBox(spr->getContentSize() * 2);
    body->setContactTestBitmask(0x01);
    spr->setScale(2.0f);
    spr->setAnchorPoint(Vec2(1,0));
    spr->setPhysicsBody(body);
    
    return spr;
}

void HelloWorld::startGame(float dt){
    scheduleUpdate();
}

void HelloWorld::menuRestart(cocos2d::Ref *pSender){
    auto scene = HelloWorld::createScene();
    Director::getInstance()->replaceScene(scene);
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
