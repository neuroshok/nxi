#ifndef INCLUDE_NXI_DATABASE_MODEL_HPP_NXI
#define INCLUDE_NXI_DATABASE_MODEL_HPP_NXI

#include <nxi/database/field.hpp>
#include <nxi/data/page.hpp>
#include <nxi/data/session.hpp>
#include <nxi/data/window.hpp>

inline constexpr static struct nxi_model_
{
    nxi::data::config::internal::table config{};
    nxi::data::page::internal::table page{};
    nxi::data::page::internal::table page_arc{};
    nxi::data::session::internal::table session{};
    nxi::data::window::internal::table window{};
} nxi_model;

#endif // INCLUDE_NXI_DATABASE_MODEL_HPP_NXI
