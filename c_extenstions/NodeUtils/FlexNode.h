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
    float _limitWidth = 0.0f;
    std::string tagName_ = "";
public:
    bool isSizeToFit = false;
    
    float virtualWidth = 0.0f;
    float virtualHeight = 0.0f;
    
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
    
    static std::shared_ptr<FlexNode> create(cocos2d::Node* node);
    
    YGNodeRef getNode() const;
    std::vector<std::shared_ptr<FlexNode>> getNodChilds() const ;
    
    FlexNode& setTagName(std::string tagNAme);
    FlexNode& setContentSize(const cocos2d::Size& contentSize);
    cocos2d::Size getContentSize(const cocos2d::Size& contentSize);
    
    float getContentWidth();
    float getContentHeight();
    
    FlexNode& setPadding(YGEdge edgeType , float edgeValue);
    
    FlexNode& setMargin(YGEdge edgeType , float edgeValue);
    
    FlexNode& setLimitWidth(float width);
    float getLimitWidth();
    
    FlexNode& setWidth(float width);
    
    FlexNode& setHeight(float height);
    
    FlexNode& setFlexDirection(YGFlexDirection direction);
    
    FlexNode& setJustifyContent(YGJustify justify);
    
    FlexNode& setAlignItems(YGAlign align);
    
    FlexNode& setAlignSelf(YGAlign align);
    
    FlexNode& setAlignContent(YGAlign align);
    
    cocos2d::Node* getCocosNode();
    
    FlexNode& addChild(const std::shared_ptr<FlexNode>& child);
    
    // 람다를 활용해 하위 노드 구성
    FlexNode& define(const std::function<void(FlexNode&)>& defineFunc);
    
    // FlexDirection 가져오기
    YGFlexDirection getFlexDirection() const ;
    
    // FlexWrap 설정
    FlexNode& setFlexWrap(YGWrap wrap);
    
    // FlexWrap 가져오기
    YGWrap getFlexWrap() const;
    
    // 너비 가져오기
    float getWidth() const;
    
    float getParentWidth();
    
    bool isRow();
    
    // 높이 가져오기
    float getHeight() const ;
    
    YGNodeRef getFirstChild(YGNodeRef parentNode);
    
    YGNodeRef getLastChild(YGNodeRef parentNode);
    
    void layoutDrawForSub(float width = YGUndefined, float height = YGUndefined, FlexNode* refFlexNode = nullptr);
    
    void layoutDrawForMain(float width = YGUndefined, float height = YGUndefined, FlexNode* refFlexNode = nullptr);
    
    void layoutDraw(float width = YGUndefined, float height = YGUndefined, FlexNode* refFlexNode = nullptr , bool isMainLayout = true);
    
    void removeAllCocosNode();
    
};



#endif /* FlexNode_hpp */
