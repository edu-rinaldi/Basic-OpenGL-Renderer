#include "Window.h"
#include "ErrorHandler.h"

Window::Window(const char* windowTitle, float width, float height) :
	m_Window(glfwCreateWindow(width, height, windowTitle, NULL, NULL)),
	m_Title(windowTitle),
	m_Width(width),
	m_Height(height)
{
	ASSERT(width > 0 && height > 0);
	ASSERT(m_Window);
}

Window::~Window()
{
	glfwDestroyWindow(m_Window);
}

void Window::SetWindowTitle(const char* title)
{
	m_Title = title;
	glfwSetWindowTitle(m_Window, title);
}

void Window::SetInputMode(int mode, int value) const
{
	glfwSetInputMode(m_Window, mode, value);
}

void Window::SetWindowShouldClose(bool value) const
{
	glfwSetWindowShouldClose(m_Window, value ? GL_TRUE : GL_FALSE);
}

void Window::SetMultiSample(bool value) const
{
	if(value) glEnable(GL_MULTISAMPLE);
	else glDisable(GL_MULTISAMPLE);
}

int Window::WindowShouldClose() const
{
	return glfwWindowShouldClose(m_Window);
}

void Window::MakeContextCurrent() const
{
	glfwMakeContextCurrent(m_Window);
}

void Window::SwapBuffers() const
{
	glfwSwapBuffers(m_Window);
}

int Window::GetKeyStatus(int key) const
{
	return glfwGetKey(m_Window, key);
}

void Window::GetCursorPosition(double* posX, double* posY) const
{
	glfwGetCursorPos(m_Window, posX, posY);
}

CursorPosition Window::GetCursorPosition() const
{
	double posX, posY;
	GetCursorPosition(&posX, &posY);
	return CursorPosition{ posX, posY };
}

WindowBuilder::WindowBuilder() : m_MultiSample(false)
{
	glfwDefaultWindowHints();
}

WindowBuilder& WindowBuilder::AddWindowHint(int windowHint, int value)
{
	glfwWindowHint(windowHint, value);
	if(windowHint == GL_SAMPLES) m_MultiSample = true;
	return *this;
}

std::shared_ptr<Window> WindowBuilder::Build(const char* windowTitle, float width, float height) const
{
	auto window = std::make_shared<Window>(windowTitle, width, height);
	window->SetMultiSample(m_MultiSample);
	return window;
}
