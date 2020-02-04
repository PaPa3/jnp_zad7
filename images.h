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

template<typename T>
Base_image<T> constant(T t) {
    return [t = std::move(t)](const Point &p) {
        (void) p;
        return t;
    };
}

template<typename T>
Base_image<T> rotate(Base_image<T> image, double phi) {
    return compose(to_polar,
                   [image = std::move(image), phi](const Point &p) { return Point(p.first, p.second - phi, true); },
                   from_polar,
                   image);
}

template<typename T>
Base_image<T> translate(Base_image<T> image, const Vector v) {
    return lift(image,
                [image = std::move(image), v](const Point &p) {
                    return Point(p.first - v.first, p.second - v.second);
                });
}

template<typename T>
Base_image<T> scale(Base_image<T> image, double s) {
    return lift(image,
                [image = std::move(image), s](const Point &p) {
                    return Point(p.first / s, p.second / s);
                });
}

template<typename T>
Base_image<T> circle(Point q, double r, T inner, T outer) {
    return [=](const Point &p) {
        return distance(q, p) < r ? inner : outer;
    };
}

template<typename T>
Base_image<T> checker(double d, T this_way, T that_way) {
    return [=](const Point &p) {
        int x = std::floor(p.first / d);
        int y = std::floor(p.second / d);
        return (x + y) % 2 == 0 ? this_way : that_way;
    };
}

template<typename T>
Base_image<T> polar_checker(double d, int n, T this_way, T that_way) {
    return compose(to_polar,
                   [=](const Point &p) { return Point(p.first, p.second * d * n / (2 * M_PI), true); },
                   checker(d, this_way, that_way));
}

template<typename T>
Base_image<T> rings(Point q, double d, T this_way, T that_way) {
    return translate(polar_checker(d, 1, this_way, that_way), Vector(q.first, q.second));
}

template<typename T>
Base_image<T> vertical_stripe(double d, T this_way, T that_way) {
    return [=](const Point &p) {
        return std::abs(p.first) * 2 < d ? this_way : that_way;
    };
}

Image cond(const Region &region, const Image &this_way, const Image &that_way);

Image lerp(const Blend &blend, const Image &this_way, const Image &that_way);

Image darken(const Image &image, const Blend &blend);

Image lighten(const Image &image, const Blend &blend);

#endif //IMAGES_H
