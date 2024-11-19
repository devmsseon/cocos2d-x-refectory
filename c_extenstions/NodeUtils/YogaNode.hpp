#ifndef YogaNode_hpp
#define YogaNode_hpp

#include "cocos2d.h"
#include "c_extenstions/yoga/Yoga.h"

class YogaNode{
public:
    std::vector<YogaNode*> yogaNodes;
    
    //CREATE_FUNC(YogaNode);
    cocos2d::Node* _node;  // Cocos2d Node 객체를 저장
    
    
    void setTargetNode(cocos2d::Node* node) {
        cocos2d::Size size = node->getContentSize();
        
        node->setContentSize(size);
        // YogaNode의 크기 설정
        setWidth(size.width);
        setHeight(size.height);
        
        // setContentSize로 Cocos2d Node의 크기 적용
        // node->setContentSize(size);
        
        _node = node;
    }
    //
    // 생성자: cocos2d::Node* node 인수를 추가
    YogaNode(cocos2d::Node* node = nullptr)
    : _node(node), _yogaNode(YGNodeNew()) {
        if (_node) {
            setTargetNode(_node);
        }
    }
    
    // 소멸자: 메모리 해제 및 자식 노드들의 메모리 해제 처리
    ~YogaNode() {
        // 자식 YogaNode들에 대한 메모리 해제
        for (auto childNode : yogaNodes) {
            delete childNode;  // 자식 노드 삭제
        }
        yogaNodes.clear();      // 자식 노드 벡터 비우기
        
        // Yoga 노드 메모리 해제
        if (_yogaNode) {
            YGNodeFree(_yogaNode);
            _yogaNode = nullptr;
        }
        
        // cocos2d::Node 객체는 외부에서 관리하는 경우가 많으므로 여기서는 삭제하지 않음
        // (필요하다면 외부에서 할당 및 해제를 관리)
    }
    
    void addChild(YogaNode* child) {
        if (_node == nullptr){
            return;
        }
        
        auto yogaChild = dynamic_cast<YogaNode*>(child);
        if (yogaChild) {
            _node->addChild(yogaChild->_node);
            yogaNodes.push_back(yogaChild);
            YGNodeInsertChild(_yogaNode, yogaChild->_yogaNode, _yogaChildrenCount++);
        }
    }
    //
    virtual void setContentSize(const cocos2d::Size& contentSize){
        if (_node == nullptr){
            return;
        }
        _node->setContentSize(contentSize);
    }
    virtual void setPosition(const cocos2d::Vec2 &position){
        if (_node == nullptr){
            return;
        }
        _node->setPosition(position);
    }
    
    void setFlexWrap(YGWrap wrap) {
        YGNodeStyleSetFlexWrap(_yogaNode, wrap);
    }
    
    void setFlexDirection(YGFlexDirection direction) {
        YGNodeStyleSetFlexDirection(_yogaNode, direction);
    }
    
    void setJustifyContent(YGJustify justify) {
        YGNodeStyleSetJustifyContent(_yogaNode, justify);
    }
    
    void setLeftMargin(float margin) {
        YGNodeStyleSetMargin(_yogaNode, YGEdgeLeft,margin); // 왼쪽 마진 10.0f 설정
    }
    
    void setRightMargin(float margin) {
        YGNodeStyleSetMargin(_yogaNode, YGEdgeRight,margin); // 왼쪽 마진 10.0f 설정
    }
    
    void setTopMargin(float margin) {
        YGNodeStyleSetMargin(_yogaNode, YGEdgeTop, margin); // 왼쪽 마진 10.0f 설정
    }
    
    void setAlignItems(YGAlign align) {
        YGNodeStyleSetAlignItems(_yogaNode, align);
    }
    
    void setWidth(float width) {
        YGNodeStyleSetWidth(_yogaNode, width);
    }
    
    void setHeight(float height) {
        YGNodeStyleSetHeight(_yogaNode, height);
    }
    
    YGAlign getAlignItemsState() {
        return YGNodeStyleGetAlignItems(_yogaNode);
    }
    
    void doLayout() {
        if (_node == nullptr){
            return;
        }
        
        YGNodeCalculateLayout(_yogaNode, YGUndefined, YGUndefined, YGDirectionLTR);
        
        float maxChildWidth = 0.0f;
        float maxChildHeight = 0.0f;
        
        
        for (uint32_t i = 0; i < YGNodeGetChildCount(_yogaNode); i++) {
            YGNodeRef yogaChild = YGNodeGetChild(_yogaNode, i);
            
            if (yogaChild) {
                
                YGNodeCalculateLayout(yogaChild, YGUndefined, YGUndefined, YGDirectionLTR);
                auto childWidth = YGNodeLayoutGetWidth(yogaChild);
                auto childHeight = YGNodeLayoutGetHeight(yogaChild);
                
                auto posX = YGNodeLayoutGetLeft(yogaChild);
                auto posY = YGNodeLayoutGetTop(yogaChild);
                
                float childXEndPoint = posX + childWidth;
                float childYEndPoint = posY + childHeight;
                if (YGNodeStyleGetFlexWrap(_yogaNode) == YGWrapWrapReverse) {
                    childYEndPoint = fabs(posY);
                }
                
                maxChildWidth = std::max(maxChildWidth, childXEndPoint);
                maxChildHeight = std::max(maxChildHeight, childYEndPoint);
                
                cocos2d::log("posX : %f", posX);
                cocos2d::log("posY : %f", posY);
                cocos2d::log("childHeight : %f", childHeight);
                cocos2d::log("childWidth : %f", childWidth);
                cocos2d::log("childEndPoint : %f", childXEndPoint);
                cocos2d::log("childYEndPoint : %f", childYEndPoint);
                cocos2d::log("maxChildWidth : %f", maxChildWidth);
                cocos2d::log("maxChildHeight : %f", maxChildHeight);
                
                auto yogaObj = yogaNodes[i];
                auto cocosNode = yogaObj->_node;
                if (cocosNode != nullptr){
                    // 마진을 포함하여 위치 설정
                    cocosNode->setPosition(posX,posY);
                    cocos2d::log("child->getPosition() : %f : %f", cocosNode->getPosition().x , cocosNode->getPosition().y);
                }
            }
        }
        
        if (this->getAlignItemsState() == YGAlignCenter || YGNodeStyleGetFlexWrap(_yogaNode) == YGWrapWrapReverse) {
            for (uint32_t i = 0; i < YGNodeGetChildCount(_yogaNode); i++) {
                auto yogaObj = yogaNodes[i];
                auto cocosNode = yogaObj->_node;
                if (cocosNode != nullptr){
                    // 마진을 포함하여 위치 설정
                    cocosNode->setPositionY(cocosNode->getPositionY() + (maxChildHeight));
                    cocos2d::log("child->getPosition() : %f : %f", cocosNode->getPosition().x , cocosNode->getPosition().y);
                }
            }
            if (this->getAlignItemsState() == YGAlignCenter && YGNodeStyleGetFlexWrap(_yogaNode) != YGWrapWrapReverse){
                maxChildHeight = maxChildHeight * 2;
            }
        }
        
        // 부모 크기 설정
        // Set the final size based on wrapping
        setContentSize(cocos2d::Size(maxChildWidth, maxChildHeight));
    }
    
private:
    YGNodeRef _yogaNode;
    int _yogaChildrenCount = 0;
    
};

#endif /* YogaNode_hpp */

