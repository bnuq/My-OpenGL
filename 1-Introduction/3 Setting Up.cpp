#include <iostream>

//OpenGL 사용을 위해서, 외부에서 추가한 라이브러리
#include <GL/glew.h>	//GLEW
#include <GLFW/glfw3.h> //GLFW
//#include <GL/wglew.h> Windows Only

//Window Dimensions, by constant
const GLint WIDTH{ 800 }, HEIGHT{ 600 };

int main() {
//Initialize OpenGL Context
//Initialize GLFW
	if (!glfwInit()) { //실패 시
		std::cout << "GLFW Initialization failed" << std::endl;
		
		//glfw 를 종료한다
		glfwTerminate();
		return 1; //error return
	}


//만들려는 창에 대한 정보넣기
//Setup GLFW window properties

	//OpenGL Context Version => 강의에서는 3.3 버전을 사용
	//확인결과, 현재 컴퓨터는 4.6 버전을 사용 중이다
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	//오래된 코드는 쓰지 않겠다
	//하위 호환성을 두지 않겠다
	//Core profile = No Backwoards Compatilbility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//상위 호환성, 나중 버전이 들어오면 그것을 사용하겠다
	//Allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


//창을 직접 만들기

	//Create the window
	//앞에서 설정한 constant 값 = 창의 실제 크기
	GLFWwindow* mainWindow{ glfwCreateWindow(WIDTH, HEIGHT, "Test Window", nullptr, nullptr) };
	if (!mainWindow) { //창 만드는 데 실패
		std::cout << "GLFW window creation failed!" << std::endl;
		glfwTerminate();
		return 1;
	}

	//Get Buffer Size information
	//Window 로 정보가 넘어가는 버퍼가 있다 ~ 그 버퍼의 크기를 알아낸다
	int bufferWidth{}, bufferHeight{};
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//Set opengl context for GLEW to use
	//만든 Window 와 GLEW 를 연결, 이 window 에 그림을 그릴 것이다
	glfwMakeContextCurrent(mainWindow);



	//Allow modern extension features
	glewExperimental = GL_TRUE;

	//Initialize the GLEW library
	if (glewInit() != GLEW_OK) {
		std::cout << "GLFW window creation failed!" << std::endl;
		glfwDestroyWindow(mainWindow); //창을 닫고
		glfwTerminate(); //종료
		return 1;
	}


	//Setup Viewport size
	//창에서 그리는 영역을 지정
	//버퍼의 크기가 그리는 영역
	glViewport(0, 0, bufferWidth, bufferHeight);


	//Loop until window closed
	//그 전까지 창이 계속 열려있게 한다
	while (!glfwWindowShouldClose(mainWindow)) {

		//Get + Handle user input events
		//키보드, 마우스 입력 인식
		glfwPollEvents();

		//Clear window
		//창의 전체 화면 색을 설정
		glClearColor(1.0f, 1.0f, 0.0f, 1.0f); //RGBA

		//버퍼의 여러 데이터 중, 앞에서 설정한 색으로 color buffer 를 채우고
		glClear(GL_COLOR_BUFFER_BIT);

		//Buffer swap
		//여기서 버퍼를 스왑 => 화면에 나타나도록 한다
		glfwSwapBuffers(mainWindow);
	}

	return 0;
}