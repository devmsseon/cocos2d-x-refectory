#ifndef FLEXCONTAINER_H
#define FLEXCONTAINER_H

#include <memory>
#include <functional>
#include <yoga/Yoga.h>
#include "cocos2d.h"
#include "FlexNode.h"

// FlexContainer 클래스 선언
class FlexContainer {
private:
    //_rootNode는 std::shared_ptr로 관리되므로, 직접 해제하지 않아도 됩니다. std::shared_ptr는 소유권을 관리하며, 참조 카운트가 0이 되는 순간 자동으로 메모리를 해제합니다.
    std::shared_ptr<FlexNode> _rootNode;

public:
    FlexContainer();
    ~FlexContainer();
    void setWidth(float width);
    void setHeight(float height);

    std::shared_ptr<FlexNode> getRootNode() const;
    cocos2d::Node* getCocosNode();

    FlexContainer& setPadding(YGEdge edgeType, float edgeValue);
    FlexContainer& setMargin(YGEdge edgeType, float edgeValue);
    FlexContainer& setAlignItems(YGAlign align);
    FlexContainer& setAlignSelf(YGAlign align);
    FlexContainer& setAlignContent(YGAlign align);
    FlexContainer& setJustifyContent(YGJustify justify);
    FlexContainer& setFlexDirection(YGFlexDirection direction = YGFlexDirectionColumn);
    FlexContainer& setFlexWrap(YGWrap wrap);
    FlexContainer& addChild(const std::shared_ptr<FlexNode>& child);
    FlexContainer& define(const std::function<void(FlexNode&)>& defineFunc);

    float getContentWidth();
    float getContentHeight();

    void layoutDrawForMain(float width = YGUndefined, float height = YGUndefined, FlexNode* refFlexNode = nullptr);
    void layoutDrawForSub(float width = YGUndefined, float height = YGUndefined, FlexNode* refFlexNode = nullptr);
};

#endif /* FLEXCONTAINER_H */
