//
//  FlexNode.hpp
//  cocos2d_libs
//
//  Created by 선명수 on 11/20/24.
//

#ifndef FlexNode_hpp
#define FlexNode_hpp

#include <functional>
#include <iostream>
#include <vector>
#include <memory>
#include <yoga/Yoga.h>
#include <cocos2d.h>

class FlexNode {
private:
    
    cocos2d::Node* _cocosNode;  // Cocos2d-x Node
    YGNodeRef _ygNode;
    std::vector<std::shared_ptr<FlexNode>> _children;
    
public:
    FlexNode() {
        _ygNode = YGNodeNew();
    }
    FlexNode(cocos2d::Node* node) : _cocosNode(_cocosNode) {
        _ygNode = YGNodeNew();
        // Node에 대응되는 YGNode 설정
        if (!_ygNode) {
            throw std::runtime_error("Failed to create Yoga node.");
        }
        
        if (node != nullptr){
            _cocosNode = node;
            _cocosNode->setAnchorPoint(cocos2d::Vec2(0, 0));
            auto tSize = _cocosNode->getContentSize();
            this->setWidth(tSize.width);
            this->setHeight(tSize.height);
        }
    }
    
    ~FlexNode() {
        YGNodeFree(_ygNode);
    }
    
    YGNodeRef getNode() const {
        return _ygNode;
    }
    std::vector<std::shared_ptr<FlexNode>> getNodChilds() const {
        return _children;
    }
    
    
    
    float getContentWidth() {
        return YGNodeStyleGetWidth(_ygNode).value;
    }
    float getContentHeight() {
        return YGNodeStyleGetHeight(_ygNode).value;
    }
    
    FlexNode& setPadding(YGEdge edgeType , float edgeValue) {
        YGNodeStyleSetPadding(_ygNode,edgeType,edgeValue);
        return *this;
    }
    
    FlexNode& setMargin(YGEdge edgeType , float edgeValue) {
        YGNodeStyleSetMargin(_ygNode,edgeType,edgeValue);
        return *this;
    }
    
    FlexNode& setWidth(float width) {
        YGNodeStyleSetWidth(_ygNode, width);
        if (_cocosNode != nullptr){
            _cocosNode->setContentSize(cocos2d::Size(width,_cocosNode->getContentSize().height));
        }
        return *this;
    }
    
    FlexNode& setHeight(float height) {
        YGNodeStyleSetHeight(_ygNode, height);
        if (_cocosNode != nullptr){
            _cocosNode->setContentSize(cocos2d::Size(_cocosNode->getContentSize().width,height));
        }
        return *this;
    }
    
    FlexNode& setFlexDirection(YGFlexDirection direction) {
        YGNodeStyleSetFlexDirection(_ygNode, direction);
        return *this;
    }
    
    FlexNode& setJustifyContent(YGJustify justify) {
        YGNodeStyleSetJustifyContent(_ygNode, justify);
        return *this;
    }
    
    FlexNode& setAlignItems(YGAlign align) {
        YGNodeStyleSetAlignItems(_ygNode, align);
        return *this;
    }
    
    FlexNode& setAlignSelf(YGAlign align) {
        YGNodeStyleSetAlignSelf(_ygNode, align);
        return *this;
    }
    
    FlexNode& setAlignContent(YGAlign align) {
        YGNodeStyleSetAlignContent(_ygNode, align);
        return *this;
    }
    
    cocos2d::Node* getCocosNode() {
        return _cocosNode;
    }
    
    FlexNode& addChild(const std::shared_ptr<FlexNode>& child) {
        YGNodeInsertChild(_ygNode, child->getNode(), YGNodeGetChildCount(_ygNode));
        _children.push_back(child);
        _cocosNode->addChild(child->getCocosNode());
        return *this;
    }
    
    // 람다를 활용해 하위 노드 구성
    FlexNode& define(const std::function<void(FlexNode&)>& defineFunc) {
        defineFunc(*this);
        return *this;
    }
    
    // FlexDirection 가져오기
    YGFlexDirection getFlexDirection() const {
        return YGNodeStyleGetFlexDirection(_ygNode);
    }
    
    // FlexWrap 설정
    FlexNode& setFlexWrap(YGWrap wrap) {
        YGNodeStyleSetFlexWrap(_ygNode, wrap);
        return *this;
    }
    
    // FlexWrap 가져오기
    YGWrap getFlexWrap() const {
        return YGNodeStyleGetFlexWrap(_ygNode);
    }
    
    // 너비 가져오기
    float getWidth() const {
        return YGNodeStyleGetWidth(_ygNode).value;
    }
    
    // 높이 가져오기
    float getHeight() const {
        return YGNodeStyleGetHeight(_ygNode).value;
    }
    
    void layout(float width, float height) {
        
        //YGNodeCalculateLayout(_ygNode, width, height, YGDirectionLTR);
        YGNodeCalculateLayout(_ygNode, width, YGUndefined, YGDirectionLTR);
        
        auto directionType = this->getFlexDirection();
        
        for (uint32_t i = 0; i < YGNodeGetChildCount(_ygNode); i++) {
            auto tNode = _children.at(i);
            tNode->layout(width, height);
        }
        // 레이아웃 계산

        // 계산된 레이아웃 값에서 위치 및 크기 정보 추출
        float x = YGNodeLayoutGetLeft(_ygNode);
        float y = YGNodeLayoutGetTop(_ygNode);
        float w = YGNodeLayoutGetWidth(_ygNode);
        float h = YGNodeLayoutGetHeight(_ygNode);
        
 
//        if (directionType == YGFlexDirectionRow || directionType == YGFlexDirectionRowReverse){
//            
//        }
//        if (y < 0){
//            y = y - (h/2.0);
//        }
        
        // Cocos2d-x Node에 적용
        if (_cocosNode != nullptr){
            _cocosNode->setPosition(cocos2d::Vec2(x, y));
            _cocosNode->setContentSize(cocos2d::Size(w, h));
            
            this->setWidth(w);
            this->setHeight(h);
        }
        
        CCLOG("cocos Child %d: x = %.2f, y = %.2f, width = %.2f, height = %.2f", 0, x, y, w, h);
        
        /*
         for (uint32_t i = 0; i < YGNodeGetChildCount(_ygNode); i++) {
         
         //tNode->layout(width, height);
         auto tNode = _children.at(i);
         auto tCocodNode = tNode->getCocosNode();
         
         YGNodeRef yogaChild = YGNodeGetChild(_ygNode, i);
         // 계산된 레이아웃 값에서 위치 및 크기 정보 추출
         float x = YGNodeLayoutGetLeft(yogaChild);
         float y = YGNodeLayoutGetTop(yogaChild);
         float w = YGNodeLayoutGetWidth(yogaChild);
         float h = YGNodeLayoutGetHeight(yogaChild);
         // CCLOG로 값 출력
         CCLOG("Child %d: x = %.2f, y = %.2f, width = %.2f, height = %.2f", i, x, y, w, h);
         
         
         if (tCocodNode != nullptr){
         tCocodNode->setPosition(cocos2d::Vec2(x, y));
         tCocodNode->setContentSize(cocos2d::Size(w, h));
         }
         }
         */
        
        
        
        
        
    }
};




#endif /* FlexNode_hpp */
