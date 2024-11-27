//
//  CCFlexWrapLayout.hpp
//  cocos2d_libs
//
//  Created by 선명수 on 11/27/24.
//

#ifndef CCFlexWrapLayout_h
#define CCFlexWrapLayout_h

#include "ui/CocosGUI.h"

using namespace cocos2d::ui;

class CCNodeWrapLayout : public cocos2d::ui::Layout {
public:
    static CCNodeWrapLayout* createWrap(cocos2d::Node* node) {
        auto layout = new (std::nothrow) CCNodeWrapLayout();
        if (layout && layout->init()) {
            layout->autorelease();
            if (node) {
                //디폴트.
                layout->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
                layout->addInnerNode(node);
            }
            return layout;
        }
        CC_SAFE_DELETE(layout);
        return nullptr;
    }

    void addInnerNode(cocos2d::Node* node) {
        innerNode_ = node;
        if (innerNode_) {
            innerNode_->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
            innerNode_->setPosition(getContentSize() / 2);
            this->addChild(innerNode_);
            syncNodeSize(); // 초기 크기 동기화
        }
    }

    void setContentSize(const cocos2d::Size& size) override {
        cocos2d::ui::Layout::setContentSize(size);
        if (innerNode_) {
            innerNode_->setPosition(getContentSize() / 2); // 중앙 배치
            syncNodeSize(); // 크기 동기화
        }
    }

private:
    cocos2d::Node* innerNode_ = nullptr;

    void syncNodeSize() {
        // Node 크기 조정 (Node가 `setContentSize`를 지원하는 경우만)
        if (innerNode_) {
            const auto layoutSize = getContentSize();

            // Node의 크기 조정 지원 여부 확인
            auto resizable = dynamic_cast<cocos2d::ui::Widget*>(innerNode_);
            if (resizable) {
                // Widget 타입은 크기를 직접 설정 가능
                resizable->setContentSize(layoutSize);
            } else {
                // 일반 Node는 스케일로 크기 조정
                const auto nodeOriginalSize = innerNode_->getContentSize();
                if (nodeOriginalSize.width > 0 && nodeOriginalSize.height > 0) {
                    float scaleX = layoutSize.width / nodeOriginalSize.width;
                    float scaleY = layoutSize.height / nodeOriginalSize.height;
                    innerNode_->setScaleX(scaleX);
                    innerNode_->setScaleY(scaleY);
                }
            }
        }
    }
};


#endif /* CCFlexWrapLayout_hpp */
