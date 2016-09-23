#-------------------------------------------------
#
# Project created by QtCreator 2016-09-20T08:36:19
#
#-------------------------------------------------

TARGET   = lzstring_test
TEMPLATE = app

QT      += testlib
QT      -= gui

CONFIG  += console
CONFIG  -= app_bundle debug_and_release debug_and_release_target

DEFINES += SRCDIR=\\\"$$PWD/\\\"
SOURCES += lzstring_test.cpp

include(../../src/lzstring.pri)

defineReplace(__quote) {
    return(\"$$1\")
}

defineTest(__copyFilesToDestdir) {
    files = $$1
    isEmpty(files) {
        return(1)
    }
    for(file, files) {

        PARTS  = $$split(file, |)
        FILE   = $$member(PARTS, 0)
        SUBDIR = $$member(PARTS, 1)
        DIR    = $$OUT_PWD/$$SUBDIR

        # Replace / with \.
        win32:FILE ~= s,/,\\,g
        win32:DIR  ~= s,/,\\,g

        if (!isEmpty(SUBDIR)) {
            QMAKE_PRE_LINK += $(CHK_DIR_EXISTS) $$__quote($$DIR) $(MKDIR) $$__quote($$DIR) $$escape_expand(\\n\\t)
        }
        QMAKE_PRE_LINK += $(COPY_FILE) $$__quote($$FILE) $$__quote($$DIR) $$escape_expand(\\n\\t)
    }

    export(QMAKE_PRE_LINK)
}

COPY_FILES += $$PWD/data.json
__copyFilesToDestdir($$COPY_FILES)
