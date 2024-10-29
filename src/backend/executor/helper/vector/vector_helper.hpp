#ifndef VECTOR_HELPER_HPP
#define VECTOR_HELPER_HPP

#include <random>
#include <vector>
#include <optional>

struct VectorHelper {
    VectorHelper() = delete;

    template<typename T>
    static std::optional<T> GetRandomObject(const std::vector<T>& vec);
};

template<typename T>
std::optional<T> VectorHelper::GetRandomObject(const std::vector<T>& vec) {
    if (vec.empty()) return std::nullopt;

    static std::random_device rd;
    static std::mt19937 generator(rd());
    std::uniform_int_distribution<std::size_t> dis(0, vec.size() - 1);

    return vec[dis(generator)];
}

#endif // VECTOR_HELPER_HPP
