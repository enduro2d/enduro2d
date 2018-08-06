/*******************************************************************************
 * This file is part of the "enduro2d"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "filesystem_impl.hpp"

#if defined(E2D_FILESYSTEM_MODE) && E2D_FILESYSTEM_MODE == E2D_FILESYSTEM_MODE_WINAPI

#include <windows.h>

namespace e2d { namespace filesystem { namespace impl
{
    bool remove_file(str_view path) {
        const wstr wide_path = make_wide(path);
        return
            ::DeleteFileW(wide_path.c_str())
            || ::GetLastError() == ERROR_FILE_NOT_FOUND
            || ::GetLastError() == ERROR_PATH_NOT_FOUND;
    }

    bool remove_directory(str_view path) {
        const wstr wide_path = make_wide(path);
        return
            ::RemoveDirectoryW(wide_path.c_str())
            || ::GetLastError() == ERROR_FILE_NOT_FOUND
            || ::GetLastError() == ERROR_PATH_NOT_FOUND;
    }

    bool file_exists(str_view path) {
        const wstr wide_path = make_wide(path);
        DWORD attributes = ::GetFileAttributesW(wide_path.c_str());
        return
            attributes != INVALID_FILE_ATTRIBUTES &&
            !(attributes & FILE_ATTRIBUTE_DIRECTORY);
    }

    bool directory_exists(str_view path) {
        const wstr wide_path = make_wide(path);
        DWORD attributes = ::GetFileAttributesW(wide_path.c_str());
        return
            attributes != INVALID_FILE_ATTRIBUTES &&
            (attributes & FILE_ATTRIBUTE_DIRECTORY);
    }

    bool create_directory(str_view path) {
        const wstr wide_path = make_wide(path);
        return
            ::CreateDirectoryW(wide_path.c_str(), nullptr) ||
            ::GetLastError() == ERROR_ALREADY_EXISTS;
    }
}}}

#endif
