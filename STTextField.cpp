

#include "STTextField.h"

USING_NS_CC;

STTextField::STTextField() {}
STTextField::~STTextField() {}

STTextField* STTextField::create()
{
    return STTextField::create("[placeholder]", "fonts/arial.ttf", 20);
}

STTextField* STTextField::create(const std::string& placeholder, const std::string& fontName, int fontSize)
{
    STTextField* widget = new (std::nothrow) STTextField();
    if (widget && widget->init())
    {
        widget->setFontName(fontName);
        widget->setFontSize(fontSize);
        widget->setPlaceHolder(placeholder);
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

void STTextField::onEnter()
{
    TextField::onEnter();
    
    // create cursor label
    auto lbl = Label::createWithTTF(" | ", "fonts/arial.ttf", this->getFontSize());
    lbl->setColor(Color3B(0,0,0));
    if(! lbl) return;
    
    auto px = lbl->getBoundingBox().size.width;
    auto py = this->getBoundingBox().size.height * .5f + 5;
    log("%f %f",px ,py);
    lbl->setPosition(2, 14.5);
    
    auto seq = Sequence::create(FadeOut::create(0.5), FadeIn::create(0.25), nullptr);
    lbl->runAction(RepeatForever::create(seq));
    lbl->setTag(100);
    this->addChild(lbl,10);
    
    _cursorLabel = lbl;
    
    _cursorLabel->setVisible(false);
    
    
    this->addEventListener([this](Ref* ref, EventType etype) {
        if(etype == EventType::ATTACH_WITH_IME) {
            // show cursor
            _cursorLabel->setVisible(true);
        }
        else if(etype == EventType::DETACH_WITH_IME) {
            // hide cursor
            _cursorLabel->setVisible(false);
        }
        else if(etype == EventType::INSERT_TEXT) {
            // move right (or just update)
            updateCursor();
        }
        else if(etype == EventType::DELETE_BACKWARD) {
            // move left (or just update)
            updateCursor();
        }
    });
}

void STTextField::onExit()
{
    this->addEventListener(nullptr);
    if(_cursorLabel) {
        _cursorLabel->removeFromParent();
        _cursorLabel = nullptr;
    }
    TextField::onExit();
}

void STTextField::updateCursor()
{
    auto lbl = dynamic_cast<Label*>(getVirtualRenderer());
    if(! lbl) return;
    
    // TODO: could just use bbox length
    auto px = lbl->getBoundingBox().size.width + 2.f;
    auto py = this->getBoundingBox().size.height * .5f + 3;
    log("%f %f",px ,py);
    
    _cursorLabel->setPosition(px, py);
    
    return;
    
    //    // Could also find position based on a specific character (if insert text in middle were possible)
    
    //    // Here's getting specific LAST character
    //    // get current length
    //    auto len = lbl->getStringLength();
    //    if(len <= 0) return;
    //
    //    auto letter = lbl->getLetter(len - 1);
    //    auto p = letter->getPosition(); // may need bbox
    //    auto px = p.x + letter->getBoundingBox().size.width * .5f;
    //    auto py = this->getBoundingBox().size.height * .5f;
    //    _cursorLabel->setPosition(px, py);
}