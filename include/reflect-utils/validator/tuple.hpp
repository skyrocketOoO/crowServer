#pragma once

#include <vector>
#include <array>


template <typename Tuple, typename Func, std::size_t... Indices>
void forEachInTuple(const Tuple& tuple, Func&& func, std::index_sequence<Indices...>) {
    (func(std::get<Indices>(tuple)), ...);
}

template <typename Tuple, typename Func>
void forEachInTuple(const Tuple& tuple, Func&& func) {
    forEachInTuple(tuple, std::forward<Func>(func), std::make_index_sequence<std::tuple_size_v<Tuple>>{});
}