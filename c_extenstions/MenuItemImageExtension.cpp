//
//  MenuItemImageExtension.cpp
//  libcocos2d Mac
//
//  Created by 선명수 on 11/7/24.
//



#include "MenuItemImageExtension.h"


using namespace cocos2d;

MenuItemImageExtension* MenuItemImageExtension::createWithSpriteFrameName(const std::string& normalFrameName,
                                                                          const std::string& selectedFrameName,
                                                                          const ccMenuCallback& callback)
{
    auto menuItem = new (std::nothrow) MenuItemImageExtension();
    if (menuItem && menuItem->initWithNormalSprite(Sprite::createWithSpriteFrameName(normalFrameName),
                                                   Sprite::createWithSpriteFrameName(selectedFrameName),
                                                   nullptr, callback))
    {
        menuItem->autorelease();
        return menuItem;
    }
    CC_SAFE_DELETE(menuItem);
    return nullptr;
}
