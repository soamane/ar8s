#ifndef VECTOR_HELPER_HPP
#define VECTOR_HELPER_HPP

#include <random>
#include <vector>
#include <optional>

struct VectorHelper {
    VectorHelper() = delete;

    template<class T>
    static std::optional<T> GetRandomObject(const std::vector<T>& vec);
};

template<class T>
inline std::optional<T> VectorHelper::GetRandomObject(const std::vector<T>& vec) {
    if (vec.empty()) {
        return std::nullopt;
    }

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<std::size_t> dis(0, vec.size() - 1);

    return vec[dis(generator)];
}

#endif // !VECTOR_HELPER_HPP