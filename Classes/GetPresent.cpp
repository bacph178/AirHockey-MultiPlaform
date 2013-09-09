//
//  GetPresent.cpp
//  AirHockey
//
//

#include "GetPresent.h"
int getPresentWriter(char *data, size_t size, size_t nmemb, string *buffer)
{
    int result = 0;
    if (buffer != NULL)
    {
        //バッファ追記
        buffer->append(data, size * nmemb);
        result = size * nmemb;
    }
    return result;
}

CCScene* GetPresent::scene() {
    CCScene *scene = CCScene::create();
    GetPresent *layer = GetPresent::create();
    scene->addChild(layer);
    return scene;
}

bool GetPresent::init() {
    isSent = true;
    userOK = 1;
    userOKName = 1;
    userOKMail = 1;
    size = CCDirector::sharedDirector()->getWinSize();
    w = size.width;
    h = size.height;
    SIZE_RATIO = (w + h)/(768 + 1024);
    SIZE_RATIO_X = w/768;
    SIZE_RATIO_Y = h/1024;
    
    CCSprite *bg_round = CCSprite::create("BackGrounds/GetPresentBG.png");
    bg_round->setPosition(ccp(w/2, h/2));
    bg_round->setScale(SIZE_RATIO);
    this->addChild(bg_round);
    
    CCSize editBoxSize = CCSizeMake((w - (350 * SIZE_RATIO_X)), 50 * SIZE_RATIO_Y);
    
    CCLabelTTF *congrats = CCLabelTTF::create("TOP 10 SCORE !!!", FONT, 64 * SIZE_RATIO);
    CCLabelTTF *emailLabel1 = CCLabelTTF::create("メールアドレス:", FONT, 30 * SIZE_RATIO);
    //    CCLabelTTF *emailLabel2 = CCLabelTTF::create("to get Presents:", FONT, 36 * SIZE_RATIO);
    
    congrats->setPosition(ccp(w/2, h*6/8));
    congrats->setColor(ccYELLOW);
    
    // name
    CCLabelTTF *nameLabel = CCLabelTTF::create("ニックネーム:", FONT, 30 * SIZE_RATIO);
    nameLabel->setPosition(ccp(w/2 - w/5, h*4.6f/8));
    this->addChild(nameLabel);
    m_pUserName =
    extension::CCEditBox::create(editBoxSize,
                                 extension::CCScale9Sprite::create("WhiteBox.png"));
    m_pUserName->setPosition(ccp(w/2 - w/14, nameLabel->getPositionY() - 50 * SIZE_RATIO));
    m_pUserName->setFontSize(40 * SIZE_RATIO);
    m_pUserName->setFontColor(ccBLACK);
    m_pUserName->setMaxLength(10);
    m_pUserName->setPlaceHolder("input username");
    m_pUserName->setReturnType(cocos2d::extension::kKeyboardReturnTypeDone);    
    m_pUserName->setInputMode(kEditBoxInputModeAny);
    m_pUserName->setDelegate(this);
    this->addChild(m_pUserName);
    
    nameFailMsg = CCLabelTTF::create("please enter your username !!",
                                     FONT, 24 * SIZE_RATIO);
    nameFailMsg->setAnchorPoint(CCPointZero);
    nameFailMsg->setPosition(ccp(m_pUserName->getPosition().x -
                                 m_pUserName->getContentSize().width/2,  
                                 m_pUserName->getPosition().y - 50 * SIZE_RATIO));
    nameFailMsg->setColor(ccRED);
    nameFailMsg->setVisible(false);
    this->addChild(nameFailMsg);
    
    nameExistedMsg = CCLabelTTF::create("username existed!! please try again !!",
                                        FONT, 24 * SIZE_RATIO);
    nameExistedMsg->setAnchorPoint(CCPointZero);
    nameExistedMsg->setPosition(ccp(m_pUserName->getPosition().x -
                                    m_pUserName->getContentSize().width/2,
                                    m_pUserName->getPosition().y - 50 * SIZE_RATIO));
    nameExistedMsg->setColor(ccRED);
    nameExistedMsg->setVisible(false);
    this->addChild(nameExistedMsg);
    
    //--------email-----------
    emailLabel1->setPosition(ccp(w/2 - w/5, h*3.6f/8));
    //    emailLabel2->setPosition(ccp(w/2, emailLabel1->getPositionY() - 45*SIZE_RATIO));
    
    this->addChild(emailLabel1);
    this->addChild(congrats);
    //    this->addChild(emailLabel2);
    
    m_pUserEmail = extension::CCEditBox::create(editBoxSize, extension::CCScale9Sprite::create("WhiteBox.png"));
    m_pUserEmail->setPosition(ccp(w/2 - w/14, emailLabel1->getPositionY() - 50*SIZE_RATIO));
    m_pUserEmail->setFontSize(40 * SIZE_RATIO);
    m_pUserEmail->setMaxLength(60);
    m_pUserEmail->setPlaceHolder("input email");
    m_pUserEmail->setFontColor(ccBLACK);
    m_pUserEmail->setReturnType(cocos2d::extension::kKeyboardReturnTypeDone);
    m_pUserEmail->setInputMode(kEditBoxInputModeEmailAddr);
    m_pUserEmail->setDelegate(this);
    this->addChild(m_pUserEmail);
    
    // Email Fail Message
    emailFailMsg = CCLabelTTF::create("Invalid Email !! Please Try Again !!",
                                      FONT, 24 * SIZE_RATIO);
    emailFailMsg->setAnchorPoint(CCPointZero);
    emailFailMsg->setPosition(ccp(m_pUserEmail->getPosition().x -
                                  m_pUserEmail->getContentSize().width/2,
                                  m_pUserEmail->getPosition().y - 50*SIZE_RATIO));
    emailFailMsg->setColor(ccRED);
    emailFailMsg->setVisible(false);
    
    emailExistedMsg = CCLabelTTF::create("Email Existed !! please try again !!",
                                         FONT, 24 * SIZE_RATIO);
    emailExistedMsg->setAnchorPoint(CCPointZero);
    emailExistedMsg->setPosition(ccp(m_pUserEmail->getPosition().x -
                                     m_pUserEmail->getContentSize().width/2,
                                     m_pUserEmail->getPosition().y - 50*SIZE_RATIO));
    emailExistedMsg->setColor(ccRED);
    emailExistedMsg->setVisible(false);
    this->addChild(emailFailMsg);
    this->addChild(emailExistedMsg);
    
    
    CCMenuItemImage *sendMenuItem = CCMenuItemImage::create("Buttons/SubmitButton.png",
                                                            "Buttons/SubmitButtonOnClicked.png",
                                                            this, menu_selector(GetPresent::menuSendEmail));
    sendMenuItem->setScale(SIZE_RATIO);
    sendMenuItem->setPosition(ccp(w/2 , 150 * SIZE_RATIO_Y));
    
    pMenu = CCMenu::create(sendMenuItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);
    
    return true;
}
void GetPresent::editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox)
{
    CCLog("editBox %p DidBegin !", editBox);
}

void GetPresent::editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox)
{
    CCLog("editBox %p DidEnd !", editBox);
}

void GetPresent::editBoxTextChanged(cocos2d::extension::CCEditBox* editBox,
                                    const std::string& text)
{
    CCLog("editBox %p TextChanged, text: %s ", editBox, text.c_str());
}

void GetPresent::editBoxReturn(cocos2d::extension::CCEditBox* editBox) {
    if (strcmp(m_pUserEmail->getText(),"") != 0) {
        this->checkEmail();
    }
    if (strcmp(m_pUserName->getText(), "") != 0) {
        this->checkName();
    }
}

#pragma mark valid Email
bool GetPresent::is_email(std::string const& address) {
    size_t at_index = address.find_first_of('@', 0);
    return at_index != std::string::npos
    
    && address.find_first_of('.', at_index) != std::string::npos;
    
}
int GetPresent::spc_email_isvalid(const char *address) {
    int        count = 0;
    const char *c, *domain;
    static char *rfc822_specials = "()<>@,;:\\\"[]!#$&%|*+}{?\'";
    
    /* first we validate the name portion (name@domain) */
    for (c = address;  *c;  c++) {
        if (*c == '\"' && (c == address || *(c - 1) == '.' || *(c - 1) ==
                           '\"')) {
            while (*++c) {
                if (*c == '\"') break;
                if (*c == '\\' && (*++c == ' ')) continue;
                if (*c == '&' || *c == '%' || *c == '$' || *c == '#' || *c == '!'
                    || *c <= ' ' || *c >= 127) return 0;
            }
            if (!*c++) return 0;
            if (*c == '@') break;
            if (*c != '.') return 0;
            continue;
        }
        if (*c == '@') break;
        if (*c <= ' ' || *c >= 127) return 0;
        if (strchr(rfc822_specials, *c)) return 0;
    }
    if (c == address || *(c - 1) == '.') return 0;
    
    /* next we validate the domain portion (name@domain) */
    if (!*(domain = ++c)) return 0;
    do {
        if (*c == '.') {
            if (c == domain || *(c - 1) == '.') return 0;
            count++;
        }
        if (*c <= ' ' || *c >= 127) return 0;
        if (strchr(rfc822_specials, *c)) return 0;
    } while (*++c);
    
    return (count >= 1);
}

void GetPresent::menuSendEmail(CCObject *pSender) {
    int i = 0;
    int j = 0;
    string email = m_pUserEmail->getText();
    if (!this->isValidEmail(email)) {
        CCFiniteTimeAction *showAction = CCFadeIn::create(0.5f);
        CCFiniteTimeAction *hideAction = CCFadeOut::create(2.0f);
        emailFailMsg->setVisible(true);
        emailFailMsg->runAction(CCSequence::create(showAction, hideAction, NULL));
    } else i = 1;
    
    if (strcmp(m_pUserName->getText(), "") == 0) {
        CCLOG("wrong username");
        CCFiniteTimeAction *showAction = CCFadeIn::create(0.5f);
        CCFiniteTimeAction *hideAction = CCFadeOut::create(2.0f);
        nameFailMsg->setVisible(true);
        nameFailMsg->runAction(CCSequence::create(showAction, hideAction, NULL));
    } else j = 1;
    
    if ((i * j * userOKName * userOKMail == 1) && isSent) {
        char *name =(char*) m_pUserName->getText();
        standardizeName(name);
        removeSpace(name);
        GameManager::sharedGameManager()->setName(name);
        string ipAddr = GameManager::sharedGameManager()->getIpAddr();
        string url = ipAddr +"/users.json";
        isSent = false;
        this->postUrl(url);
        this->sendMail();
    }else {
        if (userOKName == 0) {
            CCFiniteTimeAction *showAction = CCFadeIn::create(0.5f);
            CCFiniteTimeAction *hideAction = CCFadeOut::create(2.0f);
            nameExistedMsg->setVisible(true);
            nameExistedMsg->runAction(CCSequence::create(showAction, hideAction, NULL));
        }
        if (userOKMail == 0) {
            CCFiniteTimeAction *showAction = CCFadeIn::create(0.5f);
            CCFiniteTimeAction *hideAction = CCFadeOut::create(2.0f);
            emailExistedMsg->setVisible(true);
            emailExistedMsg->runAction(CCSequence::create(showAction, hideAction, NULL));
        }
    }
    
}
void GetPresent::menuBack(cocos2d::CCObject *pSender) {
    CCDirector::sharedDirector()->replaceScene(RankingScene::scene());
}
void GetPresent::insertChar(char *xau,int index)
{
    for(int i=strlen(xau)+1;i>index&&i>0;i--) xau[i]=xau[i-1];
    xau[index]=' ';
}
void GetPresent::standardizeName(char *xau)
{
    int i,j=0;
    for(i=0;i<strlen(xau);i++)
    {
        if (j==0&&strchr(",.\!;:?",xau[i])) continue;
        else if (i&&j&&strchr(",.\!;:?",xau[i-1])&&xau[i]!=' ') insertChar(xau,i);
        
        if (j&&strchr(",.\!;:?",xau[i])&&xau[j-1]==' ')  xau[j-1]=xau[i],xau[j]=' ';
        else if ((j==0&&xau[i]!=' ')||(j&&xau[j-1]==' '&&xau[i]!=' ')) xau[j++]=toupper(xau[i]);
        else if ((j&&xau[i]!=' ')||(j&&xau[i-1]!=' '&&xau[i]==' ')) xau[j++]=xau[i];
    }
    xau[j-1*(j&&xau[j-1]==' ')]=NULL;
}
void GetPresent::removeSpace(char *xau) {
    int len = 0;
    int i = 0;
    len=strlen(xau);
    for(i=0;i<len;i++)
    {
        if(xau[i] == ' ')
        {
            xau[i] = '_';
        }
    }
}

void GetPresent::sendInfo() {
    string ipAddr = GameManager::sharedGameManager()->getIpAddr();
    string url = ipAddr + "/users.json";
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if (curl) {
        //133.242.203.251
        //http://Pe4L60aeke:dhWLtJ8F1w@takasuapp.com
        
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_USERNAME, "Pe4L60aeke");
        curl_easy_setopt(curl, CURLOPT_PASSWORD, "dhWLtJ8F1w");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "account=kienbg");
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        //        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS ,1);
        curl_easy_setopt(curl, CURLOPT_POST, true);
        
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        
        if (res == 0) {
            CCLOG("0 response OK");
        } else {
            CCLog("code: %i",res);
        }
    }
}

bool GetPresent::isValidEmail(std::string email){
    int length = email.length();
    if (length >= 9 && ((email[0] > 64 && email[0] < 91) ||
                        (email[0] > 96 && email[0] < 123)) && email[length - 1] != '.') {
        int count1 = 0;
        int count2 = 0;
        int count3 = 0;
        int count4 = 0;
        for (int i = 0; i < length; i++) {
            if (email[i] == '@') {
                count3++;
            }
            if ((email[i] < 48 && email[i] != 46) || (email[i] > 57 && email[i] < 64) ||
                (email[i] >90 && email[i] < 97 && email[i] != 95) || (email[i] >122)) {
                count4++;
            }
            if (email[i] == '.') {
                if (i < email.find('@')) {
                    count1++;
                } else {
                    count2++;
                }
            }
        }
        if (count3 == 1 && count4 == 0 && count1 < 2 && count2 > 0 &&
            ( email.find('@') > 3 && email.find('@') < 32)) {
            return true;
        } else {
            return false;
        }
        
    } else {
        return false;
    }
    
}
#pragma mark CURL
void GetPresent::getData() {
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if (curl) {
        //133.242.203.251
        //http://Pe4L60aeke:dhWLtJ8F1w@takasuapp.com
        char * name =(char*) m_pUserName->getText();
        standardizeName(name);
        removeSpace(name);
        string ipAddr = GameManager::sharedGameManager()->getIpAddr();
        //        string url = ipAddr + "/users.json";
        string url = ipAddr+"/users.json";
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_USERNAME, "Pe4L60aeke");
        curl_easy_setopt(curl, CURLOPT_PASSWORD, "dhWLtJ8F1w");
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getPresentWriter);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &dataBuf);
        res = curl_easy_perform(curl);
        
        curl_easy_cleanup(curl);
        if (res == 0) {
            CCLOG("0 response OK\n");
        } else {
            CCLabelTTF *checkInternetMsg = CCLabelTTF::create("現在ランキングは閉じています", FONT, 30*SIZE_RATIO);
            checkInternetMsg->setPosition(ccp(w/2, h/2 - 30*SIZE_RATIO));
            checkInternetMsg->setColor(ccYELLOW);
            this->addChild(checkInternetMsg);
        }
    } else {
        CCLOG("no curl\n");
    }
}
void GetPresent::postUrl(string url) {
    string username = GameManager::sharedGameManager()->getName();
    if (!username.empty()) {
        CURL *curl;
        CURLcode res;
        curl = curl_easy_init();
        if (curl) {
            //133.242.203.251
            //http://Pe4L60aeke:dhWLtJ8F1w@takasuapp.com
            
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_USERNAME, "Pe4L60aeke");
            curl_easy_setopt(curl, CURLOPT_PASSWORD, "dhWLtJ8F1w");
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "account=kienbg");
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
            //        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS ,1);
            curl_easy_setopt(curl, CURLOPT_POST, true);
            
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            
            if (res == 0) {
                CCLOG("0 response OK");
            } else {
                CCLog("code: %i",res);
            }
        }
    }
}
void GetPresent::getDataName() {
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if (curl) {
        //133.242.203.251
        //http://Pe4L60aeke:dhWLtJ8F1w@takasuapp.com
        char * name =(char*) m_pUserName->getText();
        standardizeName(name);
        removeSpace(name);
        string ipAddr = GameManager::sharedGameManager()->getIpAddr();
        //        string url = ipAddr + "/users.json";
        string url = ipAddr+"/user3s/"+name+".json?name=ngocdu";
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_USERNAME, "Pe4L60aeke");
        curl_easy_setopt(curl, CURLOPT_PASSWORD, "dhWLtJ8F1w");
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getPresentWriter);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &dataBufName);
        res = curl_easy_perform(curl);
        
        curl_easy_cleanup(curl);
        if (res == 0) {
            CCLOG("0 response OK\n");
        } else {
            CCLabelTTF *checkInternetMsg = CCLabelTTF::create("現在ランキングは閉じています", FONT, 30*SIZE_RATIO);
            checkInternetMsg->setPosition(ccp(w/2, h/2 - 30*SIZE_RATIO));
            checkInternetMsg->setColor(ccYELLOW);
            this->addChild(checkInternetMsg);
        }
    } else {
        CCLOG("no curl\n");
    }
}
void GetPresent::checkName() {
    this->getDataName();
    string name = "";
    rapidjson::Document document;
    string data = "["+dataBufName+"]";
    if(dataBufName.c_str() != NULL && !document.Parse<0>(data.c_str()).HasParseError()) {
        for (rapidjson::SizeType  i = 0; i < document.Size(); i++) {
            if (document[i].IsObject()) {
                CCLOG("is object");
                name = document[i]["name"].GetString();
                if (name != "") {
                    userOK = 0;
                    userOKName = 0;
                    CCFiniteTimeAction *showAction = CCFadeIn::create(0.5f);
                    CCFiniteTimeAction *hideAction = CCFadeOut::create(2.0f);
                    nameExistedMsg->setVisible(true);
                    nameExistedMsg->runAction(CCSequence::create(showAction, hideAction, NULL));
                }
            }
        }
    } else {
        CCLog(document.GetParseError());
    }
    dataBufName = "";
    if (name == ""){
        userOKName = 1;
    }
}
void GetPresent::getDataEmail() {
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if (curl) {
        //133.242.203.251
        //http://Pe4L60aeke:dhWLtJ8F1w@takasuapp.com
        string email = m_pUserEmail->getText();
        string ipAddr = GameManager::sharedGameManager()->getIpAddr();
        //        string url = ipAddr + "/users.json";
        string url = ipAddr+"/user3s/dudu.json?email="+email;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_USERNAME, "Pe4L60aeke");
        curl_easy_setopt(curl, CURLOPT_PASSWORD, "dhWLtJ8F1w");
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getPresentWriter);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &dataBufEmail);
        res = curl_easy_perform(curl);
        
        curl_easy_cleanup(curl);
        if (res == 0) {
            CCLOG("0 response OK\n");
        } else {
            CCLabelTTF *checkInternetMsg = CCLabelTTF::create("現在ランキングは閉じています", FONT, 30*SIZE_RATIO);
            checkInternetMsg->setPosition(ccp(w/2, h/2 - 30*SIZE_RATIO));
            checkInternetMsg->setColor(ccYELLOW);
            this->addChild(checkInternetMsg);
        }
    } else {
        CCLOG("no curl\n");
    }
}
void GetPresent::checkEmail() {
    this->getDataEmail();
    string email = "";
    string data2 = "["+dataBufEmail+"]";
    rapidjson::Document document;
    if(dataBufEmail.c_str() != NULL && !document.Parse<0>(data2.c_str()).HasParseError()) {
        for (rapidjson::SizeType  i = 0; i < document.Size(); i++) {
            if (document[i].IsObject()) {
                email = document[i]["email"].GetString();
                if (email != "") {
                    userOK = 0;
                    userOKMail = 0;
                    CCFiniteTimeAction *showAction = CCFadeIn::create(0.5f);
                    CCFiniteTimeAction *hideAction = CCFadeOut::create(2.0f);
                    emailExistedMsg->setVisible(true);
                    emailExistedMsg->runAction(CCSequence::create(showAction, hideAction, NULL));
                }
            }
        }
    } else {
        CCLog(document.GetParseError());
    }
    if (email == "") {
        userOKMail = 1;
        GameManager::sharedGameManager()->setEmail(m_pUserEmail->getText());
    }
    dataBufEmail = "";
}
void GetPresent::sendMail() {
    rapidjson::Document document;
    if(dataBuf.c_str() != NULL && !document.Parse<0>(dataBuf.c_str()).HasParseError()) {
        for (rapidjson::SizeType  i = 0; i < document.Size(); i++) {
            string name = document[i]["name"].GetString();
            string email_server = document[i]["email"].GetString();
            CCLOG("%s", name.c_str());
            CCLOG("%s", m_pUserName->getText());
            char * n =(char*) m_pUserName->getText();
            standardizeName(n);
            removeSpace(n);
            CCLOG("text name : %s", n);
            if (strcmp(n, name.c_str()) == 0) {
                if(strcmp(m_pUserEmail->getText(), "") != 0 && strcmp(m_pUserEmail->getText(), email_server.c_str()) == 0 ){
                    CCFiniteTimeAction *showAction = CCFadeIn::create(0.2f);
                    CCFiniteTimeAction *hideAction = CCFadeOut::create(0.2f);
                    emailFailMsg->runAction(CCSequence::create(showAction, hideAction, NULL));
                    return;
                }
            }
        }
    } else {
        CCLog(document.GetParseError());
    }
    
    if (strcmp(m_pUserEmail->getText(), "") != 0 && this->isValidEmail(m_pUserEmail->getText()) &&
        this->spc_email_isvalid(m_pUserEmail->getText())) {
        string name = GameManager::sharedGameManager()->getName();
        int p = GameManager::sharedGameManager()->getPoint();
        char strP[20] = {0};
        sprintf(strP, "%i", p);
        string email  = GameManager::sharedGameManager()->getEmail();
        string ipAddr = GameManager::sharedGameManager()->getIpAddr();
        int reward    = GameManager::sharedGameManager()->getReward();
        string url;
        if (reward != 0) {
            url    = ipAddr + "/users?name="+name+"&point="+strP+"&email="+email;
        } else {
            url    = ipAddr + "/users?name="+name+"&point="+strP+"&email="+email;
        }
        this->postUrl(url);
        string url3 = ipAddr + "/user3s?name="+name+"&point="+strP+"&email="+email;
        this->postUrl(url3);
        char * n =(char*) m_pUserName->getText();
        standardizeName(n);
        GameManager::sharedGameManager()->setName(n);
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, RankingScene::scene()));
    } else {
        CCFiniteTimeAction *showAction = CCFadeIn::create(0.2f);
        CCFiniteTimeAction *hideAction = CCFadeOut::create(0.2f);
        emailFailMsg->runAction(CCSequence::create(showAction, hideAction, NULL));
    }
    dataBuf = "";
}