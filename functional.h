#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H

/** Function composition.
 * @return Identity function.
 */
inline auto compose() {
    return [](auto x) {
        return x;
    };
}

/** Function composition.
 * Giving functions f_1(x), f_2(x), ... , f_n(x) returns f_n(...f_2(f_1(x))).
 * @tparam T        - type of f_1(x);
 * @tparam TArgs    - types of f_2(x), ... , f_n(x);
 * @param f         - f_1(x);
 * @param args      - f_2(x), ... , f_n(x).
 * @return f_n(...f_2(f_1(x))).
 */
template<typename T, typename... TArgs>
auto compose(T f, TArgs... args) {
    return [=](auto x) {
        return compose(args...)(f(x));
    };
}

/** Pointwise lifting.
 * @tparam T        - type of h;
 * @tparam TArgs    - types of f_1(x), ... , f_n(x);
 * @param h         - h(x);
 * @param args      - f_1(x), ... , f_n(x).
 * @return Function g(x) = h(f_1(x), f_2(x), ... , f_n(x)).
 */
template<typename T, typename... TArgs>
auto lift(T h, TArgs... f) {
    return [=](auto p) {
        return h(f(p)...);
    };
}

#endif //FUNCTIONAL_H
