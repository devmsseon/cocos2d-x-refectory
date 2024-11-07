//
//  MenuItemImageExtension.hpp
//  libcocos2d Mac
//
//  Created by 선명수 on 11/7/24.
//

#ifndef __MENU_ITEM_IMAGE_EXTENSION_H__
#define __MENU_ITEM_IMAGE_EXTENSION_H__

#include "cocos2d.h"

class MenuItemImageExtension : public cocos2d::MenuItemImage
{
public:
    static MenuItemImageExtension* createWithSpriteFrameName(const std::string& normalFrameName,
                                                             const std::string& selectedFrameName,
                                                             const cocos2d::ccMenuCallback& callback);
};

#endif // __MENU_ITEM_IMAGE_EXTENSION_H__
