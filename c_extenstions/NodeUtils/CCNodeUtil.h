/*
 *   FILE: NodeUtil.h
 * AUTHOR: Michael David Plotz
 *   DATE: Fri Mar 15 19:21:37 PDT 2013
 */

#ifndef __COCOS2DNODEUTIL_H__
#define __COCOS2DNODEUTIL_H__

#include <iomanip>

#include "base/ccTypes.h"
//#include "ccTypes.h"
#include "cocos2d.h"

#define NS_CC_UTIL_BEGIN namespace cocos2dutil {
#define NS_CC_UTIL_END   }
#define USING_NS_CC_UTIL using namespace cocos2dutil

#define lerp(__X0__, __X1__, __T__) \
    ((__X0__) + (__T__) * ((__X1__) - (__X0__)))

#define CCLOG_POINT(__desc__, __point__) \
    CCLog("%s point: (%f, %f)", (__desc__), (__point__).x, (__point__).y)

#define CCLOG_SIZE(__desc__, __size__) \
    CCLog("%s size: (%f, %f)", (__desc__), (__size__).width, (__size__).height)

#define M_TWO_PI (2.0 * M_PI)
#define M_THREE_PI_2 (1.5 * M_PI)

#define LARGE_INT 0x10000000

USING_NS_CC;

NS_CC_UTIL_BEGIN

// ========================================================================== //
// typedefs                                                                   //

typedef std::list<Node *> NodeList;

// ========================================================================== //
// globals                                                                    //

enum Alignment
{
    ALIGNMENT_NONE                   = 0x00,

    // pure flags
    ALIGNMENT_LEFT                   = 0x01,
    ALIGNMENT_CENTER_X               = 0x02,
    ALIGNMENT_RIGHT                  = 0x04,
    ALIGNMENT_TOP                    = 0x10,
    ALIGNMENT_CENTER_Y               = 0x20,
    ALIGNMENT_BOTTOM                 = 0x40,

    // combinations
    ALIGNMENT_TOP_LEFT               = 0x11,
    ALIGNMENT_TOP_CENTER             = 0x12,
    ALIGNMENT_TOP_RIGHT              = 0x14,
    ALIGNMENT_CENTER_LEFT            = 0x21,
    ALIGNMENT_CENTER                 = 0x22,
    ALIGNMENT_CENTER_RIGHT           = 0x24,
    ALIGNMENT_BOTTOM_LEFT            = 0x41,
    ALIGNMENT_BOTTOM_CENTER          = 0x42,
    ALIGNMENT_BOTTOM_RIGHT           = 0x44
};

const Point Point_MAGIC = Point(30902126224896.0f, 66081115116603.0f);

const Point ANCHOR_CC = Point(0.5f, 0.5f);

// ========================================================================== //
// declarations                                                               //

static inline void alignNodeToParent(Node *node, Alignment nodeAlign, Alignment refAlign, const Size& padding);
static inline void alignNodeToParent(Node *node, Alignment nodeAlign, Alignment refAlign);
static inline void alignNodeToParent(Node *node, Alignment align, const Size& padding);
static inline void alignNodeToParent(Node *node, Alignment align);

static inline void alignNodeToNode(Node *node, Node *reference, Alignment nodeAlign, Alignment refAlign, const Size& padding);
static inline void alignNodeToNode(Node *node, Node *reference, Alignment nodeAlign, Alignment refAlign);
static inline void alignNodeToNode(Node *node, Node *reference, Alignment align, const Size& padding);
static inline void alignNodeToNode(Node *node, Node *reference, Alignment align);

static inline Rect getRect(Node *node);
static inline Rect& transformRect(Rect& rect, Node *srcNode, Node *dstNode);

static inline double normalizeAngle(double angle);
static inline Point centerOf(const Rect& rect);
static inline Rect outsetOf(const Rect& rect, float outset);

static std::vector<std::string> split(const std::string& s, char delim)
{
    std::vector<std::string> elements;
    std::stringstream ss(s);
    std::string token;
    while (std::getline(ss, token, delim))
    {
        elements.push_back(token);
    }
    return elements;
}

// ========================================================================== //
// cocos2d objects                                                            //

static inline Node *createSizedNode(const Size& size)
{
    Node *node = Node::create();
    node->setContentSize(size);
    return node;
}

static inline void snapToPixels(float& value)
{
    float scaleFactor = CC_CONTENT_SCALE_FACTOR();
    value = rintf(value * scaleFactor) / scaleFactor;
}

static inline void snapToPixels(Size& size)
{
    float scaleFactor = CC_CONTENT_SCALE_FACTOR();
    size.width  = rintf(size.width  * scaleFactor) / scaleFactor;
    size.height = rintf(size.height * scaleFactor) / scaleFactor;
}

static inline void snapToPixels(Point& point)
{
    float scaleFactor = CC_CONTENT_SCALE_FACTOR();
    point.x = rintf(point.x * scaleFactor) / scaleFactor;
    point.y = rintf(point.y * scaleFactor) / scaleFactor;
}

static inline void adjustNodePosition(Point& nodePos, const Point& refPos, const Size& refSize, Alignment align)
{
    if (align & ALIGNMENT_LEFT)
    {
        nodePos.x = refPos.x;
    }
    else if (align & ALIGNMENT_CENTER_X)
    {
        nodePos.x = refPos.x + 0.5f * refSize.width;
    }
    else if (align & ALIGNMENT_RIGHT)
    {
        nodePos.x = refPos.x + refSize.width;
    }

    if (align & ALIGNMENT_BOTTOM)
    {
        nodePos.y = refPos.y;
    }
    else if (align & ALIGNMENT_CENTER_Y)
    {
        nodePos.y = refPos.y + 0.5f * refSize.height;
    }
    else if (align & ALIGNMENT_TOP)
    {
        nodePos.y = refPos.y + refSize.height;
    }
}

static inline void adjustAnchorPoint(Point& anchorPt, Alignment align)
{
    if (align & ALIGNMENT_LEFT)
    {
        anchorPt.x = 0.0f;
    }
    else if (align & ALIGNMENT_CENTER_X)
    {
        anchorPt.x = 0.5f;
    }
    else if (align & ALIGNMENT_RIGHT)
    {
        anchorPt.x = 1.0f;
    }

    if (align & ALIGNMENT_BOTTOM)
    {
        anchorPt.y = 0.0f;
    }
    else if (align & ALIGNMENT_CENTER_Y)
    {
        anchorPt.y = 0.5f;
    }
    else if (align & ALIGNMENT_TOP)
    {
        anchorPt.y = 1.0f;
    }
}

static inline void addNodePadding(Point& nodePos, Alignment align, const Size& padding)
{
    if (align & ALIGNMENT_LEFT)
    {
        nodePos.x += padding.width;
    }
    else if (align & ALIGNMENT_RIGHT)
    {
        nodePos.x -= padding.width;
    }

    if (align & ALIGNMENT_BOTTOM)
    {
        nodePos.y += padding.height;
    }
    else if (align & ALIGNMENT_TOP)
    {
        nodePos.y -= padding.height;
    }
}

static inline void alignNodeToParent(Node *node, Alignment nodeAlign, Alignment refAlign, const Size& padding)
{
    Node *parent = node->getParent();
    CCAssert(parent, "Node must have a parent!");

    Size parentSize = parent->getContentSize();

    Point nodePos = node->getPosition();
    Point anchorPt = node->getAnchorPoint();

    adjustNodePosition(nodePos, CCPointZero, parentSize, refAlign);

    adjustAnchorPoint(anchorPt, nodeAlign);

    addNodePadding(nodePos, nodeAlign, padding);

    if (node->isIgnoreAnchorPointForPosition())
    {
        Size nodeSize = node->getContentSize();
        nodePos = ccpSub(nodePos, Point(anchorPt.x * nodeSize.width, anchorPt.y * nodeSize.height));
    }

    node->setPosition(nodePos);
    node->setAnchorPoint(anchorPt);
}

static inline void alignNodeToParent(Node *node, Alignment nodeAlign, Alignment refAlign)
{
    alignNodeToParent(node, nodeAlign, refAlign, CCSizeZero);
}

static inline void alignNodeToParent(Node *node, Alignment align, const Size& padding)
{
    alignNodeToParent(node, align, align, padding);
}

static inline void alignNodeToParent(Node *node, Alignment align)
{
    alignNodeToParent(node, align, align, CCSizeZero);
}

static inline void alignNodeToNode(Node *node, Node *reference, Alignment nodeAlign, Alignment refAlign, const Size& padding)
{
    CCAssert(node->getParent() && reference->getParent(), "Node and reference must have parents!");

    Point nodePos = node->getPosition();
    Point anchorPt = node->getAnchorPoint();

    Rect refRect = getRect(reference);
    if (node->getParent() != reference->getParent())
    {
        transformRect(refRect, reference->getParent(), node->getParent());
    }

    adjustNodePosition(nodePos, refRect.origin, refRect.size, refAlign);

    adjustAnchorPoint(anchorPt, nodeAlign);

    addNodePadding(nodePos, nodeAlign, padding);

    if (node->isIgnoreAnchorPointForPosition())
    {
        Size nodeSize = node->getContentSize();
        nodePos = ccpSub(nodePos, Point(anchorPt.x * nodeSize.width, anchorPt.y * nodeSize.height));
    }

    node->setPosition(nodePos);
    node->setAnchorPoint(anchorPt);
}

static inline void alignNodeToNode(Node *node, Node *reference, Alignment nodeAlign, Alignment refAlign)
{
    alignNodeToNode(node, reference, nodeAlign, refAlign, CCSizeZero);
}

static inline void alignNodeToNode(Node *node, Node *reference, Alignment align, const Size& padding)
{
    alignNodeToNode(node, reference, align, align, padding);
}

static inline void alignNodeToNode(Node *node, Node *reference, Alignment align)
{
    alignNodeToNode(node, reference, align, align, CCSizeZero);
}

static inline Rect getRect(Node *node)
{
    Rect rect;

    rect.size = node->getContentSize();
    rect.origin = node->getPosition();

    Point refAnchorPt = node->getAnchorPoint();

    if (node->isIgnoreAnchorPointForPosition())
    {
        // we're ignoring the anchor point for position but not for scale
        rect.origin = ccpAdd(rect.origin, Point(refAnchorPt.x * rect.size.width, refAnchorPt.y * rect.size.height));
    }

    rect.size.width *= node->getScaleX();
    rect.size.height *= node->getScaleY();

    rect.origin = ccpSub(rect.origin, Point(refAnchorPt.x * rect.size.width, refAnchorPt.y * rect.size.height));

    return rect;
}

static inline Point& transformPoint(Point& point, Node *srcNode, Node *dstNode)
{
    point = dstNode->convertToNodeSpace(
        srcNode->convertToWorldSpace(point)
    );
    return point;
}

static inline Rect& transformRect(Rect& rect, Node *srcNode, Node *dstNode)
{
    Point topRight = Point(rect.getMaxX(), rect.getMaxY());

    transformPoint(rect.origin, srcNode, dstNode);
    transformPoint(topRight,    srcNode, dstNode);

    rect.size.width  = topRight.x - rect.origin.x;
    rect.size.height = topRight.y - rect.origin.y;

    return rect;
}

static inline Size getScaledContentSize(Node *node)
{
    Size size = node->getContentSize();
    size.width  = fabsf(size.width  * node->getScaleX());
    size.height = fabsf(size.height * node->getScaleY());
    return size;
}

/**
 * @param float angle       angle in degrees
 */
static inline Point ccpOnNodeBoundary(Node *node, float angle, float outset)
{
    Rect rect = getRect(node);
    rect = outsetOf(rect, outset);
    Point p = Point(0.5 * rect.size.width, 0.5 * rect.size.height);

    angle = normalizeAngle(CC_DEGREES_TO_RADIANS(angle));
    float diagonalAngle = atan2f(rect.size.height, rect.size.width);

    // top
    if (angle >= diagonalAngle && angle < M_PI - diagonalAngle)
    {
        p.x += 0.5f * rect.size.height * tanf(M_PI_2 - angle);
        p.y += 0.5f * rect.size.height;
    }
    // left
    else if (angle >= M_PI - diagonalAngle && angle < M_PI + diagonalAngle)
    {
        p.x -= 0.5f * rect.size.width;
        p.y -= 0.5f * rect.size.width * tanf(angle);
    }
    // bottom
    else if (angle >= M_PI + diagonalAngle && angle < M_TWO_PI - diagonalAngle)
    {
        p.x -= 0.5f * rect.size.height * tanf(M_PI_2 - angle);
        p.y -= 0.5f * rect.size.height;
    }
    // right
    else
    {
        p.x += 0.5f * rect.size.width;
        p.y += 0.5f * rect.size.width * tanf(angle);
    }

    return p;
}

static Texture2D *makeBitmapCopy(Node *node)
{
    Point oldPos = node->getPosition();
    Point oldAnchor = node->getAnchorPoint();

    // flip, since CCRenderTexture renders upside down
    node->setScaleY(-node->getScaleY());
    node->setAnchorPoint(Point(0.0f, 1.0f));
    node->setPosition(CCPointZero);

    Size size = getScaledContentSize(node);

    CCRenderTexture *rt = CCRenderTexture::create(size.width, size.height);
    rt->begin();
    node->visit();
    rt->end();

    CCTexture2D *texture = rt->getSprite()->getTexture();
    texture->setAntiAliasTexParameters();

    node->setScaleY(-node->getScaleY());
    node->setAnchorPoint(oldAnchor);
    node->setPosition(oldPos);

    return texture;
}

// ========================================================================== //
// geometry / math                                                            //

/**
 * Normalize the angle in radians.
 */
static inline double normalizeAngle(double angle)
{
    angle = fmod(angle, M_TWO_PI);
    return angle < 0.0f ? angle + M_TWO_PI : angle;
}

static inline float cubicInterp(float x0, float x1, float x2, float x3, float t)
{
    return (               /*0.0f*/    powf(1.0f - t,  3.0f)  * x0 + 
             3.0f *      t /*1.0f*/ * (powf(1.0f - t,  2.0f)) * x1 + 
             3.0f * powf(t,  2.0f)  *      (1.0f - t)/*1.0f*/ * x2 +
                    powf(t,  3.0f)                   /*0.0f*/ * x3   );
}

static inline bool isPointInRect(const Point& point, const Rect& rect)
{
    return (point.x >= rect.origin.x && point.x <= rect.origin.x + rect.size.width)
        && (point.y >= rect.origin.y && point.y <= rect.origin.y + rect.size.height);
}

static inline Point centerOf(const Size& size)
{
    return Point(
        0.5 * size.width,
        0.5 * size.height
    );
}

static inline Point centerOf(const Rect& rect)
{
    return Point(
        rect.origin.x + 0.5 * rect.size.width,
        rect.origin.y + 0.5 * rect.size.height
    );
}

static inline Point centerOf(Node *node)
{
    Point pos = node->getPosition();
    Size size = node->getContentSize();
    Point anchor = node->isIgnoreAnchorPointForPosition() ?
        CCPointZero :
        node->getAnchorPoint();
    return Point(
        pos.x + (0.5f - anchor.x) * size.width,
        pos.y + (0.5f - anchor.y) * size.height
    );
}

/**
 * Restores the node's anchor point to the center while keeping its position
 * intact
 */
static inline void recenterAnchorPoint(Node *node)
{
    Point oldPos = node->getPosition();
    Point anchor = node->getAnchorPoint();
    Size size = getScaledContentSize(node);
    node->setPosition(Point(
        oldPos.x + (0.5f - anchor.x) * size.width,
        oldPos.y + (0.5f - anchor.y) * size.height
    ));
    node->setAnchorPoint(Point(0.5f, 0.5f));
}

static inline Rect insetOf(const Rect& rect, const Size& inset)
{
    Rect newRect;
    newRect.origin.x = rect.origin.x + inset.width;
    newRect.origin.y = rect.origin.y + inset.height;
    newRect.size.width  = fmaxf(0.0f, rect.size.width  - 2.0f * inset.width );
    newRect.size.height = fmaxf(0.0f, rect.size.height - 2.0f * inset.height);
    return newRect;
}

static inline Size outsetOf(const Size& size, const Size& outset)
{
    Size newSize;
    newSize.width  = fmaxf(0.0f, size.width  + 2.0f * outset.width );
    newSize.height = fmaxf(0.0f, size.height + 2.0f * outset.height);
    return newSize;
}

static inline Rect outsetOf(const Rect& rect, const Size& outset)
{
    Rect newRect;
    newRect.origin.x = rect.origin.x - outset.width;
    newRect.origin.y = rect.origin.y - outset.height;
    newRect.size.width  = fmaxf(0.0f, rect.size.width  + 2.0f * outset.width);
    newRect.size.height = fmaxf(0.0f, rect.size.height + 2.0f * outset.height);
    return newRect;
}

static inline Rect outsetOf(const Rect& rect, float outset)
{
    Rect newRect;
    newRect.origin.x = rect.origin.x - outset;
    newRect.origin.y = rect.origin.y - outset;
    newRect.size.width  = fmaxf(0.0f, rect.size.width  + 2.0f * outset);
    newRect.size.height = fmaxf(0.0f, rect.size.height + 2.0f * outset);
    return newRect;
}

static inline Rect getIntersection(const Rect& rectA, const Rect& rectB)
{
    if (!rectA.intersectsRect(rectB))
    {
        return CCRectZero;
    }

    Rect r;
    r.origin.x    = fmaxf(rectA.getMinX(), rectB.getMinX());
    r.origin.y    = fmaxf(rectA.getMinY(), rectB.getMinY());
    r.size.width  = fminf(rectA.getMaxX(), rectB.getMaxX()) - r.origin.x;
    r.size.height = fminf(rectA.getMaxY(), rectB.getMaxY()) - r.origin.y;

    return r;
}

static inline float azimuth(const Point& a, const Point& b)
{
    return atan2f(b.y - a.y, b.x - a.x);
}

// ========================================================================== //
// random numbers                                                             //

#define ARC4RANDOM_MAX 0x100000000
#define rand_uniform arc4random_uniform

static inline double rand_0_1()
{
    return ((double)arc4random() / ARC4RANDOM_MAX);
}

static inline double rand_range(double min, double max)
{
    return lerp(min, max, rand_0_1());
}

template <typename T>
T weighted_random(const std::map<T, float>& m)
{
    float sum = 0;
    typename std::map<T, float>::const_iterator iter;
    for (iter = m.begin(); iter != m.end(); ++iter)
    {
        sum += iter->second;
    }

    float sample = rand_range(0.0, sum);

    sum = 0;
    for (iter = m.begin(); iter != m.end(); ++iter)
    {
        sum += iter->second;
        if (sample <= sum)
        {
            return iter->first;
        }
    }

    return NULL;
}

// ========================================================================== //
// strings                                                                    //

#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

// number formatting

class custom_numpunct : public std::numpunct<char>
{
protected:
    virtual char do_thousands_sep() const
    {
        return ',';
    }
    virtual std::string do_grouping() const
    {
        return "\03";
    }
};

// int to string
static inline std::string toString(int value)
{
    std::stringstream ss;
    std::locale custom_locale(std::locale(), new custom_numpunct());
    ss.imbue(custom_locale);
    ss << std::fixed << value;
    return ss.str();
}

static inline std::string formatAsPercentage(float proportion)
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(0) << (100.0f * proportion) << "%";
    return ss.str();
}

// ========================================================================== //
// colors                                                                     //

static inline void lerpColor3B(ccColor3B& result, const ccColor3B& c0, const ccColor3B& c1, float t)
{
    result.r = c0.r + t * (c1.r - c0.r);
    result.g = c0.g + t * (c1.g - c0.g);
    result.b = c0.b + t * (c1.b - c0.b);
}

static inline void lerpColor4B(ccColor4B& result, const ccColor4B& c0, const ccColor4B& c1, float t)
{
    result.r = c0.r + t * (c1.r - c0.r);
    result.g = c0.g + t * (c1.g - c0.g);
    result.b = c0.b + t * (c1.b - c0.b);
    result.a = c0.a + t * (c1.a - c0.a);
}

static inline ccColor3B ccc3BFromccc4F(ccColor4F c)
{
    ccColor3B c3 = {GLubyte(255.0f * c.r), GLubyte(255.0f * c.g), GLubyte(255.0f * c.b)};
    return c3;
}

static inline ccColor4B ccc4BFromccc3B(ccColor3B c)
{
    ccColor4B c4 = {c.r, c.g, c.b, 0xff};
    return c4;
}

static inline ccColor3B ccc3BFromccc4B(ccColor4B c)
{
    ccColor3B c3 = {c.r, c.g, c.b};
    return c3;
}

static inline ccColor4B ccc4BFromHexString(const char *hexString)
{
    unsigned int len = strnlen(hexString, 11);
    CCAssert(len == 6 || len == 8 || len == 10, "Malformed color string");

    const char *str = hexString;
    if (strncmp("0x", str, 2) == 0)
    {
        str += 2;
    }

    len = strnlen(str, 9);
    CCAssert(len == 6 || len == 8, "Malformed color string");

    if (len == 6)
    {
        unsigned int intVal = strtoul(str, NULL, 16);
        ccColor4B c4 = {
            static_cast<GLubyte>((intVal & 0xff0000) >> 16),
            static_cast<GLubyte>((intVal & 0x00ff00) >>  8),
            static_cast<GLubyte>((intVal & 0x0000ff)),
            0xff
        };
        return c4;
    }
    else
    {
        unsigned int intVal = strtoul(str, NULL, 16);
        ccColor4B c4 = {
            static_cast<GLubyte>((intVal & 0xff000000) >> 24),
            static_cast<GLubyte>((intVal & 0x00ff0000) >> 16),
            static_cast<GLubyte>((intVal & 0x0000ff00) >>  8),
            static_cast<GLubyte>((intVal & 0x000000ff))
        };
        return c4;
    }
}

static inline const char *hexStringFromccc4B(ccColor4B c)
{
    std::stringstream stream;
    stream << std::hex << std::setfill('0');
    stream << std::setw(2) << static_cast<unsigned>(c.r);
    stream << std::setw(2) << static_cast<unsigned>(c.g);
    stream << std::setw(2) << static_cast<unsigned>(c.b);
    stream << std::setw(2) << static_cast<unsigned>(c.a);
    return stream.str().c_str();
}

static inline void setColorAndOpacity(CCRGBAProtocol *obj, const ccColor4B& color4B)
{
    obj->setColor(ccc3BFromccc4B(color4B));
    obj->setOpacity(color4B.a);
}

NS_CC_UTIL_END

#endif /* __COCOS2DNODEUTIL_H__ */

