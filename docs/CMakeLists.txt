cmake_minimum_required (VERSION 3.10)

find_package (Doxygen REQUIRED)

set (QCH_INSTALL_DESTINATION ${CMAKE_INSTALLL_PREFIX}/share/DDE/dtk CACHE STRING "QCH install location")

set (DOXYGEN_GENERATE_HTML "NO" CACHE STRING "Doxygen HTML output")
set (DOXYGEN_GENERATE_XML "NO" CACHE STRING "Doxygen XML output")
set (DOXYGEN_GENERATE_QHP "YES" CACHE STRING "Doxygen QHP output")
set (DOXYGEN_FILE_PATTERNS *.cpp *.h *.zh_CN.md *.zh_CN.dox CACHE STRING "Doxygen File Patterns")
set (DOXYGEN_PROJECT_NUMBER ${CMAKE_PROJECT_VERSION} CACHE STRING "") # Should be the same as this project is using.
set (DOXYGEN_EXTRACT_STATIC YES)
set (DOXYGEN_OUTPUT_LANGUAGE "Chinese" CACHE STRING "Doxygen Output Language")
set (DOXYGEN_IMAGE_PATH ${CMAKE_CURRENT_LIST_DIR}/images/)
set (DOXYGEN_QHG_LOCATION "qhelpgenerator")
set (DOXYGEN_QHP_NAMESPACE "org.deepin.dtk.declarative")
set (DOXYGEN_QCH_FILE "dtkdeclarative.qch")
set (DOXYGEN_QHP_VIRTUAL_FOLDER "dtkdeclarative")
set (DOXYGEN_HTML_EXTRA_STYLESHEET "" CACHE STRING "Doxygen custom stylesheet for HTML output")
set (DOXYGEN_TAGFILES "qtcore.tags=qthelp://org.qt-project.qtcore/qtcore/" CACHE STRING "Doxygen tag files")

set (DOXYGEN_PREDEFINED
    "D_DECL_DEPRECATED_X(x)="
    "\"DCORE_BEGIN_NAMESPACE=namespace Dtk { namespace Core {\""
    "\"DCORE_END_NAMESPACE=}}\""
    "\"DCORE_USE_NAMESPACE=using namespace Dtk::Core\;\""
    "\"DQUICK_BEGIN_NAMESPACE=namespace Dtk { namespace Quick {\""
    "\"DQUICK_END_NAMESPACE=}}\""
    "\"DQUICK_USE_NAMESPACE=using namespace Dtk::Quick\;\""
)
set (DOXYGEN_MACRO_EXPANSION "YES")
set (DOXYGEN_EXPAND_ONLY_PREDEF "YES")

doxygen_add_docs (doxygen
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/docs
    ALL
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "Generate documentation via Doxygen"
)

install (FILES ${PROJECT_BINARY_DIR}/docs/html/dtkdeclarative.qch DESTINATION ${QCH_INSTALL_DESTINATION})
