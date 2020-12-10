#ifndef MAKE_VISITOR_H
#define MAKE_VISITOR_H

namespace utils {

template<typename ... Ts>
struct overload : Ts ... {
    using Ts::operator()...;
};

template <class... Fs>
auto make_visitor(Fs... fs)
{
    return overload<Fs...>{fs...};
}

}

#endif // MAKE_VISITOR_H
