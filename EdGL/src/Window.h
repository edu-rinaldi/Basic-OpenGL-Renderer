#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct CursorPosition
{
	double posX, posY;
};

class Window
{
private:
	GLFWwindow* m_Window;
	std::string m_Title;
	float m_Width;
	float m_Height;

public:
	Window(const char* windowTitle, float width, float height);
	~Window();

	inline const std::string& GetTitle() const { return m_Title; };
	inline float GetWidth() const { return m_Width; }
	inline float GetHeight() const { return m_Height; }
	int GetKeyStatus(int key) const;
	void GetCursorPosition(double* posX, double* posY) const;
	CursorPosition GetCursorPosition() const;

	void SetWindowTitle(const char* title);
	void SetInputMode(int mode, int value) const;
	void SetWindowShouldClose(bool value) const;
	void SetMultiSample(bool value) const;
	int WindowShouldClose() const;
	void MakeContextCurrent() const;
	void SwapBuffers() const;
	
	

};

class WindowBuilder
{
public:
	WindowBuilder();
	WindowBuilder& AddWindowHint(int windowHint, int value);
	std::shared_ptr<Window> Build(const char* windowTitle, float width, float height) const;

private:
	bool m_MultiSample;
	
};


