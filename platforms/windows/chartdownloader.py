import info
from Packager.NullsoftInstallerPackager import *

class subinfo( info.infoclass ):
    def setTargets( self ):
        self.svnTargets['master'] = "[git]https://github.com/knro/chartdownloader"
        self.shortDescription = 'Chart Downloader'
        self.defaultTarget = 'master'

    def setDependencies( self ):
        self.dependencies['libs/qtbase'] = 'default'
        self.dependencies['libs/qtdeclarative'] = 'default'
        self.dependencies['libs/qtquickcontrols'] = 'default'
        self.dependencies['libs/qtquickcontrols2'] = 'default'        
        self.dependencies['qt-libs/poppler'] = 'default'
        
from Package.QMakePackageBase import *
      
class Package( QMakePackageBase, NullsoftInstallerPackager ):
    def __init__( self):
        QMakePackageBase.__init__( self )
        blacklists = [NSIPackagerLists.runtimeBlacklist,os.path.join(os.path.dirname(__file__), 'blacklist.txt')]
        NullsoftInstallerPackager.__init__(self, blacklists=blacklists)

    def createPackage(self):
        self.defines[ "productname" ] = "Chart Downloader"
        self.defines[ "executable" ] = "bin\\chartdownloader.exe"
        self.defines[ "icon" ] = os.path.join(os.path.dirname(__file__), "chartdownloader.ico")

        return NullsoftInstallerPackager.createPackage(self)
       
    def preArchive(self):
        archiveDir = self.archiveDir()
        binPath = os.path.join(archiveDir, "bin")        
        utils.mergeTree(os.path.join(archiveDir, "plugins"), binPath)        
        utils.mergeTree(os.path.join(archiveDir, "qml"), os.path.join(archiveDir, binPath))        
        
    def install( self ):
        installDir = self.installDir()
        print("Install Dir is : " + installDir)
        buildDir = self.buildDir()
        print("Build Dir is : " + buildDir)
        if not QMakePackageBase.install( self ):
            return False
        
        # Must create bin dir
        os.mkdir( os.path.join( self.installDir(), "bin" ) )        
        # Copy chartdownloader to it
        # copy over dlls as required by KDE convention
        for file in os.listdir( os.path.join( buildDir, "release" ) ):
            if file.endswith( ".exe" ):
                utils.copyFile( os.path.join( self.buildDir(), "release" , file ), os.path.join( self.installDir(), "bin" , file ) )            
            #if file.endswith( ".dll" ):
                #utils.copyFile( os.path.join( self.installDir(), "lib" , file ), os.path.join( self.installDir(), "bin" , file ) )
        return True
