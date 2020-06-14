#include "QFVideoWidget.h"
#include <QDebug>
//�Զ���˫����
#define GET_STR(x) #x
#define A_VER 3
#define T_VER 4

FILE *fp = NULL;

//����shader
const char *vString = GET_STR(
	attribute vec4 vertexIn;
attribute vec2 textureIn;
varying vec2 textureOut;
void main(void)
{
	gl_Position = vertexIn;
	textureOut = textureIn;
}
);


//ƬԪshader
const char *tString = GET_STR(
	varying vec2 textureOut;
uniform sampler2D tex_y;
uniform sampler2D tex_u;
uniform sampler2D tex_v;
void main(void)
{
	vec3 yuv;
	vec3 rgb;
	yuv.x = texture2D(tex_y, textureOut).r;
	yuv.y = texture2D(tex_u, textureOut).r - 0.5;
	yuv.z = texture2D(tex_v, textureOut).r - 0.5;
	rgb = mat3(1.0, 1.0, 1.0,
		0.0, -0.39465, 2.03211,
		1.13983, -0.58060, 0.0) * yuv;
	gl_FragColor = vec4(rgb, 1.0);
}

);



//׼��yuv����
// ffmpeg -i v1080.mp4 -t 10 -s 240x128 -pix_fmt yuv420p  out240x128.yuv
QFVideoWidget::QFVideoWidget(QWidget *parent)
	: QOpenGLWidget(parent)
{
}

QFVideoWidget::~QFVideoWidget()
{
}

void QFVideoWidget::repaint(AVFrame *frame)
{
	if (!frame)return;
	mux.lock();
	//�ݴ���֤�ߴ���ȷ
	if (!datas[0] || width * height == 0 ||
		frame->width != this->width || frame->height != this->height)
	{
		av_frame_free(&frame);
		mux.unlock();
		return;
	}
	if (width == frame->linesize[0]) {
		memcpy(datas[0], frame->data[0], width*height);
		memcpy(datas[1], frame->data[1], width*height / 4);
		memcpy(datas[2], frame->data[2], width*height / 4);
	}
	else {
		//�ж�������
		for (int i = 0; i < height; i++) //Y 
			memcpy(datas[0] + width * i, frame->data[0] + frame->linesize[0] * i, width);
		for (int i = 0; i < height / 2; i++) //U
			memcpy(datas[1] + width / 2 * i, frame->data[1] + frame->linesize[1] * i, width);
		for (int i = 0; i < height / 2; i++) //V
			memcpy(datas[2] + width / 2 * i, frame->data[2] + frame->linesize[2] * i, width);
	}

	//�ж�������
	mux.unlock();
	av_frame_free(&frame);
	//ˢ����ʾ
	update();
}
void QFVideoWidget::init(const int& width, const int& height)
{
	mux.lock();
	this->width = width;
	this->height = height;
	if (datas[0]) {
		delete datas[0];
	}
	if (datas[1]) {
		delete datas[1];
	}
	if (datas[2]) {
		delete datas[2];
	}
	
	///��������ڴ�ռ�
	datas[0] = new unsigned char[width*height];		//Y
	datas[1] = new unsigned char[width*height / 4];	//U
	datas[2] = new unsigned char[width*height / 4];	//V


	if (texs[0])
	{
		glDeleteTextures(3, texs);
	}
	//��������
	glGenTextures(3, texs);

	//Y
	glBindTexture(GL_TEXTURE_2D, texs[0]);
	//�Ŵ���ˣ����Բ�ֵ   GL_NEAREST(Ч�ʸߣ�������������)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//���������Կ��ռ�
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

	//U
	glBindTexture(GL_TEXTURE_2D, texs[1]);
	//�Ŵ���ˣ����Բ�ֵ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//���������Կ��ռ�
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width / 2, height / 2, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

	//V
	glBindTexture(GL_TEXTURE_2D, texs[2]);
	//�Ŵ���ˣ����Բ�ֵ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//���������Կ��ռ�
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width / 2, height / 2, 0, GL_RED, GL_UNSIGNED_BYTE, 0);


	mux.unlock();


}
//��ʼ��opengl
void QFVideoWidget::initializeGL()
{
	qDebug() << "initializeGL";
	mux.lock();
	//��ʼ��opengl ��QOpenGLFunctions�̳У����� 
	initializeOpenGLFunctions();

	//program����shader�������ƬԪ���ű�
	//ƬԪ�����أ�
	qDebug() << program.addShaderFromSourceCode(QGLShader::Fragment, tString);
	//����shader
	qDebug() << program.addShaderFromSourceCode(QGLShader::Vertex, vString);

	//���ö�������ı���
	program.bindAttributeLocation("vertexIn", A_VER);

	//���ò�������
	program.bindAttributeLocation("textureIn", T_VER);

	//����shader
	qDebug() << "program.link() = " << program.link();

	qDebug() << "program.bind() = " << program.bind();

	//���ݶ���Ͳ�������
	//����
	static const GLfloat ver[] = {
		-1.0f,-1.0f,
		1.0f,-1.0f,
		-1.0f, 1.0f,
		1.0f,1.0f
	};

	//����
	static const GLfloat tex[] = {
		0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f
	};

	//����
	glVertexAttribPointer(A_VER, 2, GL_FLOAT, 0, 0, ver);
	glEnableVertexAttribArray(A_VER);

	//����
	glVertexAttribPointer(T_VER, 2, GL_FLOAT, 0, 0, tex);
	glEnableVertexAttribArray(T_VER);


	//��shader��ȡ����
	unis[0] = program.uniformLocation("tex_y");
	unis[1] = program.uniformLocation("tex_u");
	unis[2] = program.uniformLocation("tex_v");

	mux.unlock();

	//fp = fopen("out240x128.yuv", "rb");
	//if (!fp)
	//{
	//	qDebug() << "out240x128.yuv file open failed!";
	//}


	////������ʱ��
	//QTimer *ti = new QTimer(this);
	//connect(ti, SIGNAL(timeout()), this, SLOT(update()));
	//ti->start(40);
}

//ˢ����ʾ
void QFVideoWidget::paintGL()
{
	//if (feof(fp))
	//{
	//	fseek(fp, 0, SEEK_SET);
	//}
	//fread(datas[0], 1, width*height, fp);
	//fread(datas[1], 1, width*height / 4, fp);
	//fread(datas[2], 1, width*height / 4, fp);
	mux.lock();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texs[0]); //0��󶨵�Y����
										   //�޸Ĳ�������(�����ڴ�����)
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RED, GL_UNSIGNED_BYTE, datas[0]);
	//��shader uni��������
	glUniform1i(unis[0], 0);


	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, texs[1]); //1��󶨵�U����
										   //�޸Ĳ�������(�����ڴ�����)
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width / 2, height / 2, GL_RED, GL_UNSIGNED_BYTE, datas[1]);
	//��shader uni��������
	glUniform1i(unis[1], 1);


	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, texs[2]); //2��󶨵�V����
										   //�޸Ĳ�������(�����ڴ�����)
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width / 2, height / 2, GL_RED, GL_UNSIGNED_BYTE, datas[2]);
	//��shader uni��������
	glUniform1i(unis[2], 2);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	qDebug() << "paintGL";
	mux.unlock();
}


// ���ڳߴ�仯
void QFVideoWidget::resizeGL(int width, int height)
{
	mux.lock();
	qDebug() << "resizeGL " << width << ":" << height;
	mux.unlock();
}