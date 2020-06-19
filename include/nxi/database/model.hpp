#ifndef INCLUDE_NXI_DATABASE_MODEL_HPP_NXI
#define INCLUDE_NXI_DATABASE_MODEL_HPP_NXI

#include <nxi/database/field.hpp>
#include <nxi/data/page.hpp>

inline constexpr static struct nxi_model_
{
    inline constexpr nxi::data::page::internal::table page{};
} nxi_model;

#endif // INCLUDE_NXI_DATABASE_MODEL_HPP_NXI
