// Copyright (c) 2014-2024 LG Electronics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// SPDX-License-Identifier: Apache-2.0

#ifndef GLIB_H_MOCK_
#define GLIB_H_MOCK_

#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <string>

typedef char gchar;
typedef int gint;
typedef int gboolean;
typedef void* gpointer;

#define TRUE 1
#define FALSE 0

// GOptionEntry for command line parsing
typedef enum {
    G_OPTION_ARG_NONE,
    G_OPTION_ARG_STRING,
    G_OPTION_ARG_INT,
    G_OPTION_ARG_CALLBACK,
    G_OPTION_ARG_FILENAME,
    G_OPTION_ARG_STRING_ARRAY,
    G_OPTION_ARG_FILENAME_ARRAY,
    G_OPTION_ARG_DOUBLE,
    G_OPTION_ARG_INT64
} GOptionArg;

typedef struct _GOptionEntry {
    const gchar *long_name;
    gchar short_name;
    gint flags;
    GOptionArg arg;
    gpointer arg_data;
    const gchar *description;
    const gchar *arg_description;
} GOptionEntry;

typedef struct _GOptionContext GOptionContext;

struct _GOptionContext {
    const char* summary;
};

typedef struct _GError {
    int domain;
    int code;
    char* message;
} GError;

typedef struct _GMainLoop GMainLoop;

struct _GMainLoop {
    gboolean is_running;
    int ref_count;
};

typedef struct _GMainContext GMainContext;

typedef enum {
    G_FILE_TEST_IS_REGULAR = 1 << 0,
    G_FILE_TEST_IS_SYMLINK = 1 << 1,
    G_FILE_TEST_IS_DIR = 1 << 2,
    G_FILE_TEST_IS_EXECUTABLE = 1 << 3,
    G_FILE_TEST_EXISTS = 1 << 4
} GFileTest;

typedef enum {
    G_SPAWN_SEARCH_PATH = 1 << 0
} GSpawnFlags;

// Forward declare for external mock control
extern bool g_file_test_mock_result;
extern bool g_file_test_symlink_result;
extern std::string g_file_test_expected_path;

inline gboolean g_file_test(const gchar *filename, GFileTest test) {
    if (!filename) return FALSE;

    // Check for symlink test first
    if (test & G_FILE_TEST_IS_SYMLINK) {
        return g_file_test_symlink_result ? TRUE : FALSE;
    }

    return g_file_test_mock_result ? TRUE : FALSE;
}

inline gchar* g_path_get_dirname(const gchar *file_name) {
    if (!file_name) return nullptr;
    std::string path(file_name);
    size_t pos = path.rfind('/');
    if (pos == std::string::npos) {
        return strdup(".");
    }
    return strdup(path.substr(0, pos).c_str());
}

inline gchar* g_build_path(const gchar *separator, ...) {
    // Simple implementation for tests
    va_list args;
    va_start(args, separator);
    std::string result;
    const gchar* part;
    while ((part = va_arg(args, const gchar*)) != nullptr) {
        if (!result.empty() && result.back() != '/') {
            result += separator;
        }
        result += part;
    }
    va_end(args);
    return strdup(result.c_str());
}

inline void g_free(gpointer mem) {
    free(mem);
}

inline GError* g_error_new(int domain, int code, const char* format, ...) {
    GError* error = (GError*)malloc(sizeof(GError));
    if (error) {
        error->domain = domain;
        error->code = code;
        error->message = strdup(format);
    }
    return error;
}

inline void g_error_free(GError *error) {
    if (error) {
        if (error->message) free(error->message);
        free(error);
    }
}

inline gchar* g_strdup(const gchar *str) {
    return str ? strdup(str) : nullptr;
}

inline void g_printerr(const gchar *format, ...) {
    // No-op for tests
}

inline gboolean g_spawn_sync(const gchar *working_directory,
                              gchar **argv,
                              gchar **envp,
                              GSpawnFlags flags,
                              void* child_setup,
                              gpointer user_data,
                              gchar **standard_output,
                              gchar **standard_error,
                              gint *exit_status,
                              GError **error) {
    if (exit_status) *exit_status = 0;
    return TRUE;
}

// GOptionContext functions
inline GOptionContext* g_option_context_new(const gchar *parameter_string) {
    GOptionContext* ctx = (GOptionContext*)malloc(sizeof(GOptionContext));
    if (ctx) {
        ctx->summary = parameter_string;
    }
    return ctx;
}

inline void g_option_context_add_main_entries(GOptionContext *context, 
                                               const GOptionEntry *entries,
                                               const gchar *translation_domain) {
    // No-op for tests
}

inline gboolean g_option_context_parse(GOptionContext *context,
                                        gint *argc,
                                        gchar ***argv,
                                        GError **error) {
    return TRUE;
}

inline void g_option_context_free(GOptionContext *context) {
    if (context) {
        free(context);
    }
}

// GMainLoop functions
inline GMainLoop* g_main_loop_new(GMainContext *context, gboolean is_running) {
    GMainLoop* loop = (GMainLoop*)malloc(sizeof(GMainLoop));
    if (loop) {
        loop->is_running = is_running;
        loop->ref_count = 1;
    }
    return loop;
}

inline void g_main_loop_run(GMainLoop *loop) {
    if (loop) {
        loop->is_running = TRUE;
    }
}

inline void g_main_loop_quit(GMainLoop *loop) {
    if (loop) {
        loop->is_running = FALSE;
    }
}

inline void g_main_loop_unref(GMainLoop *loop) {
    if (loop) {
        loop->ref_count--;
        if (loop->ref_count <= 0) {
            free(loop);
        }
    }
}

inline gboolean g_main_loop_is_running(GMainLoop *loop) {
    return loop ? loop->is_running : FALSE;
}

#endif // GLIB_H_MOCK_
