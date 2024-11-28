//
//  FlexNode.cpp
//  cocos2d_libs
//
//  Created by 선명수 on 11/20/24.
//

#include "CCFlexLayout.h"
#include "CCNodeWrapLayout.h"

YGNodeRef CCFlexLayout::getNode() const {
    return _ygNode;
}
std::vector<std::shared_ptr<CCFlexLayout>> CCFlexLayout::getNodChilds() const {
    return _children;
}

CCFlexLayout& CCFlexLayout::setTagName(std::string tagNAme) {
    this->tagName_ = tagNAme;
    return *this;
}

std::shared_ptr<CCFlexLayout> CCFlexLayout::create(cocos2d::Node* node) {
    //수동 메모리 관리(new와 delete)의 문제 → 스마트 포인터 등장 (std::shared_ptr, std::unique_ptr)
    //스마트 포인터의 사용 편의성과 성능 문제 → std::make_shared 등장
    auto tNode = std::make_shared<CCFlexLayout>(node);
    return tNode;
}

/*
float CCFlexLayout::getContentWidth() {
    return YGNodeStyleGetWidth(_ygNode).value;
}
*/

float CCFlexLayout::getLeftRightMargin()  {
    // 왼쪽과 오른쪽 마진 제외
    //float marginLeft = YGNodeLayoutGetMargin(_ygNode, YGEdgeLeft);
    //float marginRight = YGNodeLayoutGetMargin(_ygNode, YGEdgeRight);
    float marginLeft = marginLeft_;
    float marginRight = marginRight_;
    float total =  marginLeft + marginRight;
    return total;
}
float CCFlexLayout::getTopBottomMargin()  {
    // 왼쪽과 오른쪽 마진 제외
    //float marginTop = YGNodeLayoutGetMargin(_ygNode, YGEdgeTop);
    //float marginBottom = YGNodeLayoutGetMargin(_ygNode, YGEdgeBottom);
    float marginTop = marginTop_;
    float marginBottom = marginBottom_;
    float total =  marginTop + marginBottom;
    return total;
}

float CCFlexLayout::getContentWidth()  {
    // 기본 너비
    float width = YGNodeStyleGetWidth(_ygNode).value;
    width = width - this->getLeftRightMargin(); // 마진 제외
    return width; // 마진 제외
}

float CCFlexLayout::getContentHeight() {
    // 기본 높이
    float height = YGNodeStyleGetHeight(_ygNode).value;
    height = height - this->getTopBottomMargin();
    return height; // 마진 제외
}

//float CCFlexLayout::getContentHeight() {
//    return YGNodeStyleGetHeight(_ygNode).value;
//}

CCFlexLayout& CCFlexLayout::setPadding(YGEdge edgeType , float edgeValue) {
    YGNodeStyleSetPadding(_ygNode,edgeType,edgeValue);
    return *this;
}

CCFlexLayout& CCFlexLayout::setMargin(YGEdge edgeType , float edgeValue) {
    if (edgeType == YGEdgeTop){
        marginTop_ = edgeValue;
    }
    else if (edgeType == YGEdgeBottom){
        marginBottom_ = edgeValue;
    }
    else if (edgeType == YGEdgeLeft){
        marginLeft_ = edgeValue;
    }
    else if (edgeType == YGEdgeRight){
        marginRight_ = edgeValue;
    }
    YGNodeStyleSetMargin(_ygNode,edgeType,edgeValue);
    return *this;
}

CCFlexLayout& CCFlexLayout::setLimitWidth(float width) {
    _limitWidth = width;
    YGNodeStyleSetWidth(_ygNode, width);
    return *this;
}
float CCFlexLayout::getLimitWidth() {
    return _limitWidth;
}

void CCFlexLayout::removeAllCocosNode() {
    // Cocos2d-x 노드를 가져옵니다.
    auto cocosNode = this->getCocosNode();
    
    if (cocosNode != nullptr){
        // 자식 노드를 모두 제거합니다.
        cocosNode->removeAllChildren();
    }
   
    // _children 벡터에서 자식 노드를 지워줍니다.
    _children.clear();  // 이 부분을 추가하여 자식 노드를 관리하는 스마트 포인터도 해제되도록 합니다.
}

CCFlexLayout& CCFlexLayout::setWidth(float width) {
    YGNodeStyleSetWidth(_ygNode, width);
    if (_cocosNode != nullptr){
        _cocosNode->setContentSize(cocos2d::Size(width,_cocosNode->getContentSize().height));
    }
    return *this;
}

CCFlexLayout& CCFlexLayout::setHeight(float height) {
    YGNodeStyleSetHeight(_ygNode, height);
    if (_cocosNode != nullptr){
        _cocosNode->setContentSize(cocos2d::Size(_cocosNode->getContentSize().width,height));
    }
    return *this;
}

// 너비 가져오기
float CCFlexLayout::getWidth() const {
    return YGNodeStyleGetWidth(_ygNode).value;
}
// 높이 가져오기
float CCFlexLayout::getHeight() const {
    return YGNodeStyleGetHeight(_ygNode).value;
}

float CCFlexLayout::getPositionX(){
    float retVal = 0.0f;
    if (_cocosNode != nullptr){
        retVal = _cocosNode->getPositionX();
    }
    return retVal;
}

float CCFlexLayout::getPositionY(){
    float retVal = 0.0f;
    if (_cocosNode != nullptr){
        retVal = _cocosNode->getPositionY();
    }
    return retVal;
}

CCFlexLayout& CCFlexLayout::setPositionY(float poxY){
    if (_cocosNode != nullptr){
        _cocosNode->setPositionY(poxY);
    }
    return *this;
}


CCFlexLayout& CCFlexLayout::setPosition(float posX , float poxY){
    if (_cocosNode != nullptr){
        _cocosNode->setPosition(posX, poxY);
    }
    return *this;
}


CCFlexLayout& CCFlexLayout::setContentSize(const cocos2d::Size& contentSize){
    YGNodeStyleSetWidth(_ygNode, contentSize.width);
    YGNodeStyleSetHeight(_ygNode, contentSize.height);
    if (_cocosNode != nullptr){
        _cocosNode->setContentSize(contentSize);
    }
    return *this;
}

cocos2d::Size CCFlexLayout::getContentSize(const cocos2d::Size& contentSize){
    float posW = YGNodeLayoutGetWidth(_ygNode);
    float posH = YGNodeLayoutGetHeight(_ygNode);
    return cocos2d::Size(posW, posH);
}

CCFlexLayout& CCFlexLayout::setFlexDirection(YGFlexDirection direction) {
    YGNodeStyleSetFlexDirection(_ygNode, direction);
    return *this;
}

CCFlexLayout& CCFlexLayout::setJustifyContent(YGJustify justify) {
    YGNodeStyleSetJustifyContent(_ygNode, justify);
    return *this;
}

CCFlexLayout& CCFlexLayout::setAlignItems(YGAlign align) {
    YGNodeStyleSetAlignItems(_ygNode, align);
    return *this;
}

CCFlexLayout& CCFlexLayout::setAlignSelf(YGAlign align) {
    YGNodeStyleSetAlignSelf(_ygNode, align);
    return *this;
}

CCFlexLayout& CCFlexLayout::setAlignContent(YGAlign align) {
    YGNodeStyleSetAlignContent(_ygNode, align);
    return *this;
}

cocos2d::Node* CCFlexLayout::getCocosNode() {
    return _cocosNode;
}

CCFlexLayout& CCFlexLayout::addChild(const std::shared_ptr<CCFlexLayout>& child ,bool fitWidth) {
    YGNodeInsertChild(_ygNode, child->getNode(), YGNodeGetChildCount(_ygNode));
    
    // 자식 FlexNode의 cocos2d Node를 가져오기
    cocos2d::Node* cocosNode = child->getCocosNode();
    
    
    // child가 Label이 아닐 경우 일반적인 처리
    if (fitWidth == true){
        auto tWidth = this->getContentWidth() - child->getLeftRightMargin();
        // dynamic_cast로 Label 여부 확인
        if (cocos2d::Label* label = dynamic_cast<cocos2d::Label*>(cocosNode)) {
            // child가 Label일 경우 처리
            label->setDimensions(tWidth, 0); // 너비는 200, 높이는 0으로 설정 (자동 계산)
            
            child->setHeight(cocosNode->getContentSize().height);
        }
        // labelTestNode->layoutDrawForSub();
        child->setWidth(tWidth);
    }

    _children.push_back(child);
    _cocosNode->addChild(child->getCocosNode());

    return *this;
}


// 람다를 활용해 하위 노드 구성
void CCFlexLayout::fitWidth(){
    this->setWidth(this->getContentWidth());
}
void CCFlexLayout::fitHeight(){
    this->setHeight(this->virtualHeight + this->getTopBottomMargin());
}

//void CCFlexLayout::fitHeight(){
//    this->setHeight(this->getContentHeight());
//}

void CCFlexLayout::fitContent(){
    this->fitWidth();
    this->fitHeight();
}

// 람다를 활용해 하위 노드 구성
CCFlexLayout& CCFlexLayout::define(const std::function<void(CCFlexLayout&)>& defineFunc) {
    defineFunc(*this);
    return *this;
}

// FlexDirection 가져오기
YGFlexDirection CCFlexLayout::getFlexDirection() const {
    return YGNodeStyleGetFlexDirection(_ygNode);
}



// FlexWrap 설정
CCFlexLayout& CCFlexLayout::setFlexWrap(YGWrap wrap) {
    YGNodeStyleSetFlexWrap(_ygNode, wrap);
    return *this;
}

// FlexWrap 가져오기
YGWrap CCFlexLayout::getFlexWrap() const {
    return YGNodeStyleGetFlexWrap(_ygNode);
}



float CCFlexLayout::getParentWidth(){
    YGNodeRef parent = YGNodeGetParent(this->getNode());
    if (parent == nullptr){
        return 0;
    }
    float width = YGNodeLayoutGetWidth(parent);
    return width;
}

bool CCFlexLayout::isRow(){
    bool retVal = false;
    auto directionType = YGNodeStyleGetFlexDirection(_ygNode);
    if (directionType == YGFlexDirectionRow || directionType == YGFlexDirectionRowReverse){
        retVal = true;
    }
    return retVal;
}

CCFlexLayout& CCFlexLayout::setBackGroundColor(const cocos2d::Color3B &color) {
    auto cocosNode = this->getCocosNode();
    if (cocosNode == nullptr){
        return *this;
    }
    
    if (CCNodeWrapLayout* tNode = dynamic_cast<CCNodeWrapLayout*>(cocosNode)) {
        tNode->setBackGroundColor(color);
    }
    return *this;
}


YGNodeRef CCFlexLayout::getFirstChild(YGNodeRef parentNode) {
    if (!parentNode || YGNodeGetChildCount(parentNode) == 0) {
        return nullptr; // 부모 노드가 없거나 자식이 없는 경우
    }
    return YGNodeGetChild(parentNode, 0); // 첫 번째 자식 반환
}

YGNodeRef CCFlexLayout::getLastChild(YGNodeRef parentNode) {
    size_t childCount = YGNodeGetChildCount(parentNode);
    if (!parentNode || childCount == 0) {
        return nullptr; // 부모 노드가 없거나 자식이 없는 경우
    }
    return YGNodeGetChild(parentNode, childCount - 1); // 마지막 자식 반환
}

void CCFlexLayout::layoutDrawForSub(float width, float height, CCFlexLayout* refFlexNode){
    this->layoutDraw(width,height,refFlexNode,false);
}

void CCFlexLayout::layoutDrawForMain(float width ,float height, CCFlexLayout* refFlexNode ){
    this->layoutDraw(width,height,refFlexNode,true);
}

void CCFlexLayout::layoutDraw(float width, float height, CCFlexLayout* refFlexNode, bool isMainLayout) {

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
                cocos2d::log("child->getPosition() : %f : %f", cocosNode->getPosition().x , cocosNode->getPosition().y);
                // 마진을 포함하여 위치 설정
                //cocosNode->setPosition(posX,posY);
                //cocosNode->setContentSize(cocos2d::Size(posW, posH));
                yogaObj->setPosition(posX,posY);
                yogaObj->setContentSize(cocos2d::Size(posW, posH));
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
                /*
                auto cocosNode = yogaObj->getCocosNode();
                if (cocosNode != nullptr){
                    // 마진을 포함하여 위치 설정
                    cocosNode->setPositionY(cocosNode->getPositionY() + (maxChildHeight));
                }
                 */
                
                yogaObj->setPositionY(yogaObj->getPositionY() + (maxChildHeight));
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
