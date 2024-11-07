//
//  NodeExtension.hpp
//  cocos2d_libs
//
//  Created by 선명수 on 11/7/24.
//

#ifndef __NODE_UTILS_H__
#define __NODE_UTILS_H__

#include "cocos2d.h"

USING_NS_CC;

namespace NodeUtils {
    // 오른쪽 끝 (Right)
    float right(Node* node);
    
    // 왼쪽 끝 (Left)
    float left(Node* node);
    
    // 위쪽 끝 (Top)
    float top(Node* node);
    
    // 아래쪽 끝 (Bottom)
    float bottom(Node* node);
    
    void setSizeToParent(Node* childNode, Node* parentNode);
}

#endif // __NODE_UTILS_H__

