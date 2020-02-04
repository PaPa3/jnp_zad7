#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H

/** Function composition.
 * @return Identity function.
 */
constexpr auto compose() {
    return [](auto x) { return x; };
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
constexpr auto compose(T f, TArgs... args) {
    return [=](auto x) { return compose(args...)(f(x)); };
}

/** Pointwise lifting.
 * @tparam T        - type of h(x);
 * @param h         - function h(x).
 * @return Function h(x).
 */
template<typename T>
constexpr auto lift(T h) {
    return [=](auto p) {
        (void) p;
        return h();
    };
}

/** Pointwise lifting.
 * @tparam T1       - type of h;
 * @tparam T2       - type of f_1(x);
 * @tparam TArgs    - types of f_2(x), ... , f_n(x);
 * @param h         - h(x);
 * @param f         - f_1(x);
 * @param args      - f_2(x), ... , f_n(x).
 * @return Function g(x) = h(f_1(x), f_2(x), ... , f_n(x)).
 */
template<typename T1, typename T2, typename... TArgs>
constexpr auto lift(T1 h, T2 f, TArgs... args) {
    return [=](auto p) {
        return lift([=](auto... args2) { return h(f(p), args2...); }, args...)(p);
    };
}

#endif //FUNCTIONAL_H
