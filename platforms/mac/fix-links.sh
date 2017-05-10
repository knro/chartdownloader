CHART_DIR=/Users/jasem/Projects/build/chartdownloader-Desktop-Release
#IGNORED_OTOOL_OUTPUT="/Qt|qt5|${CHART_DIR}/chartdownloader.app/|/usr/lib/|/System/"
IGNORED_OTOOL_OUTPUT="/usr/local/Cellar"
function addFileToCopy
{
        for e in "${FILES_TO_COPY[@]}"
    do
        if [ "$e" == "$1" ]
        then
            return 0
        fi
    done

        FILES_TO_COPY+=($1)
}

function processTarget
{
        target=$1

    #entries=$(otool -L $target | sed '1d' | awk '{print $1}' | egrep -v "$IGNORED_OTOOL_OUTPUT")
    entries=$(otool -L $target | sed '1d' | awk '{print $1}' | egrep "$IGNORED_OTOOL_OUTPUT")
    echo "Processing $target"
    echo "Enteries $entries"

    relativeRoot="${CHART_DIR}/chartdownloader.app/Contents"

    pathToFrameworks="${pathToFrameworks}Frameworks/"


        for entry in $entries
        do
                baseEntry=$(basename $entry)
                newname=""

        # the pathToFrameworks should end in a slash
        #
        # if [[ "$entry" == @rpath* ]]
        # then
        #     newname="@rpath/${pathToFrameworks}Frameworks/${baseEntry}"
        # else
        #     newname="@executable_path/${pathToFrameworks}Frameworks/${baseEntry}"
        # fi

        # Now I think that the @rpaths need to change to @executable_path
        #
        newname="@executable_path/../Frameworks/${baseEntry}.framework/Versions/5/${baseEntry}"

        echo "    change $entry -> $newname"
        echo "          install_name_tool -change \\"
        echo "              $entry \\"
        echo "              $newname \\"
        echo "              $target"


       install_name_tool -change $entry $newname $target

        addFileToCopy "$entry"
        done
    echo ""
    echo "   otool for $target after"
    otool -L $target

}

function fixPlugins()
{
   pluginsDir="${CHART_DIR}/chartdownloader.app/Contents/PlugIns"
   for dir in "$pluginsDir"/*/; do
       oneDir=$(basename $dir)
       echo "Dir: $oneDir"
       for file in "$dir"/*; do
           oneFile=$(basename $file)
           echo "File: $oneFile"
           install_name_tool -id @executable_path/../PlugIns/${oneDir}/${oneFile} ${CHART_DIR}/chartdownloader.app/Contents/PlugIns/${oneDir}/${oneFile}
           processTarget "${CHART_DIR}/chartdownloader.app/Contents/PlugIns/${oneDir}/${oneFile}"
       done
   done


}

processTarget "${CHART_DIR}/chartdownloader.app/Contents/Frameworks/QtGui.framework/Versions/5/QtGui"
processTarget "${CHART_DIR}/chartdownloader.app/Contents/Frameworks/QtWidgets.framework/Versions/5/QtWidgets"
processTarget "${CHART_DIR}/chartdownloader.app/Contents/Frameworks/QtNetwork.framework/Versions/5/QtNetwork"
processTarget "${CHART_DIR}/chartdownloader.app/Contents/Frameworks/QtPrintSupport.framework/Versions/5/QtPrintSupport"
processTarget "${CHART_DIR}/chartdownloader.app/Contents/Frameworks/QtQml.framework/Versions/5/QtQml"
processTarget "${CHART_DIR}/chartdownloader.app/Contents/Frameworks/QtQuick.framework/Versions/5/QtQuick"
processTarget "${CHART_DIR}/chartdownloader.app/Contents/Frameworks/QtQuickControls2.framework/Versions/5/QtQuickControls2"
processTarget "${CHART_DIR}/chartdownloader.app/Contents/Frameworks/QtQuickTemplates2.framework/Versions/5/QtQuickTemplates2"
processTarget "${CHART_DIR}/chartdownloader.app/Contents/Frameworks/QtXml.framework/Versions/5/QtXml"

install_name_tool -change /usr/local/Cellar/poppler/0.54.0/lib/libpoppler.67.dylib @executable_path/../Frameworks/libpoppler.67.dylib ${CHART_DIR}/chartdownloader.app/Contents/Frameworks/libpoppler-qt5.1.dylib

fixPlugins

