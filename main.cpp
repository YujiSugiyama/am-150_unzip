/***************************************************************************************************************************************
**
** Copyright (C) 2024 by Fukuda Medical Engineerring 
** https://www.fukuda-me.co.jp/
**
** 本ソフトウェアの使用、複製、変更、および／または頒布は、有償無償を問わず、いかなる目的においても許可されます。
** 本ソフトウェアは「現状有姿」で提供され、作者は本ソフトウェアに関して、商品性および適合性の黙示保証を含むすべての保証を否認します。
** 本ソフトウェアは「現状有姿」で提供され、本ソフトウェアに関するすべての保証（商品性および適合性に関する黙示の保証を含む）を著者は一切行いません。
** いかなる場合においても、著者は本ソフトウェアの使用または性能に起因または関連して生じた特別損害、直接的損害、間接的損害、派生的損害、
** または使用、データ、または利益の損失から生じたいかなる損害についても、契約、過失、またはその他の不法行為にかかわらず一切の責任を負いません。
**
** Permission to use, copy, modify, and/or distribute this software for any purpose
** with or without fee is hereby granted.
** THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
** REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
** FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
** INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
** OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
** TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF
** THIS SOFTWARE.
**
**--------------------------------------------------------------------------------------------------------------------------------------
** $QT_BEGIN_LICENSE:BSD$
** This software uses the part of the Qt Toolkit as follows.
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************************************************************************/
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
