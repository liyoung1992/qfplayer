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

	//不管成功与否都释放frame空间
	virtual void repaint(AVFrame *frame);

	QfVideoWidget(QWidget *parent);
	~QfVideoWidget();
protected:
	//刷新显示
	void paintGL();

	//初始化gl
	void initializeGL();

	// 窗口尺寸变化
	void resizeGL(int width, int height);
private:
	std::mutex mux;

	//shader程序
	QGLShaderProgram program;

	//shader中yuv变量地址
	GLuint unis[3] = { 0 };
	//openg的 texture地址
	GLuint texs[3] = { 0 };

	//材质内存空间
	unsigned char *datas[3] = { 0 };

	int width = 240;
	int height = 128;

};


#endif //QfVideoWidget_H


