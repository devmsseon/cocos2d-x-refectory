#pragma once

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include <vector>

using namespace std;
using namespace cocos2d;
using namespace cocos2d::extension;

//core macro
#define    _LEFT Layout2::__Left
#define    _RIGHT Layout2::__Right
#define    _TOP Layout2::__Top
#define    _BOTTOM Layout2::__Buttom
#define    _CENTER Layout2::__Center
#define    _MOVE Layout2::__Move
#define    _MIDDLE Layout2::__Middle
//helper macro
#define    _MLT(x,y,z,b)  _MOVE(ccp(x,y),_TOP(1,z,_LEFT(0,z,b)))
#define    _MRT(x,y,z,b)  _MOVE(ccp(x,y),_TOP(1,z,_RIGHT(1,z,b)))
#define    _MRB(x,y,z,b)  _MOVE(ccp(x,y),_BOTTOM(0,z,_RIGHT(1,z,b)))
#define    _MLC(x,y,z,b)  _MOVE(ccp(x,y),_LEFT(0,z,_CENTER(z,b)))
#define    _MRC(x,y,z,b)  _MOVE(ccp(x,y),_RIGHT(1,z,_CENTER(z,b)))
#define    _MBC(x,y,z,b)  _MOVE(ccp(x,y),_BOTTOM(0,z,_CENTER(z,b)))

class Layout2
{
public:
    enum eAlign
    {
        LEFT = 1,
        RIGHT = 1 << 1,
        CENTER = 1 << 2,
        TOP = 1 << 3,
        BUTTOM = 1 << 4,

        MOVE = 1 << 5
    };



    static Node* Align ( Node* target1, Node* follow, eAlign align, CCPoint  follow_anchor_or_movement, CCPoint  target_anchor = ccp ( 0.5, 0.5 ) );
    static Node* __Left ( float anchorX, Node* target, Node* follow );
    static Node* __Right ( float anchorX,  Node* target, Node* follow );
    static Node* __Top ( float anchorY,    Node* target, Node* follow );
    static Node* __Buttom ( float anchorY,   Node* target, Node* follow );
    static Node* __Center ( Node* target, Node* follow, CCPoint  follow_anchor = ccp ( 0.5, 0.5 ), CCPoint  target_anchor = ccp ( 0.5, 0.5 ) );
    static Node* __Middle ( Node* target1, Node* target2, Node* follow );
    static Node* __Move ( CCPoint  movement , Node* target );

    static CCPoint  __Ruler ( Node* to, Node* from, CCPoint  from_anchor = ccp ( 0.5, 0.5 ), CCPoint  to_anchor = ccp ( 0.5, 0.5 ) );
    static void GetWorldRect ( Node* n, CCPoint& w_left_bottom, CCPoint& w_right_top, CCPoint& w_center );
    static void GetWorldRect ( Node* n, CCRect& o_rect/*out*/ );
    static void __DebugLayout ( Node* n );
    static CCSize GetContentSize ( Node* n );

};

