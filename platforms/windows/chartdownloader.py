import info

class subinfo( info.infoclass ):
    def setTargets( self ):
        self.svnTargets['master'] = "[git]https://github.com/knro/chartdownloader"
        self.shortDescription = 'Chart Downloader'
        self.defaultTarget = 'master'

    def setDependencies( self ):
        self.runtimeDependencies['libs/qt5/qtbase'] = None
        self.runtimeDependencies['libs/qt5/qtdeclarative'] = None
        self.runtimeDependencies['libs/qt5/qtquickcontrols'] = None
        self.runtimeDependencies['libs/qt5/qtquickcontrols2'] = None
        self.runtimeDependencies['qt-libs/poppler'] = None

from Package.CMakePackageBase import *

class Package(CMakePackageBase):
    def __init__(self):
        CMakePackageBase.__init__( self )
        self.ignoredPackages.append("binary/mysql")
        self.ignoredPackages.append("libs/llvm-meta")
        self.blacklist_file = ["blacklist.txt"]

    def createPackage(self):
        self.defines[ "productname" ] = "Chart Downloader"
        self.defines[ "executable" ] = "bin\\chartdownloader.exe"
        self.defines[ "icon" ] = os.path.join(os.path.dirname(__file__), "chartdownloader.ico")

        return TypePackager.createPackage(self)
