#ifndef TOOLS_H
#define TOOLS_H
#include <QDir>
#include <QString>
#include <QDebug>
#include <vector>
#include <opencv2/core.hpp>


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


// 定义函数，输入cv::String路径，输出QString列表
std::vector<QString> getAllFilesInDirectory(const cv::String& directoryPath) {
    // 将cv::String转换为QString
    QString qPath = QString::fromStdString(directoryPath);

    // 使用QDir来遍历目录
    QDir dir(qPath);
    std::vector<QString> files;
    if (!dir.exists()) {
        qDebug() << "Directory does not exist: " << qPath;
        return files; // 如果目录不存在，返回空列表
    }

    // QDir::Files指定只列出文件
    // QDir::NoDotAndDotDot指定忽略特殊项 '.' 和 '..'
    QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot, QDir::Name);
    foreach (const QFileInfo &fileInfo, fileList) {
        files.push_back(fileInfo.absoluteFilePath()); // 添加文件的绝对路径到列表
    }

    return files;
}




#endif // TOOLS_H
