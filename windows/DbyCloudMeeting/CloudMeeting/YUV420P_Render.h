#ifndef YUV420P_RENDER_H
#define YUV420P_RENDER_H

#include <QObject>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
class YUV420P_Render : protected QOpenGLFunctions
{
public:
	YUV420P_Render();
	~YUV420P_Render();

	//初始化gl
	void initialize();
	//刷新显示
	void render(uint8_t* y, int y_linesize, uint8_t* u, int u_linesize, uint8_t* v, int v_linesize, int width, int height);
	void render(const QByteArray& ptr, int width, int height, int type);

	void setGLView(int x, int y, int w, int h);
	void setBackgroundImage();
private:
	//shader程序
	QOpenGLShaderProgram m_program;
	//shader中yuv变量地址
	GLuint m_textureUniformY, m_textureUniformU, m_textureUniformV;
	//创建纹理
	GLuint m_idy, m_idu, m_idv;

};

#endif // YUV420P_RENDER_H
