/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "filesystem.hpp"

#if defined(E2D_PLATFORM) && E2D_PLATFORM == E2D_PLATFORM_LINUX

#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

namespace
{
    using namespace e2d;

    const mode_t default_directory_mode = S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH;

    bool extract_home_directory(str& dst) {
        const char* const home_path = std::getenv("HOME");
        if ( home_path ) {
            dst.assign(home_path);
            return true;
        }
        return false;
    }

    bool extract_appdata_directory(str& dst) {
        return extract_home_directory(dst);
    }

    bool extract_desktop_directory(str& dst) {
        str home_directory;
        if ( extract_home_directory(home_directory) ) {
            dst = path::combine(home_directory, "Desktop");
            return true;
        }
        return false;
    }

    bool extract_documents_directory(str& dst) {
        str home_directory;
        if ( extract_home_directory(home_directory) ) {
            dst = path::combine(home_directory, "Documents");
            return true;
        }
        return false;
    }

    bool extract_working_directory(str& dst) {
        char buf[PATH_MAX + 1] = {0};
        if ( ::getcwd(buf, E2D_COUNTOF(buf) - 1) ) {
            dst.assign(buf);
            return true;
        }
        return false;
    }

    bool extract_executable_path(str& dst) {
        char buf[PATH_MAX + 1] = {0};
        if ( ::readlink("/proc/self/exe", buf, E2D_COUNTOF(buf) - 1) != -1 ) {
            dst.assign(buf);
            return true;
        }
        return false;
    }

    bool extract_resources_directory(str& dst) {
        str executable_path;
        if ( extract_executable_path(executable_path) ) {
            dst = path::parent_path(executable_path);
            return true;
        }
        return false;
    }
}

namespace e2d { namespace filesystem { namespace impl
{
    bool remove_file(str_view path) {
        return 0 == ::unlink(make_utf8(path).c_str())
            || errno == ENOENT;
    }

    bool remove_directory(str_view path) {
        return 0 == ::rmdir(make_utf8(path).c_str())
            || errno == ENOENT;
    }

    bool file_exists(str_view path) {
        struct stat st{};
        return 0 == ::stat(make_utf8(path).c_str(), &st)
            && S_ISREG(st.st_mode);
    }

    bool directory_exists(str_view path) {
        struct stat st{};
        return 0 == ::stat(make_utf8(path).c_str(), &st)
            && S_ISDIR(st.st_mode);
    }

    bool create_directory(str_view path) {
        return 0 == ::mkdir(make_utf8(path).c_str(), default_directory_mode)
            || errno == EEXIST;
    }

    bool trace_directory(str_view path, const trace_func& func) {
        std::unique_ptr<DIR, decltype(&::closedir)> dir{
            ::opendir(make_utf8(path).c_str()),
            ::closedir};
        if ( !dir ) {
            return false;
        }
        while ( dirent* ent = ::readdir(dir.get()) ) {
            if ( 0 != std::strcmp(ent->d_name, ".") && 0 != std::strcmp(ent->d_name, "..") ) {
                if ( !func || !func(ent->d_name, DT_DIR == ent->d_type) ) {
                    return false;
                }
            }
        }
        return true;
    }

    bool extract_predef_path(str& dst, predef_path path_type) {
        switch ( path_type ) {
            case predef_path::home:
                return extract_home_directory(dst);
            case predef_path::appdata:
                return extract_appdata_directory(dst);
            case predef_path::desktop:
                return extract_desktop_directory(dst);
            case predef_path::working:
                return extract_working_directory(dst);
            case predef_path::documents:
                return extract_documents_directory(dst);
            case predef_path::resources:
                return extract_resources_directory(dst);
            case predef_path::executable:
                return extract_executable_path(dst);
            default:
                E2D_ASSERT_MSG(false, "unexpected predef path");
                return false;
        }
    }
}}}

#endif
