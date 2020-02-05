#include "images.h"

// This function is the only one that failed to inline (flag -Winline),
// so I declare it here instead of in header file.
Image lerp(const Blend &blend, const Image &this_way, const Image &that_way) {
    return lift([](double blend, Color this_way, Color that_way) {
                    return this_way.weighted_mean(that_way, blend);
                },
                blend,
                this_way,
                that_way);
}
