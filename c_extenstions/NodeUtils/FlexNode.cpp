//
//  FlexNode.cpp
//  cocos2d_libs
//
//  Created by 선명수 on 11/20/24.
//

#include "FlexNode.h"

YGNodeRef FlexNode::getNode() const {
    return _ygNode;
}
std::vector<std::shared_ptr<FlexNode>> FlexNode::getNodChilds() const {
    return _children;
}

FlexNode& FlexNode::setTagName(std::string tagNAme) {
    this->tagName_ = tagNAme;
    return *this;
}

std::shared_ptr<FlexNode> FlexNode::create(cocos2d::Node* node) {
    //수동 메모리 관리(new와 delete)의 문제 → 스마트 포인터 등장 (std::shared_ptr, std::unique_ptr)
    //스마트 포인터의 사용 편의성과 성능 문제 → std::make_shared 등장
    auto tNode = std::make_shared<FlexNode>(node);
    return tNode;
}

float FlexNode::getContentWidth() {
    return YGNodeStyleGetWidth(_ygNode).value;
}
float FlexNode::getContentHeight() {
    return YGNodeStyleGetHeight(_ygNode).value;
}

FlexNode& FlexNode::setPadding(YGEdge edgeType , float edgeValue) {
    YGNodeStyleSetPadding(_ygNode,edgeType,edgeValue);
    return *this;
}

FlexNode& FlexNode::setMargin(YGEdge edgeType , float edgeValue) {
    YGNodeStyleSetMargin(_ygNode,edgeType,edgeValue);
    return *this;
}

FlexNode& FlexNode::setLimitWidth(float width) {
    _limitWidth = width;
    YGNodeStyleSetWidth(_ygNode, width);
    return *this;
}
float FlexNode::getLimitWidth() {
    return _limitWidth;
}

void FlexNode::removeAllCocosNode() {
    // Cocos2d-x 노드를 가져옵니다.
    auto cocosNode = this->getCocosNode();
    
    if (cocosNode != nullptr){
        // 자식 노드를 모두 제거합니다.
        cocosNode->removeAllChildren();
    }
   
    // _children 벡터에서 자식 노드를 지워줍니다.
    _children.clear();  // 이 부분을 추가하여 자식 노드를 관리하는 스마트 포인터도 해제되도록 합니다.
}

FlexNode& FlexNode::setWidth(float width) {
    YGNodeStyleSetWidth(_ygNode, width);
    if (_cocosNode != nullptr){
        _cocosNode->setContentSize(cocos2d::Size(width,_cocosNode->getContentSize().height));
    }
    return *this;
}

FlexNode& FlexNode::setHeight(float height) {
    YGNodeStyleSetHeight(_ygNode, height);
    if (_cocosNode != nullptr){
        _cocosNode->setContentSize(cocos2d::Size(_cocosNode->getContentSize().width,height));
    }
    return *this;
}

// 너비 가져오기
float FlexNode::getWidth() const {
    return YGNodeStyleGetWidth(_ygNode).value;
}
// 높이 가져오기
float FlexNode::getHeight() const {
    return YGNodeStyleGetHeight(_ygNode).value;
}

FlexNode& FlexNode::setContentSize(const cocos2d::Size& contentSize){
    YGNodeStyleSetWidth(_ygNode, contentSize.width);
    YGNodeStyleSetHeight(_ygNode, contentSize.height);
    if (_cocosNode != nullptr){
        _cocosNode->setContentSize(contentSize);
    }
    return *this;
}

cocos2d::Size FlexNode::getContentSize(const cocos2d::Size& contentSize){
    float posW = YGNodeLayoutGetWidth(_ygNode);
    float posH = YGNodeLayoutGetHeight(_ygNode);
    return cocos2d::Size(posW, posH);
}

FlexNode& FlexNode::setFlexDirection(YGFlexDirection direction) {
    YGNodeStyleSetFlexDirection(_ygNode, direction);
    return *this;
}

FlexNode& FlexNode::setJustifyContent(YGJustify justify) {
    YGNodeStyleSetJustifyContent(_ygNode, justify);
    return *this;
}

FlexNode& FlexNode::setAlignItems(YGAlign align) {
    YGNodeStyleSetAlignItems(_ygNode, align);
    return *this;
}

FlexNode& FlexNode::setAlignSelf(YGAlign align) {
    YGNodeStyleSetAlignSelf(_ygNode, align);
    return *this;
}

FlexNode& FlexNode::setAlignContent(YGAlign align) {
    YGNodeStyleSetAlignContent(_ygNode, align);
    return *this;
}

cocos2d::Node* FlexNode::getCocosNode() {
    return _cocosNode;
}

FlexNode& FlexNode::addChild(const std::shared_ptr<FlexNode>& child) {
    
    // 자식 FlexNode의 cocos2d Node를 가져오기
    cocos2d::Node* cocosNode = child->getCocosNode();
    
    // dynamic_cast로 Label 여부 확인
    if (cocos2d::Label* label = dynamic_cast<cocos2d::Label*>(cocosNode)) {
        
        // child가 Label일 경우 처리
        label->setDimensions(this->getWidth(), 0); // 너비는 200, 높이는 0으로 설정 (자동 계산)
        // labelTestNode->layoutDrawForSub();
        child->setWidth(this->getWidth());
        child->setHeight(cocosNode->getContentSize().height);
    } else {
        // child가 Label이 아닐 경우 일반적인 처리
    }
    
    YGNodeInsertChild(_ygNode, child->getNode(), YGNodeGetChildCount(_ygNode));
    _children.push_back(child);
    _cocosNode->addChild(child->getCocosNode());

    return *this;
}

// 람다를 활용해 하위 노드 구성
FlexNode& FlexNode::define(const std::function<void(FlexNode&)>& defineFunc) {
    defineFunc(*this);
    return *this;
}

// FlexDirection 가져오기
YGFlexDirection FlexNode::getFlexDirection() const {
    return YGNodeStyleGetFlexDirection(_ygNode);
}



// FlexWrap 설정
FlexNode& FlexNode::setFlexWrap(YGWrap wrap) {
    YGNodeStyleSetFlexWrap(_ygNode, wrap);
    return *this;
}

// FlexWrap 가져오기
YGWrap FlexNode::getFlexWrap() const {
    return YGNodeStyleGetFlexWrap(_ygNode);
}



float FlexNode::getParentWidth(){
    YGNodeRef parent = YGNodeGetParent(this->getNode());
    if (parent == nullptr){
        return 0;
    }
    float width = YGNodeLayoutGetWidth(parent);
    return width;
}

bool FlexNode::isRow(){
    bool retVal = false;
    auto directionType = YGNodeStyleGetFlexDirection(_ygNode);
    if (directionType == YGFlexDirectionRow || directionType == YGFlexDirectionRowReverse){
        retVal = true;
    }
    return retVal;
}


YGNodeRef FlexNode::getFirstChild(YGNodeRef parentNode) {
    if (!parentNode || YGNodeGetChildCount(parentNode) == 0) {
        return nullptr; // 부모 노드가 없거나 자식이 없는 경우
    }
    return YGNodeGetChild(parentNode, 0); // 첫 번째 자식 반환
}

YGNodeRef FlexNode::getLastChild(YGNodeRef parentNode) {
    size_t childCount = YGNodeGetChildCount(parentNode);
    if (!parentNode || childCount == 0) {
        return nullptr; // 부모 노드가 없거나 자식이 없는 경우
    }
    return YGNodeGetChild(parentNode, childCount - 1); // 마지막 자식 반환
}

void FlexNode::layoutDrawForSub(float width, float height, FlexNode* refFlexNode){
    this->layoutDraw(width,height,refFlexNode,false);
}

void FlexNode::layoutDrawForMain(float width ,float height, FlexNode* refFlexNode ){
    this->layoutDraw(width,height,refFlexNode,true);
}

void FlexNode::layoutDraw(float width, float height, FlexNode* refFlexNode, bool isMainLayout) {

    YGNodeCalculateLayout(_ygNode, width, height, YGNodeLayoutGetDirection(_ygNode));
    
    if (isMainLayout == true){
        for (uint32_t i = 0; i < YGNodeGetChildCount(_ygNode); i++) {
            auto tNode = _children.at(i);
            tNode->layoutDraw(width, height,refFlexNode);
        }
    }
   
    
    auto wrapType = YGNodeStyleGetFlexWrap(_ygNode);
    auto directionType = YGNodeStyleGetFlexDirection(_ygNode);
    bool postionYCondition = false;
  
    float childEndXPoint = 0;
    float childEndYPoint = 0;
    float maxChildWidth = 0;
    float maxChildHeight = 0;
    
    float basePostion = 0;
    
    float wordPosX = 0;
    float wordPosY = 0;
    ;
    float v_childEndXPoint = 0;
    float v_childEndYPoint = 0;
    float v_maxChildWidth = 0;
    float v_maxChildHeight = 0;

    if (refFlexNode != nullptr){
        if (this->getNode()  != nullptr){
            YGNodeRef parent = YGNodeGetParent(this->getNode());
            if (parent  != nullptr){
                wordPosX = YGNodeLayoutGetLeft(parent);
                wordPosY = YGNodeLayoutGetTop(parent);
            }
        }
        
    }

     
    for (uint32_t i = 0; i < YGNodeGetChildCount(_ygNode); i++) {
        YGNodeRef yogaChild = YGNodeGetChild(_ygNode, i);

        
       // auto [correctedX, correctedY] = calculateCorrectedPosition(_ygNode, yogaChild);
        //float posX = correctedX;
        //float posY = correctedY;
        float posX = YGNodeLayoutGetLeft(yogaChild);
        float posY = YGNodeLayoutGetTop(yogaChild);
       
        
        float posW = YGNodeLayoutGetWidth(yogaChild);
        float posH = YGNodeLayoutGetHeight(yogaChild);
        
        
        float v_posX = fabs(posX);
        float v_posY = fabs(posY);
   

        if (i == 0){
            basePostion = (posY + posH);
        }
        
        if (this->isRow() == true){
        
            if (wrapType == YGWrapWrapReverse){
                float cPosY = (basePostion - posY);
                posY = -cPosY;
                
                // 자식들의 레이아웃을 뒤집어서 계산할 필요가 있을 경우
                //posY = YGNodeLayoutGetTop(_ygNode) + YGNodeLayoutGetHeight(_ygNode) - (posY + posH);
                // X 방향 반전
                //posX = YGNodeLayoutGetLeft(_ygNode) + YGNodeLayoutGetWidth(_ygNode) - (posX + posW);
            }
        }
        else{
            if (directionType == YGFlexDirectionColumnReverse){
   
                float cPosY = (basePostion - posY);
                posY = -cPosY;
                 
                /*
                if (i == 0){
                    basePostion = posY + posH;
                }
                posY = basePostion - posY - posH;
                posY = YGNodeLayoutGetTop(_ygNode) + YGNodeLayoutGetHeight(_ygNode) - (posY + posH);
                 */

            }
        }
        
        childEndXPoint = wordPosX + posX + posW;
        childEndYPoint = wordPosY + posY + posH;
        
        
        v_childEndXPoint = wordPosX + v_posX + posW;
        v_childEndYPoint = wordPosY + v_posY + posH;
        
        if (wordPosY > 0 && wordPosX > 0){
            return;
        }
        
        if (this->isRow() == true){
            //리버스
            if (wrapType == YGWrapWrapReverse){
                childEndYPoint = wordPosY + fabs(posY);
                postionYCondition = true;
                /*
                float positionDiff = fabs(fabs(posY) - fabs(posH)) ;
                 childEndYPoint = positionDiff + posH;
        
                 postionYCondition = true;
      */
            }
        }
        else{
            if (directionType == YGFlexDirectionColumnReverse){
                childEndYPoint = wordPosY + fabs(posY);
                postionYCondition = true;
            }
        }
        
        maxChildWidth = std::max(maxChildWidth, childEndXPoint);
        maxChildHeight = std::max(maxChildHeight, childEndYPoint);
        
        v_maxChildWidth = std::max(v_maxChildWidth, v_childEndXPoint);
        v_maxChildHeight = std::max(v_maxChildHeight, v_childEndYPoint);
        

        if (isMainLayout == true){
            auto yogaObj = _children[i];
            auto cocosNode = yogaObj->getCocosNode();
            if (cocosNode != nullptr){
                // 마진을 포함하여 위치 설정
                cocosNode->setPosition(posX,posY);
                cocosNode->setContentSize(cocos2d::Size(posW, posH));
                cocos2d::log("child->getPosition() : %f : %f", cocosNode->getPosition().x , cocosNode->getPosition().y);
            }
        }

    }

    float posX = YGNodeLayoutGetLeft(_ygNode);
    float posY = YGNodeLayoutGetTop(_ygNode);
    float posW = YGNodeLayoutGetWidth(_ygNode);
    float posH = YGNodeLayoutGetHeight(_ygNode);
    

    CCLOG("Cur Position: X = %.2f, Y = %.2f", posX, posY);
    CCLOG("Cur: Size: Width = %.2f, Height = %.2f", posW, posH);
    
    // 값 로깅
    CCLOG("Child X End Point: %.2f", childEndXPoint);
    CCLOG("Child Y End Point: %.2f", childEndYPoint);
    CCLOG("Max Child Width: %.2f", maxChildWidth);
    CCLOG("Max Child Height: %.2f", maxChildHeight);
    
    if (tagName_ == "justifyTestNode"){
        CCLOG("가상 맥스 Width: %.2f", v_maxChildWidth);
        CCLOG("가상 맥스 Height: %.2f", v_maxChildHeight);
    }
    virtualWidth = v_maxChildWidth;
    virtualHeight = v_maxChildHeight;
    
    maxChildWidth = std::max(maxChildWidth, posW);
    maxChildHeight = std::max(childEndYPoint, maxChildHeight);
   
    
    

    if (isSizeToFit == true){
        maxChildWidth = std::min(childEndXPoint, maxChildWidth);
    }
    
    if (isMainLayout == true){
        if (postionYCondition == true){
            for (uint32_t i = 0; i < YGNodeGetChildCount(_ygNode); i++) {
                auto yogaObj = _children[i];
                auto cocosNode = yogaObj->getCocosNode();
                if (cocosNode != nullptr){
                    // 마진을 포함하여 위치 설정
                    cocosNode->setPositionY(cocosNode->getPositionY() + (maxChildHeight));
                }
            }
        }
    }
    
    // Cocos2d-x Node에 적용
    if (_cocosNode != nullptr){
        _cocosNode->setPosition(posX,posY);
        this->setWidth(maxChildWidth);
        this->setHeight(maxChildHeight);
    }
}
