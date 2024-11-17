
#include "Layout2.h"
#include <assert.h>

Size Layout2::GetContentSize ( Node* n )
{
    Size size;
    if ( dynamic_cast<ScrollView*> ( n ) )
    {
        size = dynamic_cast<ScrollView*> ( n )->getViewSize();
    }
    else if ( dynamic_cast<Label*> ( n ) )
    {
        Label* t = dynamic_cast<Label*> ( n );
        size = t->getContentSize();
        if ( size.height == 0 )
        {
            size.height = t->getBMFontSize();
        }
    }
    else
    {
        size = n->getContentSize();
    }

    if ( size.width == 0 )
    {
        size.width = 1;
    }
    if ( size.height == 0 )
    {
        size.height = 1;
    }
    // assert (size.width != 0 && size.height != 0 );
    return size;
}

void  Layout2::GetWorldRect ( Node* n, CCPoint& w_left_bottom, CCPoint& w_right_top, CCPoint& w_center )
{
    CCRect  r;
    GetWorldRect ( n, r );
    w_left_bottom = r.origin;
    w_right_top = r.origin + r.size;
    w_center = r.origin + r.size / 2;
}

void Layout2::GetWorldRect ( Node* n, CCRect& o_rect/*out*/ )
{
    Size size = GetContentSize ( n );
    auto w_matrix = n->nodeToWorldTransform();
    size = SizeApplyAffineTransform ( size, w_matrix );
    o_rect.origin = ccp ( w_matrix.tx, w_matrix.ty );
    o_rect.size = size;

}


Node* Layout2::Align ( Node* target, Node* follower, eAlign align, CCPoint follow_anchor_or_movement , CCPoint  target_anchor /*= ccp(0.5, 0.5)*/ )
{
    CCPoint w_target_lb, w_target_rt, w_target_center;
    GetWorldRect ( target, w_target_lb, w_target_rt, w_target_center );
    Size followsize = GetContentSize ( follower );

    //get follower world pos
    CCPoint	w_from_pos = follower->convertToWorldSpace ( ccp ( followsize.width * ( follow_anchor_or_movement.x ), followsize.height * ( follow_anchor_or_movement.y ) ) );

#define ALIGN_ACCURACY 2000.0f
    //get scale from cascades,because  scale is accumulated through child hierarchy,move ccp(ALIGN_ACCURACY,ALIGN_ACCURACY) offset,then move back, get the scale from world to local
    follower->setPosition ( ccpAdd ( follower->getPosition(), ccp ( ALIGN_ACCURACY, ALIGN_ACCURACY ) ) );
    CCPoint	w_from_pos_try_move = follower->convertToWorldSpace ( ccp ( followsize.width * ( follow_anchor_or_movement.x ), followsize.height * ( follow_anchor_or_movement.y ) ) );
    CCPoint move_scale = ccp ( ( w_from_pos_try_move.x - w_from_pos.x ) / ALIGN_ACCURACY, ( w_from_pos_try_move.y - w_from_pos.y ) / ALIGN_ACCURACY );
    follower->setPosition ( ccpAdd ( follower->getPosition(), ccp ( -ALIGN_ACCURACY, -ALIGN_ACCURACY ) ) );
#undef ALIGN_ACCURACY

    CCPoint w_offset;
    switch ( align )
    {
    case LEFT:
    {
        w_offset = ccp ( w_target_lb.x - w_from_pos.x, 0 );
    }
    break;
    case RIGHT:
    {
        w_offset = ccp ( w_target_rt.x - w_from_pos.x, 0 );
    }
    break;
    case CENTER:
    {
        w_target_center.x -= ( ( w_target_rt.x - w_target_lb.x ) * ( 0.5 - target_anchor.x ) );
        w_target_center.y -= ( ( w_target_rt.y - w_target_lb.y ) * ( 0.5 - target_anchor.y ) );
        w_offset = ccpSub ( w_target_center, w_from_pos );
    }
    break;
    case TOP:
    {
        w_offset = ccp ( 0, w_target_rt.y - w_from_pos.y );
    }
    break;
    case BUTTOM:
    {
        w_offset = ccp ( 0, w_target_lb.y - w_from_pos.y );
    }
    break;
    case MOVE:
    {
        w_offset = follow_anchor_or_movement;
    }
    break;
    default:
        break;
    }

    follower->setPosition ( ccpAdd ( follower->getPosition(), ccp ( w_offset.x / move_scale.x, w_offset.y / move_scale.y ) ) );
//    DebugDrawLayer::Get()->AddNode (target);
    return follower;
}

Node* Layout2::__Left ( float anchorX, Node* target, Node* follow )
{
    return Align ( target, follow, LEFT, ccp ( anchorX, 0 ) );
}

Node* Layout2::__Right ( float anchorX, Node* target, Node* follow )
{
    return Align ( target, follow, RIGHT, ccp ( anchorX, 0 ) );
}

Node* Layout2::__Top ( float anchorY, Node* target, Node* follow )
{
    return Align ( target, follow, TOP, ccp ( 0, anchorY ) );
}

Node* Layout2::__Buttom ( float anchorY, Node* target, Node* follow )
{
    return Align ( target, follow, BUTTOM, ccp ( 0, anchorY ) );
}

Node* Layout2::__Move ( CCPoint movement, Node* target )
{
    return Align ( target, target, MOVE, movement );
}

Node* Layout2::__Center ( Node* target, Node* follow, CCPoint follow_anchor /*= ccp (0.5, 0.5) */, CCPoint  target_anchor /*= ccp(0.5, 0.5)*/ )
{
    return Align ( target, follow, CENTER, follow_anchor, target_anchor );
}

void drawLine ( const CCPoint start, const CCPoint end, int width = 1 )
{
    for ( int i = 0; i < width; ++i )
    {
        ccDrawLine ( ccpAdd ( start, ccp ( -i, -i ) ), ccpAdd ( end, ccp ( -i, -i ) ) );
    }
    ccDrawLine ( start, end );
    for ( int i = 0; i < width; ++i )
    {
        ccDrawLine ( ccpAdd ( start, ccp ( i, i ) ), ccpAdd ( end, ccp ( i, i ) ) );
    }
}



void Layout2::__DebugLayout ( Node* n )
{
}

CCPoint Layout2::__Ruler ( Node* to, Node* from, CCPoint from_anchor /*= ccp (0.5, 0.5)*/, CCPoint to_anchor /*= ccp (0.5, 0.5) */ )
{
    CCRect rect_world_to;
    GetWorldRect ( to, rect_world_to );

    CCRect rect_world_from;
    GetWorldRect ( from, rect_world_from );
    return ccp ( rect_world_to.origin.x + to_anchor.x * rect_world_to.size.width - rect_world_from.origin.x - from_anchor.x * rect_world_from.size.width,
                 rect_world_to.origin.y + to_anchor.y * rect_world_to.size.height - rect_world_from.origin.y - from_anchor.y * rect_world_from.size.height
               );
}

Node* Layout2::__Middle ( Node* target1, Node* target2, Node* follow )
{
    CCPoint w_target_lb1, w_target_rt1, w_target_center1;
    GetWorldRect ( target1, w_target_lb1, w_target_rt1, w_target_center1 );

    CCPoint w_target_lb2, w_target_rt2, w_target_center2;
    GetWorldRect ( target2, w_target_lb2, w_target_rt2, w_target_center2 );

    CCPoint w_follow_will_go_to = w_target_center1 + ( w_target_center2 - w_target_center1 ) / 2;
    if ( follow->getParent() )
    {
        follow->setPosition ( follow->getParent()->convertToNodeSpace ( w_follow_will_go_to ) );
    }
    else
    {
        //world coordinate
        follow->setPosition ( w_follow_will_go_to );
    }

    return follow;
}

