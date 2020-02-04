#include "images.h"

#include <utility>

Image cond(const Region &region, const Image &this_way, const Image &that_way) {
    return [=](const Point &p) {
        return region(p) ? this_way(p) : that_way(p);
    };
}

Image lerp(const Blend &blend, const Image &this_way, const Image &that_way) {
    return [=](const Point &p) {
        return this_way(p).weighted_mean(that_way(p), blend(p));
    };
    //return lift();
}

Image darken(const Image& image, const Blend& blend) {
    static const Image black = constant<Color>(Colors::black);
    return lerp(blend, image, black);
}

Image lighten(const Image& image, const Blend& blend) {
    static const Image white = constant<Color>(Colors::white);
    return lerp(blend, image, white);
}
