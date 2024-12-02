#include <QCoreApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
//	QCoreApplication a(argc, argv);
	
	if(argc != 3){
		qDebug() << "FmeUnzip(Uncompressing Application): $ FmeUnzip in.fz out.fme";
		exit(1);
	} else{
		qDebug() << "Uncompressing" << argv[1] << "to" << argv[2];
	}

	FILE *in = fopen(argv[1], "rb+");
	if(in == NULL){
		qDebug() << "Cannot open file" << argv[1];
		exit(1);
	}
	FILE *out = fopen(argv[2], "wb+");
	if(out == NULL){
		qDebug() << "Cannot open file" << argv[2];
		fclose(in);
		exit(1);
	}

	while(1){
		// Compressed length
		int sz;
		if(fread(&sz, sizeof(int), 1, in) < 1){
			if(feof(in)){
				qDebug() << "Uncompressing done";
				break;
			}
			qDebug() << "Cannot read file(length)" << argv[1];
			break;
		}
			
		// Load compressed file
		char b;
		QByteArray zip;
		for(int i=0; i<sz; i++){
			if(fread(&b, sizeof(char), 1, in) < 1){			// load data
				qDebug() << "Missing the data(lack of data)" << argv[1];
				goto err_exit;
			}
			zip.append(b);
		}	 
	
		// Uncompress and store
		QByteArray unz = qUncompress(zip);
		foreach(b, unz){
			if(fwrite(&b, sizeof(char), 1, out) < 1){		// store uncompressed data
				qDebug() << "Cannot write file(data)" << argv[2];
				goto err_exit;
			}
		}
	}

err_exit:
	fclose(in);
	fclose(out);

//	return a.exec();
}
