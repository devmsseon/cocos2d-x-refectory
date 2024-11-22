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
    
    
    
    float totalMaxWidth = 0.0f;
    float totalMaxHeight = 0.0f;
    
    float virtualWidth = 0.0f;
    float virtualHeight = 0.0f;
   
    
    
    bool isTotalRootNode = false;
    
    FlexNode& setTagName(std::string tagNAme) {
        this->tagName_ = tagNAme;
        return *this;
    }
    
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
    
    FlexNode& setLimitWidth(float width) {
        _limitWidth = width;
        YGNodeStyleSetWidth(_ygNode, width);
        return *this;
    }
    float getLimitWidth() {
        return _limitWidth;
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
    
    float getParentWidth(){
        YGNodeRef parent = YGNodeGetParent(this->getNode());
        if (parent == nullptr){
            return 0;
        }
        float width = YGNodeLayoutGetWidth(parent);
        return width;
    }
    
    bool isRow(){
        bool retVal = false;
        auto directionType = YGNodeStyleGetFlexDirection(_ygNode);
        if (directionType == YGFlexDirectionRow || directionType == YGFlexDirectionRowReverse){
            retVal = true;
        }
        return retVal;
    }
  
    // 높이 가져오기
    float getHeight() const {
        return YGNodeStyleGetHeight(_ygNode).value;
    }
    
    YGNodeRef getFirstChild(YGNodeRef parentNode) {
        if (!parentNode || YGNodeGetChildCount(parentNode) == 0) {
            return nullptr; // 부모 노드가 없거나 자식이 없는 경우
        }
        return YGNodeGetChild(parentNode, 0); // 첫 번째 자식 반환
    }

    YGNodeRef getLastChild(YGNodeRef parentNode) {
        uint32_t childCount = YGNodeGetChildCount(parentNode);
        if (!parentNode || childCount == 0) {
            return nullptr; // 부모 노드가 없거나 자식이 없는 경우
        }
        return YGNodeGetChild(parentNode, childCount - 1); // 마지막 자식 반환
    }
    
    void layoutDrawForSub(float width = YGUndefined, float height = YGUndefined, FlexNode* refFlexNode = nullptr){
        this->layoutDraw(width,height,refFlexNode,false);
    }
    
    void layoutDrawForMain(float width = YGUndefined, float height = YGUndefined, FlexNode* refFlexNode = nullptr){
        this->layoutDraw(width,height,refFlexNode,true);
    }
    
    void layoutDraw(float width = YGUndefined, float height = YGUndefined, FlexNode* refFlexNode = nullptr , bool isMainLayout = true) {
  
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
        
        
        float v_posX = 0;
        float v_posY = 0;
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
    
    void layout(float width = YGUndefined, float height = YGUndefined , FlexNode* refFlexNode = nullptr) {
       
        //YGNodeCalculateLayout(_ygNode, width, height, YGDirectionLTR);
        YGNodeCalculateLayout(_ygNode, width, YGUndefined, YGNodeLayoutGetDirection(_ygNode));
        
        
        for (uint32_t i = 0; i < YGNodeGetChildCount(_ygNode); i++) {
            auto tNode = _children.at(i);
            tNode->layout(width, height,refFlexNode);
        }
        // 레이아웃 계산

        // 계산된 레이아웃 값에서 위치 및 크기 정보 추출
        float posX = YGNodeLayoutGetLeft(_ygNode);
        float posY = YGNodeLayoutGetTop(_ygNode);
        float posW = YGNodeLayoutGetWidth(_ygNode);
        float posH = YGNodeLayoutGetHeight(_ygNode);
        
        if (YGNodeStyleGetFlexDirection(_ygNode) == YGFlexDirectionColumnReverse) {
            // 부모 높이를 기준으로 Y 좌표 보정
            posY = posH - (YGNodeLayoutGetTop(_ygNode) + YGNodeLayoutGetHeight(_ygNode));
        }
        
        // WrapReverse 처리
        if (YGNodeStyleGetFlexWrap(_ygNode) == YGWrapWrapReverse) {
            // 자식들의 레이아웃을 뒤집어서 계산할 필요가 있을 경우
            posY = YGNodeLayoutGetTop(_ygNode) + YGNodeLayoutGetHeight(_ygNode) - (posY + posH);
        }
        
        float childXEndXPoint = 0;
        float childXEndYPoint = 0;
        float wordPosX = 0;
        float wordPosY = 0;
        if (refFlexNode != nullptr && this->isTotalRootNode == false){
            
            YGNodeRef parent = YGNodeGetParent(this->getNode());
            wordPosX = YGNodeLayoutGetLeft(parent);
            wordPosY = YGNodeLayoutGetTop(parent);
            
            
            auto refFlexDirType = refFlexNode->getFlexDirection();

            if (refFlexDirType == YGFlexDirectionRow || refFlexDirType == YGFlexDirectionRowReverse){
                childXEndXPoint = wordPosX + posX + posW;
                childXEndYPoint = wordPosY + posY + posH;
               
            }
            else if (refFlexDirType == YGFlexDirectionColumn || refFlexDirType == YGFlexDirectionColumnReverse){
                childXEndXPoint = wordPosX + posX + posW;
                childXEndYPoint = wordPosY + posY + posH;
            
            }
           
            refFlexNode->totalMaxWidth = std::max(refFlexNode->totalMaxWidth, childXEndXPoint);
            refFlexNode->totalMaxHeight = std::max(refFlexNode->totalMaxHeight, childXEndYPoint);
        }
 
        // Cocos2d-x Node에 적용
        if (_cocosNode != nullptr){
            _cocosNode->setPosition(cocos2d::Vec2(posX, posY));
            _cocosNode->setContentSize(cocos2d::Size(posW, posH));
            
            this->setWidth(posW);
            this->setHeight(posH);
        }
       
        CCLOG("cocos Child %d: wordPosX = %.2f, wordPosY  = %.2f", 0, wordPosX,wordPosY);
        CCLOG("cocos Child %d: x = %.2f, y = %.2f, width = %.2f, height = %.2f", 0, posX, posY, posW, posH);
        
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
    float getMaxChildContentWidth(const std::vector<std::shared_ptr<FlexNode>>& flexNodes) {
        float maxWidth = 0.0f;

        for (const auto& flexNode : flexNodes) {
            if (!flexNode) continue;

            // FlexNode에서 Cocos 노드 가져오기
            auto cocosNode = flexNode->getCocosNode();
            if (cocosNode) {
                // Cocos 노드의 콘텐츠 크기 계산
                float contentWidth = cocosNode->getContentSize().width;
                maxWidth = std::max(maxWidth, contentWidth);
            }
        }

        return maxWidth;
    }
    cocos2d::Size getCalContentSize() {
        if (_children.empty()) {
            return cocos2d::Size::ZERO; // 자식 노드가 없으면 거리는 0
        }

       // auto cocosNode = this->getCocosNode();
       // const auto& children = cocosNode->getChildren();
        
        // 첫 번째와 마지막 노드 가져오기
        auto firstCocosNode = _children.front()->getCocosNode();
        auto lastCocosNode = _children.back()->getCocosNode();
        
        // 첫 번째와 마지막 노드 가져오기
        auto firstNode = this->getFirstChild(this->_ygNode);
        auto lastNode =  this->getLastChild(this->_ygNode);
        
    
        if (!firstCocosNode || !lastCocosNode) {
            return cocos2d::Size::ZERO; // 유효하지 않은 노드가 있다면 거리 0 반환
        }
        
        if (!firstNode || !lastNode) {
            return cocos2d::Size::ZERO; // 유효하지 않은 노드가 있다면 거리 0 반환
        }
        
        
        auto dirType = this->getFlexDirection();
       

        // Y 좌표 계산
        float firstNodeY = firstCocosNode->getPositionY();
        float lastNodeY = lastCocosNode->getPositionY();
        
        float firstNodeX = firstCocosNode->getPositionX();
        float lastNodeX = lastCocosNode->getPositionX();
        
        
        float diffX = std::fabs(lastNodeX + firstNodeX);
        if (lastNodeX <= 0){
          //  diffX = 0;
        }
        
        float diffY = std::fabs(lastNodeY - firstNodeY);
       
        float maxWidth = 0.0f;
        
        // 자식 노드 순회하며 가장 큰 콘텐츠 높이 계산
        
        
        
        // Y 축 거리 계산 (절대값)
        float diffOffsetY = 0;
        float diffOffsetX = 0;
        float topMargin = 0;
        if (dirType == YGFlexDirectionColumnReverse){
            //maxWidth = this->getMaxChildContentWidth(this->_children) + 1;
            diffOffsetX = maxWidth;
            diffOffsetY = firstCocosNode->getContentSize().height;
        }
        else if (dirType == YGFlexDirectionColumn){
            diffOffsetY = lastCocosNode->getContentSize().height;
            diffOffsetX = lastCocosNode->getContentSize().width;
        }
        else if (dirType == YGFlexDirectionRow){
           // maxWidth = this->getMaxChildContentWidth(this->_children) + 1;
           // diffOffsetX = maxWidth;
        }
        float finalValueX = diffX + diffOffsetX;
        float finalValueY = diffY + diffOffsetY + topMargin;
      
  
        return cocos2d::Size(finalValueX,finalValueY);
    }
    void rootDemesionProcess(){
        std::cout << "rootDemesionProcess: {"
        << "this->totalMaxWidth: " <<this->totalMaxWidth
        << "this->totalMaxHeight: " <<this->totalMaxHeight
        << "}\n";
    
        if ( this->_limitWidth > 0){
            if (this->totalMaxWidth > this->getLimitWidth()){
                this->totalMaxWidth = this->getLimitWidth();
            }
        }
        auto containerCalSize = this->getCalContentSize();
        this->setWidth(this->totalMaxWidth);
        this->setHeight(containerCalSize.height);
    }
    
    std::pair<float, float> getCalculateContentSize(){
        return calculateContentSizeWithWrap(_ygNode);
    }
    
    void calMaxDemesion(){
        std::cout << "Node: {"
        << "this->totalMaxWidth: " <<this->totalMaxWidth
        << "this->totalMaxHeight: " <<this->totalMaxHeight
        << "}\n";
    
        if ( this->_limitWidth > 0){
            if (this->totalMaxWidth > this->getLimitWidth()){
                this->totalMaxWidth = this->getLimitWidth();
            }
        }
        auto [contentWidth, contentHeight] = calculateContentSizeWithWrap(_ygNode);
        this->setWidth(contentWidth);
        this->setHeight(contentHeight);
        /*
        auto containerCalSize = this->getCalContentSize();
        this->setWidth(this->totalMaxWidth);
        this->setHeight(containerCalSize.height);
         */
      
        /*
        auto containerCalSize = this->getCalContentSize();
    
        this->setWidth(containerCalSize.width);
        this->setHeight(containerCalSize.height);
  */
        
       // this->setWidth(this->totalMaxWidth);
        //this->setHeight(this->totalMaxHeight);
     
        /*
        if (this->getCocosNode() != nullptr){
            this->getCocosNode()->setContentSize(cocos2d::Size(this->totalMaxWidth,this->totalMaxHeight));
        }
         */
    }
    
    float calculateContentWidth(YGNodeRef parentNode) {
        float totalWidth = 0.0f;
        for (uint32_t i = 0; i < YGNodeGetChildCount(parentNode); ++i) {
            YGNodeRef childNode = YGNodeGetChild(parentNode, i);
            totalWidth = std::max(totalWidth, YGNodeLayoutGetLeft(childNode) + YGNodeLayoutGetWidth(childNode));
        }
        return totalWidth;
    }

    float calculateContentHeight(YGNodeRef parentNode) {
        float totalHeight = 0.0f;
        for (uint32_t i = 0; i < YGNodeGetChildCount(parentNode); ++i) {
            YGNodeRef childNode = YGNodeGetChild(parentNode, i);
            totalHeight = std::max(totalHeight, YGNodeLayoutGetTop(childNode) + YGNodeLayoutGetHeight(childNode));
        }
        return totalHeight;
    }

    // 사용 예시
    //float contentWidth = calculateContentWidth(_ygNode);
    //float contentHeight = calculateContentHeight(_ygNode);

    /*
     2. Wrap, Reverse 보정
     Wrap이나 Reverse가 활성화되면, 좌표가 꼬이는 문제가 발생합니다.
     다음은 FlexDirection과 Wrap 옵션을 처리하는 보정 로직입니다:
     */
    float calculateCorrectedYPosition(YGNodeRef parentNode, YGNodeRef childNode) {
        float parentHeight = YGNodeLayoutGetHeight(parentNode);
        float childTop = YGNodeLayoutGetTop(childNode);
        float childHeight = YGNodeLayoutGetHeight(childNode);

        if (YGNodeStyleGetFlexDirection(parentNode) == YGFlexDirectionColumnReverse) {
            return parentHeight - (childTop + childHeight);
        }
        return childTop;
    }

    float calculateCorrectedXPosition(YGNodeRef parentNode, YGNodeRef childNode) {
        float parentWidth = YGNodeLayoutGetWidth(parentNode);
        float childLeft = YGNodeLayoutGetLeft(childNode);
        float childWidth = YGNodeLayoutGetWidth(childNode);

        if (YGNodeStyleGetFlexDirection(parentNode) == YGFlexDirectionRowReverse) {
            return parentWidth - (childLeft + childWidth);
        }
        return childLeft;
    }

    // 사용 예시
    //float correctedX = calculateCorrectedXPosition(parentNode, childNode);
    //float correctedY = calculateCorrectedYPosition(parentNode, childNode);

    /*
     Wrap 처리 방식
     Wrap이 활성화된 상태에서는 각 줄의 자식들의 최대 폭과 모든 줄의 총 높이를 계산해야 합니다. 이를 위해 다음과 같은 로직이 필요합니다:

     콘텐츠 크기 계산 (Wrap 지원)
     */
    std::pair<float, float> calculateContentSizeWithWrap(YGNodeRef parentNode) {
        float maxRowWidth = 0.0f; // 현재 줄에서 가장 큰 너비
        float totalHeight = 0.0f; // 모든 줄의 높이 합산

        float currentRowWidth = 0.0f; // 현재 줄의 누적 너비
        float currentRowHeight = 0.0f; // 현재 줄에서 가장 높은 노드의 높이

        for (uint32_t i = 0; i < YGNodeGetChildCount(parentNode); ++i) {
            YGNodeRef childNode = YGNodeGetChild(parentNode, i);
            float childWidth = YGNodeLayoutGetWidth(childNode);
            float childHeight = YGNodeLayoutGetHeight(childNode);

            // 자식 노드가 다음 줄로 넘어가는 경우 (Wrap 발생)
            if (currentRowWidth + childWidth > YGNodeLayoutGetWidth(parentNode)) {
                // 현재 줄의 너비와 높이를 Wrap 결과에 반영
                maxRowWidth = std::max(maxRowWidth, currentRowWidth);
                totalHeight += currentRowHeight;

                // 새 줄 초기화
                currentRowWidth = 0.0f;
                currentRowHeight = 0.0f;
            }

            // 현재 줄에 자식 노드 추가
            currentRowWidth += childWidth;
            currentRowHeight = std::max(currentRowHeight, childHeight);
        }

        // 마지막 줄 반영
        maxRowWidth = std::max(maxRowWidth, currentRowWidth);
        totalHeight += currentRowHeight;

        return {maxRowWidth, totalHeight};
    }

    // 사용 예시
    //auto [contentWidth, contentHeight] = calculateContentSizeWithWrap(_ygNode);
    //CCLOG("Content Size with Wrap: Width = %.2f, Height = %.2f", contentWidth, contentHeight);

    //Wrap과 Reverse가 동시에 사용될 때
    //Wrap과 Reverse가 함께 설정된 경우, 위치 보정이 필요합니다. 아래 로직은 이런 경우를 고려해 작성된 위치 보정 코드입니다:
    std::pair<float, float> calculateCorrectedPosition(YGNodeRef parentNode, YGNodeRef childNode) {
        float correctedX = YGNodeLayoutGetLeft(childNode);
        float correctedY = YGNodeLayoutGetTop(childNode);

        // RowReverse 처리
        if (YGNodeStyleGetFlexDirection(parentNode) == YGFlexDirectionRowReverse) {
            correctedX = YGNodeLayoutGetWidth(parentNode) - (correctedX + YGNodeLayoutGetWidth(childNode));
        }

        // ColumnReverse 처리
        if (YGNodeStyleGetFlexDirection(parentNode) == YGFlexDirectionColumnReverse) {
            correctedY = YGNodeLayoutGetHeight(parentNode) - (correctedY + YGNodeLayoutGetHeight(childNode));
        }

        return {correctedX, correctedY};
    }

    // 사용 예시
    //auto [correctedX, correctedY] = calculateCorrectedPosition(parentNode, childNode);
    //CCLOG("Corrected Position: X = %.2f, Y = %.2f", correctedX, correctedY);


    //최종 조합
    //Wrap과 Reverse를 모두 고려한 콘텐츠 크기와 위치 계산을 종합하면 다음과 같습니다:
    void layoutWithWrapAndReverse(YGNodeRef parentNode) {
        auto [contentWidth, contentHeight] = calculateContentSizeWithWrap(parentNode);

        for (uint32_t i = 0; i < YGNodeGetChildCount(parentNode); ++i) {
            YGNodeRef childNode = YGNodeGetChild(parentNode, i);
            auto [correctedX, correctedY] = calculateCorrectedPosition(parentNode, childNode);

            CCLOG("Child %d: Corrected Position: X = %.2f, Y = %.2f", i, correctedX, correctedY);
        }

        CCLOG("Total Content Size: Width = %.2f, Height = %.2f", contentWidth, contentHeight);
    }

    /*
     어떤 근거로 작성했는지?
     Yoga 문서
     Yoga는 Facebook이 만든 레이아웃 엔진으로 Flexbox의 동작 원칙을 따릅니다.
     제가 제안한 calculateContentSizeWithWrap와 같은 로직은 Yoga의 레이아웃 계산 원칙을 해석하고, 실제 적용 방법을 C++ 코드로 구현한 것입니다.

     참조 문서:

     Yoga Layout Documentation (GitHub)
     Cocos2d-x의 Node 시스템
     Cocos2d-x의 Node는 기본적으로 자식 노드들을 계층적으로 관리하며, 상대 좌표와 크기를 다룹니다. 하지만 Yoga를 사용하면서 자체적으로 크기와 위치를 계산해야 하는 경우가 많습니다.
     이를 반영해 getChildren()을 이용한 순회 방식과 좌표 보정 방법을 결합했습니다.

     Flexbox와 Wrap 처리
     Flexbox의 Wrap은 콘텐츠를 여러 줄로 나누는 동작으로, 한 줄의 너비와 각 줄의 높이를 별도로 계산해야 합니다.
     이런 개념은 Flexbox CSS 표준과 Yoga 엔진의 Wrap 구현을 참고했습니다.

     Reverse 보정
     RowReverse나 ColumnReverse가 활성화되면 좌표 체계가 반전됩니다. 이를 보정하기 위해 Yoga의 레이아웃 계산 값을 기반으로 좌표를 재계산하는 로직을 추가한 것입니다.


     */

    /*
    void FlexNode::layoutAndSetSize(float width = YGUndefined, float height = YGUndefined) {
        // 레이아웃 계산
        YGNodeCalculateLayout(_ygNode, width, height, YGNodeLayoutGetDirection(_ygNode));

        // 현재 노드의 크기 및 위치 계산
        auto correctedPosition = calculateCorrectedPosition(_ygNode, width, height);
        float posX = correctedPosition.x;
        float posY = correctedPosition.y;
        float posW = YGNodeLayoutGetWidth(_ygNode);
        float posH = YGNodeLayoutGetHeight(_ygNode);

        // Cocos2d-x 노드에 설정
        if (_cocosNode != nullptr) {
            _cocosNode->setPosition(cocos2d::Vec2(posX, posY));
            _cocosNode->setContentSize(cocos2d::Size(posW, posH));
        }

        // 자식 노드의 Wrap 내용을 고려하여 크기 조정
        auto contentSize = this->calculateContentSizeWithWrap(_ygNode, posW, posH);
        float contentW = contentSize.width;
        float contentH = contentSize.height;

        // 하위 노드 처리
        for (uint32_t i = 0; i < YGNodeGetChildCount(_ygNode); i++) {
            auto child = _children.at(i);
            if (child) {
                child->layoutAndSetSize(contentW, contentH);
            }
        }
    }
     */

      
};



#endif /* FlexNode_hpp */
