#ifndef IMAGES_H
#define IMAGES_H

#include <functional>
#include "functional.h"
#include "coordinate.h"
#include "color.h"

using Fraction = double;

template<typename T>
using Base_image = std::function<T(const Point)>;

using Region = Base_image<bool>;

using Image = Base_image<Color>;

using Blend = Base_image<Fraction>;

/** Creates constant image.
 * @tparam T        - type of every pixel;
 * @param t         - value of every pixel.
 * @return Base_image that represents constant image.
 */
template<typename T>
inline Base_image<T> constant(const T &t) {
    return [=](const Point &p) {
        (void) p;
        return t;
    };
}

/** Rotates an image by angle.
 * @tparam T        - type of the image;
 * @param image     - image to rotate;
 * @param phi       - angle.
 * @return Base_image that represents rotated image.
 */
template<typename T>
inline Base_image<T> rotate(const Base_image<T> &image, double phi) {
    return compose(to_polar,
                   [=](const Point &p) { return Point(p.first, p.second - phi, true); },
                   from_polar,
                   image);
}

/** Translates image by vector.
 * @tparam T        - type of the image;
 * @param image     - image to translate;
 * @param v         - vector.
 * @return Base_image that represents translated image.
 */
template<typename T>
inline Base_image<T> translate(const Base_image<T> &image, const Vector &v) {
    return lift(image,
                [=](const Point &p) { return Point(p.first - v.first, p.second - v.second); });
}

/** Scales an image by value.
 * @tparam T        - type of the image;
 * @param image     - image;
 * @param s         - value.
 * @return Base_image that represents scaled image.
 */
template<typename T>
inline Base_image<T> scale(const Base_image<T> &image, double s) {
    return lift(image,
                [=](const Point &p) { return Point(p.first / s, p.second / s); });
}

/** Creates an image with circle.
 * @tparam T        - type of the image;
 * @param q         - middle of the circle;
 * @param r         - radius of the circle;
 * @param inner     - pixels inside the circle;
 * @param outer     - pixels outside the circle.
 * @return Base_image that represents image with circle.
 */
template<typename T>
inline Base_image<T> circle(const Point &q, double r, const T &inner, const T &outer) {
    return [=](const Point &p) {
        return distance(q, p) < r ? inner : outer;
    };
}

/** Creates an image with checker.
 * @tparam T        - type of the image;
 * @param d         - side length of squares in desired checker;
 * @param this_way  - pixels on "black" pieces;
 * @param that_way  - pixels on "white" pieces;
 * @return Base_image that represents image with checker.
 */
template<typename T>
inline Base_image<T> checker(double d, const T &this_way, const T &that_way) {
    return [=](const Point &p) {
        int x = std::floor(p.first / d);
        int y = std::floor(p.second / d);
        return (x + y) % 2 == 0 ? this_way : that_way;
    };
}

/** Creates an image with checker in polar axis.
 * @tparam T        - type of the image;
 * @param d         - length of pieces in desired checker;
 * @param n         - number of radiant pieces;
 * @param this_way  - pixels on "black" pieces;
 * @param that_way  - pixels on "white" pieces;
 * @return Base_image that represents image with checker in polar axis.
 */
template<typename T>
inline Base_image<T> polar_checker(double d, int n, const T &this_way, const T &that_way) {
    return compose(to_polar,
                   [=](const Point &p) { return Point(p.first, p.second * d * n / (2 * M_PI), true); },
                   checker(d, this_way, that_way));
}

/** Creates an image with rings.
 * @tparam T        - type of the image;
 * @param q         - middle of the circle;
 * @param d         - width of each ring;
 * @param this_way  - pixels on pieces inside rings;
 * @param that_way  - pixels on pieces outside rings.
 * @return Base_image that represents image with rings.
 */
template<typename T>
inline Base_image<T> rings(const Point &q, double d, const T &this_way, const T &that_way) {
    return translate(polar_checker(d, 1, this_way, that_way), Vector(q.first, q.second));
}

/** Creates an image with vertical stripe.
 * @tparam T        - type of the image;
 * @param d         - width of the stripe;
 * @param this_way  - pixels inside the stripe;
 * @param that_way  - pixels outside the stripe.
 * @return Base_image that represents image vertical stripe.
 */
template<typename T>
inline Base_image<T> vertical_stripe(double d, const T &this_way, const T &that_way) {
    return [=](const Point &p) {
        return std::abs(p.first) * 2 < d ? this_way : that_way;
    };
}

/** Puts one image on another.
 * @param region    - region containing first image;
 * @param this_way  - first image;
 * @param that_way  - second image.
 * @return Image that represents one image on another.
 */
inline Image cond(const Region &region, const Image &this_way, const Image &that_way) {
    return [=](const Point &p) {
        return region(p) ? this_way(p) : that_way(p);
    };
}

/** Mixes two images.
 * @param blend     - percent of first image in result;
 * @param this_way  - first image;
 * @param that_way  - second image.
 * @return Image that represents two mixed images
 */
inline Image lerp(const Blend &blend, const Image &this_way, const Image &that_way) {
    return [=](const Point &p) {
        return this_way(p).weighted_mean(that_way(p), blend(p));
    };
}

/** Mixes image with black image.
 * @param image     - image.
 * @param blend     - percent of given image in result;
 * @return Image that represents two mixed images
 */
inline Image darken(const Image& image, const Blend& blend) {
    static const Image black = constant<Color>(Colors::black);
    return lerp(blend, image, black);
}

/** Mixes image with white image.
 * @param image     - image.
 * @param blend     - percent of given image in result;
 * @return Image that represents two mixed images
 */
inline Image lighten(const Image& image, const Blend& blend) {
    static const Image white = constant<Color>(Colors::white);
    return lerp(blend, image, white);
}

#endif //IMAGES_H
