/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "filesystem.hpp"

#if defined(E2D_PLATFORM) && E2D_PLATFORM == E2D_PLATFORM_MACOSX

#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#import <Foundation/Foundation.h>

namespace
{
    using namespace e2d;

    const mode_t default_directory_mode = S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH;

    bool extract_home_directory(str& dst) {
        NSString* path = NSHomeDirectory();
        if ( path ) {
            dst.assign([path UTF8String]);
            return true;
        }
        return false;
    }

    bool extract_appdata_directory(str& dst) {
        NSArray<NSString*>* paths = NSSearchPathForDirectoriesInDomains(
            NSApplicationSupportDirectory, NSUserDomainMask, YES);
        if ( paths && paths.count > 0 ) {
            dst.assign([[paths objectAtIndex:0] UTF8String]);
            return true;
        }
        return false;
    }

    bool extract_desktop_directory(str& dst) {
        NSArray<NSString*>* paths = NSSearchPathForDirectoriesInDomains(
            NSDesktopDirectory, NSUserDomainMask, YES);
        if ( paths && paths.count > 0 ) {
            dst.assign([[paths objectAtIndex:0] UTF8String]);
            return true;
        }
        return false;
    }

    bool extract_working_directory(str& dst) {
        NSString* filename = [[NSFileManager defaultManager] currentDirectoryPath];
        if ( filename ) {
            dst.assign([filename UTF8String]);
            return true;
        }
        return false;
    }

    bool extract_documents_directory(str& dst) {
        NSArray<NSString*>* paths = NSSearchPathForDirectoriesInDomains(
            NSDocumentDirectory, NSUserDomainMask, YES);
        if ( paths && paths.count > 0 ) {
            dst.assign([[paths objectAtIndex:0] UTF8String]);
            return true;
        }
        return false;
    }

    bool extract_resources_directory(str& dst) {
        NSString* filename = [[NSBundle mainBundle] resourcePath];
        if ( filename ) {
            dst.assign([filename UTF8String]);
            return true;
        }
        return false;
    }

    bool extract_executable_path(str& dst) {
        NSString* filename = [[NSBundle mainBundle] executablePath];
        if ( filename ) {
            dst.assign([filename UTF8String]);
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
