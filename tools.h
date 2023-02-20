#ifndef TOOLS_H
#define TOOLS_H
#include <QDir>


inline bool DeleteFileOrFolder( const QString& strPath )
{
    if( strPath.isEmpty() || !QDir().exists( strPath ) )
        return false;

    QFileInfo fileInfo( strPath );

    if( fileInfo.isFile() )
        QFile::remove( strPath );
    else if( fileInfo.isDir() )
    {
        QDir qDir(strPath);
        qDir.setFilter( QDir::AllEntries | QDir::NoDotAndDotDot );
        QFileInfoList fileInfoLst = qDir.entryInfoList();
        foreach( QFileInfo qFileInfo, fileInfoLst )
        {
            if( qFileInfo.isFile() )
                qDir.remove( qFileInfo.absoluteFilePath() );
            else
            {
                DeleteFileOrFolder( qFileInfo.absoluteFilePath() );
                qDir.rmdir( qFileInfo.absoluteFilePath() );
            }
        }
        qDir.rmdir( fileInfo.absoluteFilePath() );
    }

    return true;
}

#endif // TOOLS_H
