
#ifndef STTextField_h
#define STTextField_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class STTextField : public cocos2d::ui::TextField
{
public:
    STTextField();
    virtual ~STTextField();
    
    static STTextField* create();
    static STTextField* create(const std::string& placeholder,
                               const std::string& fontName,
                               int fontSize);
    
    // TODO: possibly need touch handlers (override or new)
    virtual void onEnter();
    virtual void onExit();
    
    void updateCursor();
    
protected:
    cocos2d::Label* _cursorLabel;
};

#endif