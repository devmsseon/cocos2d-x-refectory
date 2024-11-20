#ifndef YogaNode_hpp
#define YogaNode_hpp

#include "cocos2d.h"
#include "c_extenstions/yoga/Yoga.h"

class CCYFlexLayout{
public:
    std::vector<CCYFlexLayout*> yogaNodes;
    
    //CREATE_FUNC(YogaNode);
    cocos2d::Node* _node;  // Cocos2d Node 객체를 저장
    
    
    float mRealWdith = 0.0;
    
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
    CCYFlexLayout(cocos2d::Node* node = nullptr)
    : _node(node), _yogaNode(YGNodeNew()) {
        if (_node) {
            setTargetNode(_node);
        }
    }
    
    // 소멸자: 메모리 해제 및 자식 노드들의 메모리 해제 처리
    ~CCYFlexLayout() {
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
    
    void addChild(CCYFlexLayout* child) {
        if (_node == nullptr){
            return;
        }
        
        auto yogaChild = dynamic_cast<CCYFlexLayout*>(child);
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
        this->setWidth(contentSize.width);
        this->setHeight(contentSize.height);
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
        if (direction == YGFlexDirectionColumnReverse || direction == YGFlexDirectionColumn){
            this->setFlexWrap(YGWrapNoWrap);
        }
        YGNodeStyleSetFlexDirection(_yogaNode, direction);
    }
    
    void setJustifyContent(YGJustify justify) {
        YGNodeStyleSetJustifyContent(_yogaNode, justify);
    }
    
    void setPadding(YGEdge edgeType , float edgeValue) {
        YGNodeStyleSetPadding(_yogaNode,edgeType,edgeValue);
    }
    void setMargin(YGEdge edgeType , float edgeValue) {
        YGNodeStyleSetMargin(_yogaNode,edgeType,edgeValue);
    }
    
    float getMargin(YGEdge edgeType) {
        return YGNodeStyleGetMargin(_yogaNode,edgeType).value;
    }
    
    void setAlignContent(YGAlign align) {
        YGNodeStyleSetAlignContent(_yogaNode, align);
    }

    void setAlignItems(YGAlign align) {
        YGNodeStyleSetAlignItems(_yogaNode, align);
    }
    
    void setWidth(float width) {
        YGNodeStyleSetWidth(_yogaNode, width);
    }
    
    float getWidth() {
        return YGNodeStyleGetWidth(_yogaNode).value;
    }
    float getHeight() {
        return YGNodeStyleGetHeight(_yogaNode).value;
    }
    void setHeight(float height) {
        YGNodeStyleSetHeight(_yogaNode, height);
    }
    
    float getTotalWidth(){
        YGNodeCalculateLayout(_yogaNode, YGUndefined, YGUndefined,YGDirectionLTR);
        
        float totalWidth = 0.0f;
    
        for (uint32_t i = 0; i < YGNodeGetChildCount(_yogaNode); i++) {
            YGNodeRef yogaChild = YGNodeGetChild(_yogaNode, i);
            
            if (yogaChild) {
                float childWidth = YGNodeStyleGetWidth(yogaChild).value;
                float leftMaring = this->getMargin(YGEdgeLeft);
                float rightMaring = this->getMargin(YGEdgeRight);
                
                if (std::isnan(leftMaring)) {
                    leftMaring = 0;
                }
                if (std::isnan(rightMaring)) {
                    rightMaring = 0;
                }
                
                float addWidth = childWidth + leftMaring + rightMaring;
                
                totalWidth = totalWidth + addWidth;
            }
        }
        return totalWidth;
    }
    
    YGAlign getAlignItemsState() {
        return YGNodeStyleGetAlignItems(_yogaNode);
    }
    
    void doLayout() {
        if (_node == nullptr){
            return;
        }

       
        for (uint32_t i = 0; i < YGNodeGetChildCount(_yogaNode); i++) {
            auto yogaObj = yogaNodes[i];
            
            if (yogaObj->yogaNodes.size() > 0){
                yogaObj->doLayout();
            }
        
        }
     
    
        YGNodeCalculateLayout(_yogaNode, YGUndefined, YGUndefined,YGNodeStyleGetDirection(_yogaNode));
        
  
        float maxChildWidth = 0.0f;
        float maxChildHeight = 0.0f;
        
        auto wrapType = YGNodeStyleGetFlexWrap(_yogaNode);
        auto directionType = YGNodeStyleGetFlexDirection(_yogaNode);
        
        
        bool postionYCondition = false;
    
        
        float subMaxHeight = 0.0f;
 
        for (uint32_t i = 0; i < YGNodeGetChildCount(_yogaNode); i++) {
            YGNodeRef yogaChild = YGNodeGetChild(_yogaNode, i);
            
            if (yogaChild) {

                float childWidth = YGNodeStyleGetWidth(yogaChild).value;
                float childHeight =  YGNodeStyleGetHeight(yogaChild).value;
          
                
                auto yogaObj = yogaNodes[i];
                
                
                float posX = YGNodeLayoutGetLeft(yogaChild);
                float posY = YGNodeLayoutGetTop(yogaChild);
                
                float extraPosX = posX;
                float extraPosY = posY;
                
               

                float childXEndPoint = extraPosX + childWidth;
                float childYEndPoint = extraPosY + childHeight;
                
                float extraYOffst = 0;
                
                //열이면
                if (directionType == YGFlexDirectionRow || directionType == YGFlexDirectionRowReverse){
                    //리버스
                    if (wrapType == YGWrapWrapReverse){
                        childYEndPoint = childHeight;
                        
                        extraYOffst = childHeight;
                        postionYCondition = true;
                    }
                }
                else if (directionType == YGFlexDirectionColumnReverse){
                    childYEndPoint = fabs(posY);
                    postionYCondition = true;
                }
                subMaxHeight = std::max(subMaxHeight, childHeight);
                
              
                maxChildWidth = std::max(maxChildWidth, childXEndPoint);
                maxChildHeight = std::max(maxChildHeight, childYEndPoint);
                
                cocos2d::log("\n");
                cocos2d::log("posX : %f", posX);
                cocos2d::log("posY : %f", posY);
                cocos2d::log("childHeight : %f", childHeight);
                cocos2d::log("childWidth : %f", childWidth);
                cocos2d::log("childEndPoint : %f", childXEndPoint);
                cocos2d::log("childYEndPoint : %f", childYEndPoint);
                cocos2d::log("maxChildWidth : %f", maxChildWidth);
                cocos2d::log("maxChildHeight : %f", maxChildHeight);
                
                cocos2d::log("mRealWdith : %f", mRealWdith);
                cocos2d::log("\n");
                
                //yogaObj->doLayout();
                auto cocosNode = yogaObj->_node;
                if (cocosNode != nullptr){
                    // 마진을 포함하여 위치 설정
                    cocosNode->setPosition(posX,posY);
                    cocos2d::log("child->getPosition() : %f : %f", cocosNode->getPosition().x , cocosNode->getPosition().y);
                }
            }
        }
        
        maxChildHeight = std::max(maxChildHeight, subMaxHeight);
        
        if (postionYCondition == true){
            for (uint32_t i = 0; i < YGNodeGetChildCount(_yogaNode); i++) {
                auto yogaObj = yogaNodes[i];
                auto cocosNode = yogaObj->_node;
                if (cocosNode != nullptr){
                    // 마진을 포함하여 위치 설정
                    cocosNode->setPositionY(cocosNode->getPositionY() + (maxChildHeight));
                }
            }
        }
    
        // 부모 크기 설정
        // Set the final size based on wrapping
        
        if (YGNodeStyleGetFlexDirection(_yogaNode) == YGFlexDirectionColumn || YGNodeStyleGetFlexDirection(_yogaNode) == YGFlexDirectionColumnReverse){
            maxChildWidth = std::max(maxChildWidth, this->getWidth());
            
            if (maxChildHeight <= 0){
                maxChildHeight = std::max(maxChildHeight, this->_node->getContentSize().height);
            }
            setContentSize(cocos2d::Size(maxChildWidth, maxChildHeight));
        }
        else{
            //maxChildWidth = std::max(maxChildWidth, this->getWidth());
            setContentSize(cocos2d::Size(maxChildWidth, maxChildHeight));
        }

    }
    
private:
    YGNodeRef _yogaNode;
    int _yogaChildrenCount = 0;
    
};

#endif /* YogaNode_hpp */

//이 차이 덕분에 YGNodeLayoutGetHeight를 사용하면 최종 레이아웃의 높이 값을 확인할 수 있고,
//YGNodeStyleGetHeight는 스타일에 직접 설정한 값을 확인하는 데 유용합니다.
