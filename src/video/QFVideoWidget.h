#ifndef QFVIDEOWIDGET_H
#define QFVIDEOWIDGET_H


#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QGLShaderProgram>
#include <mutex>
#include "../utils/pch.h"
class QFVideoWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT
public:
	void init(int width, int height);

	//���ܳɹ�����ͷ�frame�ռ�
	virtual void repaint(AVFrame *frame);

	QFVideoWidget(QWidget *parent);
	~QFVideoWidget();
protected:
	//ˢ����ʾ
	void paintGL();

	//��ʼ��gl
	void initializeGL();

	// ���ڳߴ�仯
	void resizeGL(int width, int height);
private:
	std::mutex mux;

	//shader����
	QGLShaderProgram program;

	//shader��yuv������ַ
	GLuint unis[3] = { 0 };
	//openg�� texture��ַ
	GLuint texs[3] = { 0 };

	//�����ڴ�ռ�
	unsigned char *datas[3] = { 0 };

	int width = 240;
	int height = 128;

};


#endif //QFVIDEOWIDGET_H

