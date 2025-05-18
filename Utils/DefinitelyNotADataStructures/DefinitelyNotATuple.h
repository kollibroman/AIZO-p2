#ifndef DEFINITELYNOTATUPLE_H
#define DEFINITELYNOTATUPLE_H

template<typename T1, typename T2, typename T3>
class DefinitelyNotATuple {
private:
    T1 first_;
    T2 second_;
    T3 third_;

public:
    DefinitelyNotATuple() = default;

    DefinitelyNotATuple(const T1& first, const T2& second, const T3& third)
        : first_(first), second_(second), third_(third) {}

    DefinitelyNotATuple(T1&& first, T2&& second, T3&& third)
        : first_(std::move(first)), second_(std::move(second)), third_(std::move(third)) {}

    // Getters
    const T1& first() const { return first_; }
    const T2& second() const { return second_; }
    const T3& third() const { return third_; }

    T1& first() { return first_; }
    T2& second() { return second_; }
    T3& third() { return third_; }

    // Structured binding support
    template<size_t I>
    auto& get() & {
        if constexpr (I == 0) return first_;
        else if constexpr (I == 1) return second_;
        else if constexpr (I == 2) return third_;
    }

    template<size_t I>
    const auto& get() const & {
        if constexpr (I == 0) return first_;
        else if constexpr (I == 1) return second_;
        else if constexpr (I == 2) return third_;
    }
};


#endif //DEFINITELYNOTATUPLE_H
