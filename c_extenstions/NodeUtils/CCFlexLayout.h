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

class CCFlexLayout {
private:
    cocos2d::Node* _cocosNode;  // Cocos2d-x Node
    YGNodeRef _ygNode;
    
    //_rootNode는 std::shared_ptr로 관리되므로, 직접 해제하지 않아도 됩니다. std::shared_ptr는 소유권을 관리하며, 참조 카운트가 0이 되는 순간 자동으로 메모리를 해제합니다.
    std::vector<std::shared_ptr<CCFlexLayout>> _children;
    float _limitWidth = 0.0f;
    std::string tagName_ = "";
    
    float marginLeft_ = 0.0;
    float marginRight_ = 0.0;
    float marginTop_ = 0.0;
    float marginBottom_ = 0.0;
public:
    bool isSizeToFit = false;
    
    float virtualWidth = 0.0f;
    float virtualHeight = 0.0f;
    
    CCFlexLayout() {
        _ygNode = YGNodeNew();
    }
    CCFlexLayout(cocos2d::Node* node) : _cocosNode(_cocosNode) {
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
    
    ~CCFlexLayout() {
        if (_ygNode) {
            YGNodeFree(_ygNode);
            _ygNode = nullptr;
        }
        /*
        // Cocos2d-x 노드 해제 (스마트 포인터가 아닐 경우)
        if (_cocosNode) {
            _cocosNode->removeFromParent();
            _cocosNode = nullptr;
        }
         */
    }
    
    static std::shared_ptr<CCFlexLayout> create(cocos2d::Node* node);
    
    YGNodeRef getNode() const;
    std::vector<std::shared_ptr<CCFlexLayout>> getNodChilds() const ;
    
    CCFlexLayout& setTagName(std::string tagNAme);
    
    float getPositionY();
    CCFlexLayout& setPositionY(float poxY);
    CCFlexLayout& setPosition(float posX , float poxY);
    CCFlexLayout& setContentSize(const cocos2d::Size& contentSize);
    cocos2d::Size getContentSize(const cocos2d::Size& contentSize);
    
    float getContentWidth();
    float getContentHeight();
    
    CCFlexLayout& setPadding(YGEdge edgeType , float edgeValue);
    
    CCFlexLayout& setMargin(YGEdge edgeType , float edgeValue);
    
    CCFlexLayout& setLimitWidth(float width);
    float getLimitWidth();
    
    CCFlexLayout& setWidth(float width);
    
    CCFlexLayout& setHeight(float height);
    
    CCFlexLayout& setFlexDirection(YGFlexDirection direction);
    
    CCFlexLayout& setJustifyContent(YGJustify justify);
    
    CCFlexLayout& setAlignItems(YGAlign align);
    
    CCFlexLayout& setAlignSelf(YGAlign align);
    
    CCFlexLayout& setAlignContent(YGAlign align);
    
    cocos2d::Node* getCocosNode();
    
    CCFlexLayout& addChild(const std::shared_ptr<CCFlexLayout>& child);
    
    // 람다를 활용해 하위 노드 구성
    CCFlexLayout& define(const std::function<void(CCFlexLayout&)>& defineFunc);
    
    // FlexDirection 가져오기
    YGFlexDirection getFlexDirection() const ;
    
    // FlexWrap 설정
    CCFlexLayout& setFlexWrap(YGWrap wrap);
    
    // FlexWrap 가져오기
    YGWrap getFlexWrap() const;
    
    // 너비 가져오기
    float getWidth() const;
    
    float getParentWidth();
    
    bool isRow();
    
    // 높이 가져오기
    float getHeight() const ;
    
    float getLeftRightMargin();
    float getTopBottomMargin();
    
    CCFlexLayout& setBackGroundColor(const cocos2d::Color3B &color);
    
    YGNodeRef getFirstChild(YGNodeRef parentNode);
    
    YGNodeRef getLastChild(YGNodeRef parentNode);
    
    void layoutDrawForSub(float width = YGUndefined, float height = YGUndefined, CCFlexLayout* refFlexNode = nullptr);
    
    void layoutDrawForMain(float width = YGUndefined, float height = YGUndefined, CCFlexLayout* refFlexNode = nullptr);
    
    void layoutDraw(float width = YGUndefined, float height = YGUndefined, CCFlexLayout* refFlexNode = nullptr , bool isMainLayout = true);
    
    void removeAllCocosNode();
    
};




#endif /* FlexNode_hpp */
