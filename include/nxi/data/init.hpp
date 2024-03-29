#include <nxi/database.hpp>
#include <nxi/database/result.hpp>

#ifdef MAKE_STRUCT
#define init_open(Name)                                                                                                                              \
    namespace nxi                                                                                                                                    \
    {                                                                                                                                                \
        class result;                                                                                                                                \
        struct Name##_data                                                                                                                           \
        {
#define init_field(Index, Type, Name, SqlType) Type Name;
#define init_lfield(Index, Type, Name, SqlType) Type Name;
#define init_SQL(Data)
#define init_close(Name)                                                                                                                             \
    }                                                                                                                                                \
    ;                                                                                                                                                \
    } // nxi

#undef MAKE_STRUCT
#endif

#ifdef MAKE_MODEL
#define init_open(Name)                                                                                                                              \
    namespace nxi::data::Name::internal                                                                                                              \
    {                                                                                                                                                \
        inline void make(nxi::database& db)                                                                                                          \
        {                                                                                                                                            \
            db.exec(str_table_##Name.data());                                                                                                        \
        }                                                                                                                                            \
        void prepare(nxi::database&);                                                                                                                \
        inline static constexpr struct table_##Name                                                                                                  \
        {

#define init_field(Index, Type, Name, SqlType) inline static constexpr nxi::field<Index, Type> Name{};
#define init_lfield(Index, Type, Name, SqlType) inline static constexpr nxi::field<Index, Type> Name{};
#define init_SQL(Data)
#define init_close(Name)                                                                                                                             \
    }                                                                                                                                                \
    Name##_data{};                                                                                                                                   \
    } // nxi::data::Name::internal

#undef MAKE_MODEL
#endif

#ifdef MAKE_TABLE
#define init_open(Name)                                                                                                                              \
    namespace nxi::data::Name::internal                                                                                                              \
    {                                                                                                                                                \
        constexpr std::string_view str_table_##Name = "CREATE TABLE `" #Name "` ("

#define init_field(Index, Type, Name, SqlType) "`" #Name "` " SqlType ","
#define init_lfield(Index, Type, Name, SqlType) "`" #Name "` " SqlType
#define init_SQL(Data) Data
#define init_close(Name)                                                                                                                             \
    ")";                                                                                                                                             \
    } // nxi::data::Name::internal

#undef MAKE_TABLE
#endif