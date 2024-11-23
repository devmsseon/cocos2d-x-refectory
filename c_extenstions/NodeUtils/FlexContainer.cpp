//
//  FlexContainer.cpp
//  cocos2d_libs
//
//  Created by 선명수 on 11/20/24.
//


#include "FlexContainer.h"


// FlexContainer 생성자
FlexContainer::FlexContainer() {
    auto tNode = cocos2d::Node::create();
    tNode->setAnchorPoint(cocos2d::Vec2(0, 0));
    _rootNode = std::make_shared<FlexNode>(tNode);

    // 지금은 어쩔 수 없음. 일단 크게 잡아놔야 Flex 시스템 자체에서 계산함.
    // 안 그러면 -좌표값이 나와 일일이 계산해야 되는데 엄청 복잡함.
}

FlexContainer::~FlexContainer() {
    /*일단 이슈 있음 
    if (_rootNode != nullptr){
        _rootNode->removeAllCocosNode();
    }
     */
}

// 너비 설정
void FlexContainer::setWidth(float width) {
    _rootNode->setWidth(width);
}

// 높이 설정
void FlexContainer::setHeight(float height) {
    _rootNode->setHeight(height);
}

// RootNode 반환
std::shared_ptr<FlexNode> FlexContainer::getRootNode() const {
    return _rootNode;
}

// CocosNode 반환
cocos2d::Node* FlexContainer::getCocosNode() {
    return _rootNode->getCocosNode();
}

// Padding 설정
FlexContainer& FlexContainer::setPadding(YGEdge edgeType, float edgeValue) {
    _rootNode->setPadding(edgeType, edgeValue);
    return *this;
}

// Margin 설정
FlexContainer& FlexContainer::setMargin(YGEdge edgeType, float edgeValue) {
    _rootNode->setMargin(edgeType, edgeValue);
    return *this;
}

// AlignItems 설정
FlexContainer& FlexContainer::setAlignItems(YGAlign align) {
    _rootNode->setAlignItems(align);
    return *this;
}

// AlignSelf 설정
FlexContainer& FlexContainer::setAlignSelf(YGAlign align) {
    _rootNode->setAlignSelf(align);
    return *this;
}

// AlignContent 설정
FlexContainer& FlexContainer::setAlignContent(YGAlign align) {
    _rootNode->setAlignContent(align);
    return *this;
}

// JustifyContent 설정
FlexContainer& FlexContainer::setJustifyContent(YGJustify justify) {
    _rootNode->setJustifyContent(justify);
    return *this;
}

// FlexDirection 설정
FlexContainer& FlexContainer::setFlexDirection(YGFlexDirection direction) {
    _rootNode->setFlexDirection(direction);
    return *this;
}

// FlexWrap 설정
FlexContainer& FlexContainer::setFlexWrap(YGWrap wrap) {
    _rootNode->setFlexWrap(wrap);
    return *this;
}

// 자식 노드 추가
FlexContainer& FlexContainer::addChild(const std::shared_ptr<FlexNode>& child) {
    _rootNode->addChild(child);
    return *this;
}

// 람다를 활용한 하위 구성 정의
FlexContainer& FlexContainer::define(const std::function<void(FlexNode&)>& defineFunc) {
    _rootNode->define(defineFunc);
    return *this;
}

// Content Width 반환
float FlexContainer::getContentWidth() {
    return _rootNode->getContentWidth();
}

// Content Height 반환
float FlexContainer::getContentHeight() {
    return _rootNode->getContentHeight();
}

// 메인 레이아웃 계산
void FlexContainer::layoutDrawForMain(float width, float height, FlexNode* refFlexNode) {
    _rootNode->layoutDrawForMain(width, height, _rootNode.get());
}

// 서브 레이아웃 계산
void FlexContainer::layoutDrawForSub(float width, float height, FlexNode* refFlexNode) {
    _rootNode->layoutDrawForSub(width, height, _rootNode.get());
}
