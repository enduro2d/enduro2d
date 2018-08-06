/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "filesystem_impl.hpp"

#if defined(E2D_FILESYSTEM_MODE) && E2D_FILESYSTEM_MODE == E2D_FILESYSTEM_MODE_POSIX

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

namespace
{
    mode_t default_directory_mode = S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH;
}

namespace e2d { namespace filesystem { namespace impl
{
    bool remove_file(str_view path) {
        return
            0 == ::unlink(make_utf8(path).c_str())
            || errno == ENOENT;
    }

    bool remove_directory(str_view path) {
        return
            0 == ::rmdir(make_utf8(path).c_str())
            || errno == ENOENT;
    }

    bool file_exists(str_view path) {
        struct stat st;
        return
            0 == ::stat(make_utf8(path).c_str(), &st)
            && S_ISREG(st.st_mode);
    }

    bool directory_exists(str_view path) {
        struct stat st;
        return
            0 == ::stat(make_utf8(path).c_str(), &st)
            && S_ISDIR(st.st_mode);
    }

    bool create_directory(str_view path) {
        return
            0 == ::mkdir(make_utf8(path).c_str(), default_directory_mode)
            || errno == EEXIST;
    }
}}}

#endif
