//
//  RewardScene.cpp
//  AirHockey
//
//

#include "RewardScene.h"

USING_NS_CC;
USING_NS_CC_EXT;

CCScene* RewardScene::scene() {
    CCScene *scene = CCScene::create();
    RewardScene *layer1 = RewardScene::create();
    scene->addChild(layer1);
    return scene;
}

bool RewardScene::init() {
    size    = CCDirector::sharedDirector()->getWinSize();
    w       = size.width;
    h       = size.height;
    SIZE_RATIO = (w + h)/(768 + 1024);
    SIZE_RATIO_X = w/768;
    SIZE_RATIO_Y = h/1024;

    players = new CCArray();
    
    CCSprite *background = CCSprite::create("BackGrounds/RewardBG.png");
    background->setPosition(ccp(w/2, h/2));
    background->setScaleY(h/background->getContentSize().height);
    background->setScaleX(w/background->getContentSize().width);

    this->addChild(background);
    
    string playerName = GameManager::sharedGameManager()->getName();
    if (playerName != "") {
        char nameBuf[50];
        sprintf(nameBuf, "Player Name: %s", playerName.c_str());
        this->convertName(nameBuf);
        CCLabelTTF *playerNameLabel = CCLabelTTF::create(nameBuf, FONT, 30*SIZE_RATIO);
        playerNameLabel->setPosition(ccp(w/2, h*3/4));
        this->addChild(playerNameLabel);
    }
    
    CCHttpRequest* request = new CCHttpRequest();
    string ipAddr = GameManager::sharedGameManager()->getIpAddr();
    request->setUrl((ipAddr+":3000/users.json").c_str());
    request->setRequestType(CCHttpRequest::kHttpGet);
    request->setResponseCallback(this, callfuncND_selector(RewardScene::onHttpRequestCompleted));
    CCHttpClient::getInstance()->send(request);
    request->release();
    
    //create startMenuItem
    CCMenuItemImage *back = CCMenuItemImage::create("Buttons/BackButton.png",
                                                    "Buttons/BackButtonOnClicked.png",
                                                    this, menu_selector(RewardScene::back));
    back->setScale(SIZE_RATIO);
    back->setPosition(ccp(w/2, h/8));
    
    CCMenu* pMenu = CCMenu::create(back, NULL);
    pMenu->setPosition(ccp(0,0));
    this->addChild(pMenu);
        
    return true;
}

void RewardScene::onHttpRequestCompleted(CCNode *sender, void *data) {
    CCHttpResponse *response = (CCHttpResponse*)data;
    
    if (!response) {
        return;
    }

    if (0 != strlen(response->getHttpRequest()->getTag())) {
        CCLog("%s completed", response->getHttpRequest()->getTag());
    }
    
    int statusCode = response->getResponseCode();
    char statusString[64] = {0};
    sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode,
            response->getHttpRequest()->getTag());
    
    if (!response->isSucceed()) {
        CCLabelTTF *checkInternetMsg = CCLabelTTF::create("「現在ランキングは閉じています」", FONT, 24*SIZE_RATIO);
        checkInternetMsg->setPosition(ccp(w/2, h/2 - 40*SIZE_RATIO_Y));
        this->addChild(checkInternetMsg);

        return;
    }
    
    // dump data
    std::vector<char> *buffer = response->getResponseData();
    char * data2 = (char*)(malloc(buffer->size() *  sizeof(char)));
    int d = -1;
    printf("Http Test, dump data: ");
    for (unsigned int i = 0; i < buffer->size(); i++) {
        d++ ;
        data2[d] = (*buffer)[i];
    }
    data2[d + 1] = '\0';
    //-----------------------

    rapidjson::Document document;
    if(data2 != NULL && !document.Parse<0>(data2).HasParseError()) {
        string username = GameManager::sharedGameManager()->getName();
        string email = GameManager::sharedGameManager()->getEmail();
        for (rapidjson::SizeType  i = 0; i < document.Size(); i++) {
            string name = document[i]["name"].GetString();
            convertName((char*)name.c_str());
            if (username == name &&
                email == document[i]["email"].GetString()){
                string mail = document[i]["email"].GetString();
                string time = document[i]["updated_at"].GetString();
                int p = document[i]["point"].GetInt();
                int r = document[i]["reward"].GetInt();
                Player1 *player = new Player1(name,p, mail, time, r);
                players->addObject(player);
            }
        }
    } else {
        CCLog(document.GetParseError());
    }
    free(data2);
    CCTableView *tableView=CCTableView::create(this, CCSizeMake(700*SIZE_RATIO_X,
                                                                350*SIZE_RATIO_Y));
    tableView->setDirection(kCCScrollViewDirectionVertical);
    tableView->setAnchorPoint(ccp(0, 0));
    tableView->setPosition(ccp(w/8, 250*SIZE_RATIO_Y));
    tableView->setDelegate(this);
    tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    this->addChild(tableView, 21);
    tableView->reloadData();
}

void RewardScene::clickBtSendEmail(cocos2d::CCObject *pSender) {
    CCLOG("cell: %i", celltouch);
    CCMenuItemImage *bt_send_email = (CCMenuItemImage*)pSender;
    CCLOG("tag menu : %i", bt_send_email->getTag());
    CCHttpRequest * request = new CCHttpRequest();
    Player1 *p = (Player1*)players->objectAtIndex(bt_send_email->getTag() - 100);
    int point = p->getPoint();
    string name = GameManager::sharedGameManager()->getName();
    convertName2((char*)name.c_str());
    char strP[20] = {0};
    sprintf(strP, "%i", point);
    string email  = GameManager::sharedGameManager()->getEmail();
    string ipAddr = GameManager::sharedGameManager()->getIpAddr();
    string url    = ipAddr + ":3000/users?name="+name+"&point="+strP+"&email="+email+"&reward=abc&time="+p->getTime();
    request->setUrl(url.c_str());
    request->setRequestType(CCHttpRequest::kHttpPost);
    CCHttpClient::getInstance()->send(request);
    request->release();
    p->setReward(-1);
    int r = GameManager::sharedGameManager()->getReward();
    GameManager::sharedGameManager()->setReward(r - 1);
    bt_send_email->removeFromParentAndCleanup(true);
}

void RewardScene::back(CCObject* pSender) {
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.7f, RankingScene::scene()));
}

void RewardScene::tableCellTouched(CCTableView* table, CCTableViewCell* cell) {
    CCLOG("cell touched at index: %i", cell->getIdx());
    celltouch = cell->getIdx();
}

CCSize RewardScene::tableCellSizeForIndex(CCTableView *table, unsigned int idx) {
    return CCSizeMake(600*SIZE_RATIO_X, 80*SIZE_RATIO_Y);
}

CCTableViewCell* RewardScene::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    CCTableViewCell *cell = table->dequeueCell();
    cell = new CCTableViewCell();
    cell->autorelease();
    
    // Player Point
    Player1 * p = (Player1*)players->objectAtIndex(idx);
    CCString *string = CCString::createWithFormat("%d",p->getPoint());

    CCLabelTTF *Pointlabel = CCLabelTTF::create(string->getCString(),
                                                "Helvetica",
                                                30*SIZE_RATIO);
    Pointlabel->setAnchorPoint(ccp(1, 0));
    Pointlabel->setPosition(ccp(500*SIZE_RATIO_X, 25*SIZE_RATIO_Y));
    Pointlabel->setTag(123);
    cell->addChild(Pointlabel);
    //time
    std::string sttime = p->getTime();
//    this->convertTime((char*)sttime.c_str());
    char timeBuf[50];
    strcpy(timeBuf, sttime.c_str());
    this->convertTime(timeBuf);
    CCLabelTTF *timeLabel = CCLabelTTF::create(timeBuf,
                                               "Helvetica",
                                               20*SIZE_RATIO);
    timeLabel->setAnchorPoint(CCPointZero);
    timeLabel->setOpacity(90);
    timeLabel->setPosition(ccp(100*SIZE_RATIO_X, 10*SIZE_RATIO_Y));
    cell->addChild(timeLabel);

    // Player Name
    std::string name = p->getName();
    CCLabelTTF *Namelabel = CCLabelTTF::create(p->getName().c_str(),
                                               "Helvetica",
                                               30*SIZE_RATIO);
    Namelabel->setAnchorPoint(CCPointZero);
    Namelabel->setPosition(ccp(100*SIZE_RATIO_X, 30*SIZE_RATIO_Y));
    cell->addChild(Namelabel);
    
    // Player Rank
    CCSprite *rank;
    if (p->getReward() != 0) rank = CCSprite::create("BestScore.png");
    else rank = CCSprite::create("Top10.png");
    rank->setAnchorPoint(CCPointZero);
    rank->setScale(SIZE_RATIO);
    rank->setPosition(CCPointZero);
    cell->addChild(rank);
    
    int rewardLocal = GameManager::sharedGameManager()->getReward();
    std::string nameLocal = GameManager::sharedGameManager()->getName();
    if (p->getReward() > 0 && rewardLocal != 0) {
        CCMenuItemImage *bt_send_email =
            CCMenuItemImage::create("Present.png","PresentOnClicked.png",
                                    this, menu_selector(RewardScene::clickBtSendEmail));
        bt_send_email->setTag(idx + 100);
        rewardMenu = CCMenu::create(bt_send_email, NULL);
        rewardMenu->setPosition(ccp(550*SIZE_RATIO_X, 40*SIZE_RATIO_Y));
        cell->addChild(rewardMenu);
    }
    
    return cell;
}

void RewardScene::convertName(char *str_name) {
    int len = 0;
    int i = 0;
    len=strlen(str_name);
    for(i=0;i<len;i++) {
        if(str_name[i] == '_') {
            str_name[i] = ' ';
        }
    }
}
void RewardScene::convertName2(char *str_name) {
    int len = 0;
    int i = 0;
    len=strlen(str_name);
    for(i=0;i<len;i++) {
        if(str_name[i] == ' ') {
            str_name[i] = '_';
        }
    }
}
void RewardScene::convertTime(char *str_time) {
    int len = 0;
    int i = 0;
    len=strlen(str_time);
    for(i=0;i<len;i++) {
        if(str_time[i] == 'T') {
            str_time[i] = '  ';
        }else if (str_time[i] == 'Z') str_time[i] = '\0';
    }
}

void RewardScene::convertTime2(char *str_time) {
    int len = 0;
    int i = 0;
    len=strlen(str_time);
    for(i=0;i<len;i++) {
        if(str_time[i] == ' ') {
            str_time[i] = 'T';
        }
    }
}


unsigned int RewardScene::numberOfCellsInTableView(CCTableView *table) {
    return players->count();
}

Player1::Player1(string name , int point, string email, string time, int reward) {
    this->_point    = point;
    this->_name     = name;
    this->_email    = email;
    this->_reward   = reward;
    this->_time     = time;
}
Player1::~Player1(){}
