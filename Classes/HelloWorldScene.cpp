#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

enum {
    kBallCollisionGroup = 10
};

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vec2(0,0));
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

//function for moving balls
void moveToWithVelocity(cocos2d::Node * ballNode, const float & directionAngleInDegrees, const float & absoluteVelocity)
{
    auto body = ballNode->getPhysicsBody();
    
    auto mass = body->getMass();
    auto currentVelocity = body->getVelocity();
    auto targetVelocity = Vec2::forAngle(CC_DEGREES_TO_RADIANS(directionAngleInDegrees))*absoluteVelocity;
    
    body->applyImpulse(mass*(targetVelocity-currentVelocity));
};

HelloWorld::HelloWorld():
player1(nullptr),
player2(nullptr)
{
    
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
    
    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to start moving objects
    //    you may modify it.
    
    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuMoveBallsCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width,
                                origin.y + visibleSize.height - closeItem->getContentSize().height));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    //creating screen box
    auto pStaticBoxBody = PhysicsBody::createEdgeBox(visibleSize, PhysicsMaterial(PHYSICS_INFINITY, 0.5f, PHYSICS_INFINITY), 2.f);
    pStaticBoxBody->setMoment(PHYSICS_INFINITY);
    pStaticBoxBody->setMass(PHYSICS_INFINITY);
    pStaticBoxBody->setDynamic(false);
    this->setPhysicsBody(pStaticBoxBody);
    
    //lambda for creating balls
    auto createBall = [](float radius, const PhysicsMaterial& material = PHYSICSBODY_MATERIAL_DEFAULT) -> Node* {
        auto ballNode = Node::create();
        ballNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        auto ballBody = PhysicsBody::createCircle(radius, PHYSICSBODY_MATERIAL_DEFAULT);
        //auto ballBody = PhysicsBody::createEdgeBox(Size(radius*2, radius*2),material);
        ballBody->setGroup(kBallCollisionGroup);
        ballBody->setRotationEnable(false);
        ballBody->setDynamic(true);
        ballNode->setPhysicsBody(ballBody);
        return ballNode;
    };
    
    {
        //create player1
        const auto r1 = 100.f;
        player1 = createBall(r1, PhysicsMaterial(0.f, 0.5f, 0.f));
        player1->setPosition(Vec2(visibleSize.width/2.f, r1+50.f));
        this->addChild(player1);
        player1->getPhysicsBody()->setLinearDamping(0.3f);
    }
    
    {
        //create player2
        const auto r2 = 100.f;
        player2 = createBall(r2, PhysicsMaterial(0.f, 0.5f, 0.f));
        player2->setPosition(Vec2(visibleSize.width/2.f-2*r2-150, r2+50.f));
        this->addChild(player2);
        player2->getPhysicsBody()->setLinearDamping(0.3f);
    }
    
    this->scheduleUpdate();
    
    return true;
}

void HelloWorld::update(float delta)
{
    auto MV = 15;
    auto v1 = player1->getPhysicsBody()->getVelocity();
    auto v2 = player2->getPhysicsBody()->getVelocity();
    if (v1.x > MV || v1.x < -MV ||
        v1.y > MV || v1.y < -MV) {
    } else if(v1 != Vec2(0,0)) {
        player1->getPhysicsBody()->setVelocity(Vec2(0,0));
        CCLOG("sx 1 : %f %f",player1->getPosition().x,player1->getPosition().y);
        MessageBox((std::to_string(player1->getPosition().x) + " " + std::to_string(player1->getPosition().y)).c_str(), "Competitor shot at");
    }
    
    if (v2.x > MV || v2.x < -MV ||
        v2.y > MV || v2.y < -MV) {
    } else if(v2 != Vec2(0,0)) {
        player2->getPhysicsBody()->setVelocity(Vec2(0,0));
        CCLOG("sx 2 : %f %f",v2.x,v2.y);
    }
}

void HelloWorld::menuMoveBallsCallback(Ref* pSender)
{
    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const auto r1 = 100.f;
    const auto r2 = 100.f;

    player1->setPosition(Vec2(visibleSize.width/2.f, r1+50.f));
    player2->setPosition(Vec2(visibleSize.width/2.f-2*r2-150, r2+50.f));
}

void HelloWorld::onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event) {
    CCLOG("start moving balls");
    //move player1
    //moveToWithVelocity(player1, -40.f, 300.f);
    player1->getPhysicsBody()->setVelocity(Vec2(100,500));
    
    //move player2
    //moveToWithVelocity(player2, 0.f, 200.f);
    //player2->getPhysicsBody()->setVelocity(Vec2(100,-10));
}
void HelloWorld::onEnter() {
    Layer::onEnter();
    // touch event listener
    auto eventListener = EventListenerTouchAllAtOnce::create();
    eventListener->onTouchesBegan = CC_CALLBACK_2(HelloWorld::onTouchesBegan, this);
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, this);

}
