//
//  NodeExtension.cpp
//  cocos2d_libs
//
//  Created by 선명수 on 11/7/24.
//
#include "NodeUtils.h"

namespace NodeUtils {


    float right(Node* node) {
        // getBoundingBox()를 사용하여 노드의 실제 위치와 크기를 얻어옴
        Rect rect = node->getBoundingBox();
        return rect.getMaxX() ; // 오른쪽 끝
    }

    float left(Node* node) {
        Rect rect = node->getBoundingBox();
        return rect.getMinX(); // 오른쪽 끝
    }

    float top(Node* node) {
        Rect rect = node->getBoundingBox();
        return rect.getMaxY(); // 오른쪽 끝
    }

    float bottom(Node* node) {
        Rect rect = node->getBoundingBox();
        return rect.getMinY(); // 아래쪽 끝
    }

    // 부모 노드 크기에 맞게 자식 노드 크기 조정
    void setSizeToParent(Node* childNode, Node* parentNode) {
        Size parentSize = parentNode->getContentSize();
        Size childSize = childNode->getContentSize();

        float scaleX = parentSize.width / childSize.width;
        float scaleY = parentSize.height / childSize.height;
        //float scale = std::max(scaleX, scaleY);  // 동일 비율로 크기 조정
        childNode->setScale(scaleX, scaleY);
        
        childNode->setPosition(parentSize.width / 2, parentSize.height / 2);  // 부모 노드 중앙에 위치
    }
}
