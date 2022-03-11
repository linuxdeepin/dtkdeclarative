TEMPLATE = aux
QMAKE_DOCS = $$PWD/dtkdeclarative.qdocconf

qtver.name = QT_VERSION
qtver.value = $$QT_VERSION
isEmpty(qtver.value): error("No version for documentation specified.")
qtmver.name = QT_VER
qtmver.value = $$replace(qtver.value, ^(\\d+\\.\\d+).*$, \\1)
qtvertag.name = QT_VERSION_TAG
qtvertag.value = $$replace(qtver.value, \.,)
qtdocs.name = QT_INSTALL_DOCS
qtdocs.value = $$[QT_INSTALL_DOCS/src]
builddir.name = BUILDDIR
builddir.value = $$OUT_PWD
QT_TOOL_ENV = qtver qtmver qtvertag qtdocs builddir
qtPrepareTool(QDOC, qdoc)
qtPrepareTool(QHELPGENERATOR, qhelpgenerator)
QT_TOOL_ENV =

QMAKE_DOCS_BASE_OUTDIR = $$OUT_PWD
QMAKE_DOCS_TARGET = $$replace(QMAKE_DOCS, ^(.*/)?(.*)\\.qdocconf$, \\2)
isEmpty(QMAKE_DOCS_TARGETDIR): QMAKE_DOCS_TARGETDIR = $$QMAKE_DOCS_TARGET
QMAKE_DOCS_OUTPUTDIR = $$QMAKE_DOCS_BASE_OUTDIR/$$QMAKE_DOCS_TARGETDIR

QDOC += -outputdir $$shell_quote($$QMAKE_DOCS_OUTPUTDIR)
DOC_INDEXES = -indexdir $$[QT_INSTALL_DOCS]

doc_command = $$QDOC $$QMAKE_DOCS
generate_docs.commands += $$doc_command -generate $$DOC_INDEXES
qch_docs.commands = $$QHELPGENERATOR $$shell_quote($$QMAKE_DOCS_OUTPUTDIR/$${QMAKE_DOCS_TARGET}.qhp) -o $$shell_quote($$QMAKE_DOCS_BASE_OUTDIR/$${QMAKE_DOCS_TARGET}.qch)

QMAKE_EXTRA_TARGETS += generate_docs qch_docs install_generate_docs install_qch_docs
docs.commands = $(MAKE) -f $(MAKEFILE) generate_docs && $(MAKE) -f $(MAKEFILE) qch_docs
QMAKE_EXTRA_TARGETS += docs

#TODO: Make docs can automatically update the file of install
install_generate_docs.files = $$QMAKE_DOCS_OUTPUTDIR
install_generate_docs.path = $$[QT_INSTALL_DOCS]
install_generate_docs.CONFIG += directory no_build
install_generate_docs.depends = docs
INSTALLS += install_generate_docs

install_qch_docs.files = $$QMAKE_DOCS_BASE_OUTDIR/$${QMAKE_DOCS_TARGET}.qch
install_qch_docs.path = $$[QT_INSTALL_DOCS]
install_qch_docs.CONFIG += no_build
install_qch_docs.depends = docs
INSTALLS += install_qch_docs

OTHER_FILES += \
    $$files($$PWD/chinese/src/*.qdoc) \
    $$files($$PWD/chinese/src/navigation/*.qdoc) \
