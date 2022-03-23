#include <iostream>

//OpenGL ����� ���ؼ�, �ܺο��� �߰��� ���̺귯��
#include <GL/glew.h>	//GLEW
#include <GLFW/glfw3.h> //GLFW
//#include <GL/wglew.h> Windows Only

//Window Dimensions, by constant
const GLint WIDTH{ 800 }, HEIGHT{ 600 };

int main() {
//Initialize OpenGL Context
//Initialize GLFW
	if (!glfwInit()) { //���� ��
		std::cout << "GLFW Initialization failed" << std::endl;
		
		//glfw �� �����Ѵ�
		glfwTerminate();
		return 1; //error return
	}


//������� â�� ���� �����ֱ�
//Setup GLFW window properties

	//OpenGL Context Version => ���ǿ����� 3.3 ������ ���
	//Ȯ�ΰ��, ���� ��ǻ�ʹ� 4.6 ������ ��� ���̴�
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	//������ �ڵ�� ���� �ʰڴ�
	//���� ȣȯ���� ���� �ʰڴ�
	//Core profile = No Backwoards Compatilbility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//���� ȣȯ��, ���� ������ ������ �װ��� ����ϰڴ�
	//Allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


//â�� ���� �����

	//Create the window
	//�տ��� ������ constant �� = â�� ���� ũ��
	GLFWwindow* mainWindow{ glfwCreateWindow(WIDTH, HEIGHT, "Test Window", nullptr, nullptr) };
	if (!mainWindow) { //â ����� �� ����
		std::cout << "GLFW window creation failed!" << std::endl;
		glfwTerminate();
		return 1;
	}

	//Get Buffer Size information
	//Window �� ������ �Ѿ�� ���۰� �ִ� ~ �� ������ ũ�⸦ �˾Ƴ���
	int bufferWidth{}, bufferHeight{};
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//Set opengl context for GLEW to use
	//���� Window �� GLEW �� ����, �� window �� �׸��� �׸� ���̴�
	glfwMakeContextCurrent(mainWindow);



	//Allow modern extension features
	glewExperimental = GL_TRUE;

	//Initialize the GLEW library
	if (glewInit() != GLEW_OK) {
		std::cout << "GLFW window creation failed!" << std::endl;
		glfwDestroyWindow(mainWindow); //â�� �ݰ�
		glfwTerminate(); //����
		return 1;
	}


	//Setup Viewport size
	//â���� �׸��� ������ ����
	//������ ũ�Ⱑ �׸��� ����
	glViewport(0, 0, bufferWidth, bufferHeight);


	//Loop until window closed
	//�� ������ â�� ��� �����ְ� �Ѵ�
	while (!glfwWindowShouldClose(mainWindow)) {

		//Get + Handle user input events
		//Ű����, ���콺 �Է� �ν�
		glfwPollEvents();

		//Clear window
		//â�� ��ü ȭ�� ���� ����
		glClearColor(1.0f, 1.0f, 0.0f, 1.0f); //RGBA

		//������ ���� ������ ��, �տ��� ������ ������ color buffer �� ä���
		glClear(GL_COLOR_BUFFER_BIT);

		//Buffer swap
		//���⼭ ���۸� ���� => ȭ�鿡 ��Ÿ������ �Ѵ�
		glfwSwapBuffers(mainWindow);
	}

	return 0;
}