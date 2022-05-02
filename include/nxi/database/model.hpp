#ifndef INCLUDE_NXI_DATABASE_MODEL_HPP_NXI
#define INCLUDE_NXI_DATABASE_MODEL_HPP_NXI

#include <nxi/database/field.hpp>
#include <nxi/data/config.hpp>
#include <nxi/data/context.hpp>
#include <nxi/data/cookie.hpp>
#include <nxi/data/module.hpp>
#include <nxi/data/page.hpp>
#include <nxi/data/session.hpp>
#include <nxi/data/window.hpp>

inline constexpr static struct nxi_model_
{
    nxi::data::config::internal::table config{};
    nxi::data::context::internal::table_context context{};
    nxi::data::cookie::internal::table_cookie cookie{};
    nxi::data::module::internal::table_module module{};
    nxi::data::page::internal::table_page page{};
    nxi::data::page::internal::table_page_arc page_arc{};
    nxi::data::session::internal::table session{};
    nxi::data::window::internal::table window{};
} nxi_model;

#endif // INCLUDE_NXI_DATABASE_MODEL_HPP_NXI