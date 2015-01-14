#include "FileIO.h"

FileIO::FileIO()
{
}


FileIO::~FileIO()
{
}


QString FileIO::LoadFile(QString URI)
{
	QString text;
	QString line;

	QFile file(URI);
	if (!file.exists())
		std::cout << "File doesn't exist" << std::endl;
	
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream stream(&file);
		while (!stream.atEnd())
		{
			line = stream.readLine();
			text.append(line);
			text.append("\n");
		}
	}

	if (file.error())
	{
		throw LoadException(file.errorString().toStdString());
	}

	file.close();

	return text;
}

void FileIO::SaveFile(QString URI, QString code)
{
	QFile file(URI);
	if (file.exists())
	{
		file.remove();
	}

	file.open(QIODevice::ReadWrite | QIODevice::Text);
	if (file.error())
	{
		throw SaveException(file.errorString().toStdString());
	}
	else
	{
		QTextStream stream(&file);
		stream << code;
	}

	file.close();
}

void FileIO::SaveFile(std::shared_ptr<QFile> file, QString code)
{
	if (file->exists())
	{
		file->remove();
	}

	file->open(QIODevice::ReadWrite | QIODevice::Text);
	if (file->error())
	{
		throw SaveException(file->errorString().toStdString());
	}
	else
	{
		QTextStream stream(file.get());
		stream << code;
	}

	file->close();
}