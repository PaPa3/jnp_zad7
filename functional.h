#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H

constexpr auto compose() {
    return [](auto x) { return x; };
}

template<typename T, typename... TArgs>
constexpr auto compose(T f, TArgs... args) {
    return [=](auto x) { return compose(args...)(f(x)); };
}

template<typename T>
constexpr auto lift(T h) {
    return [=](auto p) {
        (void) p;
        return h();
    };
}

template<typename T1, typename T2, typename... TArgs>
constexpr auto lift(T1 h, T2 f, TArgs... args) {
    return [=](auto p) {
        return lift([=](auto... args2) { return h(f(p), args2...); }, args...)(p);
    };
}

#endif //FUNCTIONAL_H
