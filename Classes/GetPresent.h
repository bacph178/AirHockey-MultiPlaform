//
//  GetPresent.h
//  AirHockey
//
//

#ifndef AirHockey_GetPresent_h
#define AirHockey_GetPresent_h


#include "AppMacros.h"
#include "cocos-ext.h"
#include "Difficulty.h"
#include "GameManager.h"
#include "GameLayer.h"
#include "RankingScene.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "HttpClient.h"

using namespace cocos2d;
using namespace cocos2d::extension;

class GetPresent : public CCLayer, public extension::CCEditBoxDelegate{
private:
    CCSize size;

    int userOK;
    int userOKName;
    int userOKMail;

    float w, h;
    float SIZE_RATIO;
    float SIZE_RATIO_X;
    float SIZE_RATIO_Y;

    bool isSent;

    CCLabelTTF *emailFailMsg;
    CCLabelTTF *emailExistedMsg;
    CCLabelTTF *nameFailMsg;
    CCLabelTTF *nameExistedMsg;
    extension::CCEditBox* m_pUserEmail;
    extension::CCEditBox* m_pUserName;
    CCLabelTTF* m_pTTFShowEditReturn;
    CCMenu* pMenu;
    
    string dataBufName;
    string dataBufEmail;
    string dataBuf;
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static CCScene* scene();
    void toExtensionsMainLayer(CCObject *sender);
    virtual void editBoxEditingDidBegin(extension::CCEditBox* editBox);
    virtual void editBoxEditingDidEnd(extension::CCEditBox* editBox);
    virtual void editBoxTextChanged(extension::CCEditBox* editBox,
                                    const std::string& text);
    virtual void editBoxReturn(extension::CCEditBox* editBox);
    void menuSendEmail(CCObject *pSender);
    void menuBack(CCObject *pSender);
    
    bool is_email(std::string const& address);
    bool isValidEmail(std::string str);
    int spc_email_isvalid(const char *address);
    
    void sendInfo();
    
    void insertChar(char *xau,int index);
    void standardizeName(char *xau);
    void removeSpace(char *xau);
    //----------------curl----------------
    void checkName();
    void checkEmail();
    void getDataName();
    void getDataEmail();
    void getData();
    void sendMail();
    void postUrl(string url);
    //------------------------------------
    // a selector callback
    CREATE_FUNC(GetPresent);
};
#endif