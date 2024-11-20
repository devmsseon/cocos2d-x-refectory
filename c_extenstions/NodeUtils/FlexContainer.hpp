//
//  FlexContainer.hpp
//  cocos2d_libs
//
//  Created by 선명수 on 11/20/24.
//

#ifndef FlexContainer_hpp
#define FlexContainer_hpp

#include <stdio.h>
#include "FlexNode.hpp"



#include <functional>
#include <iostream>
#include <vector>
#include <memory>
#include <yoga/Yoga.h>
#include "cocos2d.h"
// FlexContainer 클래스
class FlexContainer {
private:
    std::shared_ptr<FlexNode> _rootNode;
   // std::vector<FlexNode*> _children;   // 자식 FlexNode들
    std::vector<std::shared_ptr<FlexNode>> _children;
public:
    FlexContainer() {
        auto tNode = cocos2d::Node::create();
        tNode->setAnchorPoint(cocos2d::Vec2(0, 0));
        _rootNode = std::make_shared<FlexNode>(tNode);
    }
    
    void setWidth(float width) {
        _rootNode->setWidth(width);
    }
    
    void setHeight(float height) {
        _rootNode->setHeight(height);
    }
    
    std::shared_ptr<FlexNode> getRootNode() const {
        return _rootNode;
    }
    
    cocos2d::Node* getCocosNode() {
        return _rootNode->getCocosNode();
    }
    
    FlexContainer& setPadding(YGEdge edgeType , float edgeValue) {
        _rootNode->setPadding(edgeType, edgeValue);
        return *this;
    }
    
    FlexContainer& setMargin(YGEdge edgeType , float edgeValue) {
        _rootNode->setMargin(edgeType, edgeType);
        return *this;
    }
    
    FlexContainer& setAlignItems(YGAlign align) {
        _rootNode->setAlignItems(align);
        return *this;
    }
    
    FlexContainer& setAlignSelf(YGAlign align) {
        _rootNode->setAlignSelf(align);
        return *this;
    }
    
    FlexContainer& setAlignContent(YGAlign align) {
        _rootNode->setAlignContent(align);
        return *this;
    }
    
    FlexContainer& setFlexDirection(YGFlexDirection direction = YGFlexDirectionColumn) {
        _rootNode->setFlexDirection(direction);
        return *this;
    }
    FlexContainer& setFlexWrap(YGWrap wrap) {
        _rootNode->setFlexWrap(wrap);
        return *this;
    }
    
    
    FlexContainer& addChild(const std::shared_ptr<FlexNode>& child) {
      _rootNode->addChild(child);
        /*
        YGNodeInsertChild(_rootNode->getNode(), child->getNode(), YGNodeGetChildCount(_rootNode->getNode()));
        _children.push_back(child);
         */
        return *this;
    }
    
    // 람다를 활용한 하위 구성 정의
    FlexContainer& define(const std::function<void(FlexNode&)>& defineFunc) {
        _rootNode->define(defineFunc);
        return *this;
    }
    
    float getContentWidth() {
        return _rootNode->getContentWidth();
    }
    float getContentHeight() {
        return _rootNode->getContentHeight();
    }
    
    void layout(float width, float height) {
        // FlexContainer의 레이아웃을 계산하고 자식들에게 적용
        _rootNode->layout(width, height);
        /*
        for (auto& child : _children) {
            child->layout(width, height);  // 각 자식에 대해 레이아웃 계산
        }
         */
    }
    
    /*
     void printLayout() const {
     auto printNode = [](YGNodeRef node, int depth = 0) {
     for (int i = 0; i < depth; ++i) std::cout << "  ";
     std::cout << "Node: {"
     << "x: " << YGNodeLayoutGetLeft(node)
     << ", y: " << YGNodeLayoutGetTop(node)
     << ", width: " << YGNodeLayoutGetWidth(node)
     << ", height: " << YGNodeLayoutGetHeight(node)
     << "}\n";
     for (uint32_t i = 0; i < YGNodeGetChildCount(node); ++i) {
     printNode(YGNodeGetChild(node, i), depth + 1);
     }
     };
     
     printNode(_rootNode->getNode());
     }
     */
};

#endif /* FlexContainer_hpp */


/*
 #include <iostream>
 
 int main() {
 FlexContainer container;
 
 // 람다식으로 구성 정의
 container.setFlexDirection(YGFlexDirectionColumn)
 .define([](FlexNode& root) {
 // 첫 번째 자식
 root.addChild(std::make_shared<FlexNode>()->setWidth(100).setHeight(50));
 
 // 두 번째 자식 (내부에서도 람다식 사용 가능)
 root.addChild(std::make_shared<FlexNode>()->setWidth(150).setHeight(75)
 ->define([](FlexNode& child) {
 child.addChild(std::make_shared<FlexNode>()->setWidth(50).setHeight(25));
 child.addChild(std::make_shared<FlexNode>()->setWidth(75).setHeight(25));
 }));
 });
 
 // 레이아웃 계산
 container.layout(300, YGUndefined);
 
 // 결과 출력
 container.printLayout();
 
 return 0;
 }
 */
