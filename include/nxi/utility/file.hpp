#ifndef INCLUDE_NXI_UTILITY_FILE_HPP_NXI
#define INCLUDE_NXI_UTILITY_FILE_HPP_NXI

#include <nxi/log.hpp>
#include <QFile>
#include <QString>

namespace nxi
{
    inline QString read_file(const QString& path)
    {
        QString result;
        QFile file(path);
        if(!file.open(QIODevice::ReadOnly)) nxi_warning("{} not found", path);
        else
        {
            result = file.readAll();
            file.close();
        }
        return result;
    }
} // nxi

#endif // INCLUDE_NXI_UTILITY_FILE_HPP_NXI
