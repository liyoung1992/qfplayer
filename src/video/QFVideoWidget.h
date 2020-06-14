#ifndef QfVideoWidget_H
#define QfVideoWidget_H


#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QGLShaderProgram>
#include <mutex>
#include "../utils/pch.h"
#include "QfVideoCallback.h"
class QfVideoWidget : public QOpenGLWidget,
	protected QOpenGLFunctions,public QFVideoCallback
{
	Q_OBJECT
public:
	virtual void init(const int& width, const int& height);

	//���ܳɹ�����ͷ�frame�ռ�
	virtual void repaint(AVFrame *frame);

	QfVideoWidget(QWidget *parent);
	~QfVideoWidget();
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


#endif //QfVideoWidget_H


